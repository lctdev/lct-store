package lct.soun;

import java.io.ByteArrayOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

import lct.util.BinaryHeader;
import lct.util.BinaryUtility;

public class BinaryConverter {
	private static final int MAX_NAME_SIZE = 32;
	
	private static final int VERSION = 0;
	
	public void storeWave(Wave wave, String filePath, boolean bigEndian) {
		try {
			FileOutputStream fileOutputStream = new FileOutputStream(filePath);
			
			ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
			writeWaveData(wave, byteArrayOutputStream, bigEndian);
			byte[] dataArray = byteArrayOutputStream.toByteArray();			
			
			BinaryHeader binaryHeader = new BinaryHeader();
			binaryHeader.groupCode = Constants.GROUP_CODE.toCharArray();
			binaryHeader.typeCode = Constants.WAVE_TYPE_CODE.toCharArray();
			binaryHeader.bigEndian = bigEndian;
			binaryHeader.version = VERSION;
			binaryHeader.dataSize = dataArray.length;
			
			BinaryUtility.writeBinaryHeader(fileOutputStream, binaryHeader, bigEndian);
			fileOutputStream.write(dataArray);
			
			fileOutputStream.close();
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
	}
	
	private void writeWaveData(Wave wave, OutputStream outputStream, boolean bigEndian) throws IOException {
		BinaryUtility.writeStringBuffer(outputStream, wave.name, MAX_NAME_SIZE);
		BinaryUtility.writeU32(outputStream, wave.channelCount, bigEndian);
		BinaryUtility.writeU32(outputStream, wave.sampleSize, bigEndian);
		BinaryUtility.writeU32(outputStream, wave.sampleLength, bigEndian);
		BinaryUtility.writeU32(outputStream, wave.sampleRate, bigEndian);
		int size = wave.sampleArray.length * wave.sampleSize;
		size = BinaryUtility.roundSize(size);
		BinaryUtility.writeU32(outputStream, size, bigEndian); 
		
		if (wave.sampleSize == 2) {
			Wave.SampleS16[] sampleArray = (Wave.SampleS16[])wave.sampleArray;
			for (int sampleIndex = 0; sampleIndex < sampleArray.length; ++sampleIndex) {
				Wave.SampleS16 sample = sampleArray[sampleIndex];
				BinaryUtility.writeS16(outputStream, sample.v, bigEndian);
			}
			int dataSize = sampleArray.length * 2;
			int padSize = size - dataSize;
			if (padSize > 0) {
				BinaryUtility.writePadding(outputStream, padSize);
			}
		}
	}
	
	public void storeClip(Clip clip, String filePath, boolean bigEndian) {
		try {
			FileOutputStream fileOutputStream = new FileOutputStream(filePath);
			
			ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
			writeClipData(clip, byteArrayOutputStream, bigEndian);
			byte[] dataArray = byteArrayOutputStream.toByteArray();			
			
			BinaryHeader binaryHeader = new BinaryHeader();
			binaryHeader.groupCode = Constants.GROUP_CODE.toCharArray();
			binaryHeader.typeCode = Constants.CLIP_TYPE_CODE.toCharArray();
			binaryHeader.bigEndian = bigEndian;
			binaryHeader.version = VERSION;
			binaryHeader.dataSize = dataArray.length;
			
			BinaryUtility.writeBinaryHeader(fileOutputStream, binaryHeader, bigEndian);
			fileOutputStream.write(dataArray);
			
			fileOutputStream.close();
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
	}
	
	private void writeClipData(Clip clip, OutputStream outputStream, boolean bigEndian) throws IOException {
		BinaryUtility.writeStringBuffer(outputStream, clip.name, MAX_NAME_SIZE);
		
		for (int propertyIndex = 0; propertyIndex < Clip.PropertyType.values().length; ++propertyIndex) {
			float property = clip.propertyArray[propertyIndex];
			BinaryUtility.writeF32(outputStream, property, bigEndian);
		}
		
		BinaryUtility.writeBool(outputStream, clip.loop);
		BinaryUtility.writeU8(outputStream, clip.segmentVector.size());
		BinaryUtility.writePadding(outputStream, 2);
		
		for (Clip.Segment segment : clip.segmentVector) {
			BinaryUtility.writeStringBuffer(outputStream, segment.waveName, MAX_NAME_SIZE);
		}
	}
	
	public void storeRamp(Ramp ramp, String filePath, boolean bigEndian) {
		try {
			FileOutputStream fileOutputStream = new FileOutputStream(filePath);
			
			ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
			writeRampData(ramp, byteArrayOutputStream, bigEndian);
			byte[] dataArray = byteArrayOutputStream.toByteArray();			
			
			BinaryHeader binaryHeader = new BinaryHeader();
			binaryHeader.groupCode = Constants.GROUP_CODE.toCharArray();
			binaryHeader.typeCode = Constants.RAMP_TYPE_CODE.toCharArray();
			binaryHeader.bigEndian = bigEndian;
			binaryHeader.version = VERSION;
			binaryHeader.dataSize = dataArray.length;
			
			BinaryUtility.writeBinaryHeader(fileOutputStream, binaryHeader, bigEndian);
			fileOutputStream.write(dataArray);
			
			fileOutputStream.close();
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
	}
	
	private void writeRampData(Ramp ramp, OutputStream outputStream, boolean bigEndian) throws IOException {
		BinaryUtility.writeStringBuffer(outputStream, ramp.name, MAX_NAME_SIZE);
		BinaryUtility.writeU32(outputStream, ramp.trackVector.size(), bigEndian);
		for (Ramp.Track track : ramp.trackVector) {
			BinaryUtility.writeU8(outputStream, ((Clip.PropertyType)track.targetPropertyType).ordinal());
			BinaryUtility.writeU8(outputStream, track.finishType.ordinal());
			BinaryUtility.writePadding(outputStream, 2);
			BinaryUtility.writeF32(outputStream, track.loopSecond, bigEndian);
			BinaryUtility.writeU32(outputStream, track.keyVector.size(), bigEndian);
			
			for (Ramp.Track.Key key : track.keyVector) {
				BinaryUtility.writeF32(outputStream, key.second, bigEndian);
				BinaryUtility.writeF32(outputStream, key.value, bigEndian);
				BinaryUtility.writeF32(outputStream, key.slope, bigEndian);
				BinaryUtility.writeU32(outputStream, key.curveType.ordinal(), bigEndian);
			}
		}
	}
}
