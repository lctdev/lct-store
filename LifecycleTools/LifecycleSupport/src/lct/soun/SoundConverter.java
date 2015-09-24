package lct.soun;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import lct.util.BinaryUtility;
import lct.util.DataException;
import lct.util.PathUtility;

public class SoundConverter {
	private static final String RIFF_ID = "RIFF";
	private static final String WAVE_TYPE = "WAVE";
	private static final String FORMAT_ID = "fmt ";
	private static final String DATA_ID = "data";
	
	private static final int CHUNK_HEADER_SIZE = 4;
	private static final int RIFF_INFO_SIZE = 4;
	private static final int FORMAT_INFO_BASIC_SIZE = 16;
	private static final int FORMAT_INFO_COMPLETE_SIZE = 18;
	
	private static final int WAVE_FORMAT_PCM = 0x001;
	//private static final int WAVE_FORMAT_IEEE_FLOAT = 0x0003;
	
	static class ChunkHeader {
		String id;				// char[4]
		int size;				// u32
	}
	
	static class RIFFInfo {
		String riffType;		// char[4]
	}
	
	static class FormatInfo {
		int formatTag;			// u16
		int channels;			// u16
		int samplesPerSec;		// u32
		int avgBytesPerSec;		// u32
		int blockAlign;			// u16
		int bitsPerSample;		// u16
		int extSize;			// u16
	}
	
	public Wave loadWaveWAV(String filePath) {
		try {
			BufferedInputStream bufferedInputStream = new BufferedInputStream(new FileInputStream(filePath));
			
			ChunkHeader fileHeader = readChunkHeader(bufferedInputStream);
			
			if (fileHeader.id.equals(RIFF_ID)) {
				int filePos = 0;
				
				RIFFInfo riffInfo = readRIFFInfo(bufferedInputStream);
				filePos += RIFF_INFO_SIZE;
				if (riffInfo.riffType.equals(WAVE_TYPE)) {	
					FormatInfo formatInfo = null;
					int size = 0;
					int sampleSize = 0;
					int sampleCount = 0;
					int sampleLength = 0;
					Object[] sampleArray = null;
					
					while (filePos < fileHeader.size) {
						ChunkHeader chunkHeader = readChunkHeader(bufferedInputStream);
						filePos += CHUNK_HEADER_SIZE;
						
						switch (chunkHeader.id) {
						case FORMAT_ID:
						{
							boolean readExt = chunkHeader.size != FORMAT_INFO_BASIC_SIZE;
							formatInfo = readFormatInfo(bufferedInputStream, readExt);
							if (formatInfo.formatTag != WAVE_FORMAT_PCM) {
								throw new DataException("Unsupported Sample Format(Unknown)");
							}
						}
						break;
						case DATA_ID:
						{
							size = chunkHeader.size;
							sampleSize = formatInfo.bitsPerSample / 8;
							sampleCount = size / sampleSize;
							sampleLength = sampleCount / formatInfo.channels;
							if (sampleSize == 2) {
								sampleArray = readSampleDataS16(bufferedInputStream, sampleCount);
							}
							else {
								throw new DataException("Unsupported Sample Size(Unknown)");
							}
						}
						break;
						default:
							bufferedInputStream.read(new byte[chunkHeader.size]);
						}
						filePos += chunkHeader.size;
					}
					
					Wave wave = new Wave();
					wave.name = PathUtility.getFileNameNoExtension(filePath);
					wave.channelCount = formatInfo.channels;
					wave.sampleSize = sampleSize;
					wave.sampleLength = sampleLength;
					wave.sampleRate = formatInfo.samplesPerSec;
					wave.sampleArray = sampleArray;
					
					bufferedInputStream.close();
					
					return wave;
				}
				else {
					throw new DataException("Unsupported WAV Format(Unknown)");
				}
			}
			else {
				throw new DataException("Unsupported WAV Format(Unknown)");
			}
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
			return null;
		}
	}
	
	public void storeWaveWAV(Wave wave, String filePath) {
		try {
			BufferedOutputStream bufferedOutputStream = new BufferedOutputStream(new FileOutputStream(filePath));
			
			ChunkHeader fileHeader = new ChunkHeader();
			fileHeader.id = RIFF_ID;
			fileHeader.size = 0;
			
			RIFFInfo riffInfo = new RIFFInfo();
			riffInfo.riffType = WAVE_TYPE;
			
			fileHeader.size += RIFF_INFO_SIZE;
			
			ChunkHeader formatHeader = new ChunkHeader();
			formatHeader.id = FORMAT_ID;
			formatHeader.size = FORMAT_INFO_COMPLETE_SIZE;			
			
			FormatInfo formatInfo = new FormatInfo();
			formatInfo.formatTag = WAVE_FORMAT_PCM;
			formatInfo.channels = wave.channelCount;
			formatInfo.samplesPerSec = wave.sampleRate;
			formatInfo.avgBytesPerSec = wave.sampleRate * wave.sampleSize;
			formatInfo.blockAlign = wave.sampleSize * wave.channelCount;
			formatInfo.bitsPerSample = wave.sampleSize * 8;
			formatInfo.extSize = 0;
			
			fileHeader.size += CHUNK_HEADER_SIZE;
			fileHeader.size += formatHeader.size;
			
			ChunkHeader dataHeader = new ChunkHeader();
			dataHeader.id = DATA_ID;
			dataHeader.size = wave.sampleArray.length * wave.sampleSize;
			
			fileHeader.size += CHUNK_HEADER_SIZE;
			fileHeader.size += dataHeader.size;
			
			writeChunkHeader(fileHeader, bufferedOutputStream);
			writeRIFFInfo(riffInfo, bufferedOutputStream);
			
			writeChunkHeader(formatHeader, bufferedOutputStream);
			writeFormatInfo(formatInfo, bufferedOutputStream, true);
			
			writeChunkHeader(dataHeader, bufferedOutputStream);
			writeSampleDataS16((Wave.SampleS16[])wave.sampleArray, bufferedOutputStream);
			
			bufferedOutputStream.close();
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
	}
	
	public ChunkHeader readChunkHeader(InputStream inputStream) throws IOException {
		ChunkHeader chunkHeader = new ChunkHeader();
		chunkHeader.id = new String(BinaryUtility.readCharArray(inputStream, 4));
		chunkHeader.size = BinaryUtility.readU32(inputStream, false);
		
		return chunkHeader;
	}
	
	public void writeChunkHeader(ChunkHeader chunkHeader, OutputStream outputStream) throws IOException {
		BinaryUtility.writeCharArray(outputStream, chunkHeader.id.toCharArray());
		BinaryUtility.writeU32(outputStream, chunkHeader.size, false);
	}
	
	public RIFFInfo readRIFFInfo(InputStream inputStream) throws IOException {
		RIFFInfo riffInfo = new RIFFInfo();
		riffInfo.riffType = new String(BinaryUtility.readCharArray(inputStream, 4));
		
		return riffInfo;
	}
	
	public void writeRIFFInfo(RIFFInfo riffInfo, OutputStream outputStream) throws IOException {
		BinaryUtility.writeCharArray(outputStream, riffInfo.riffType.toCharArray());
	}
	
	public FormatInfo readFormatInfo(InputStream inputStream, boolean readExt) throws IOException {
		FormatInfo formatInfo = new FormatInfo();
		formatInfo.formatTag = BinaryUtility.readU16(inputStream, false);
		formatInfo.channels = BinaryUtility.readU16(inputStream, false);
		formatInfo.samplesPerSec = BinaryUtility.readU32(inputStream, false);
		formatInfo.avgBytesPerSec = BinaryUtility.readU32(inputStream, false);
		formatInfo.blockAlign = BinaryUtility.readU16(inputStream, false);
		formatInfo.bitsPerSample = BinaryUtility.readU16(inputStream, false);
		if (readExt) {
			formatInfo.extSize = BinaryUtility.readU16(inputStream, false);
			if (formatInfo.extSize > 0) {
				inputStream.read(new byte[formatInfo.extSize]);
			}
		}
		else {
			formatInfo.extSize = 0;
		}
		
		return formatInfo;
	}
	
	public void writeFormatInfo(FormatInfo formatInfo, OutputStream outputStream, boolean writeExt) throws IOException {
		BinaryUtility.writeU16(outputStream, formatInfo.formatTag, false);
		BinaryUtility.writeU16(outputStream, formatInfo.channels, false);
		BinaryUtility.writeU32(outputStream, formatInfo.samplesPerSec, false);
		BinaryUtility.writeU32(outputStream, formatInfo.avgBytesPerSec, false);
		BinaryUtility.writeU16(outputStream, formatInfo.blockAlign, false);
		BinaryUtility.writeU16(outputStream, formatInfo.bitsPerSample, false);
		if (writeExt) {
			BinaryUtility.writeU16(outputStream, formatInfo.extSize, false);
		}
	}
	
	public Wave.SampleS16[] readSampleDataS16(InputStream inputStream, int sampleCount) throws IOException {
		Wave.SampleS16[] sampleArray = new Wave.SampleS16[sampleCount];
		
		for (int sampleIndex = 0; sampleIndex < sampleCount; ++sampleIndex) {
			Wave.SampleS16 sample = new Wave.SampleS16();
			sample.v = BinaryUtility.readS16(inputStream, false);
			sampleArray[sampleIndex] = sample;
		}
		
		return sampleArray;
	}
	
	public void writeSampleDataS16(Wave.SampleS16[] sampleArray, OutputStream outputStream) throws IOException {
		for (int sampleIndex = 0; sampleIndex < sampleArray.length; ++sampleIndex) {
			Wave.SampleS16 sample = sampleArray[sampleIndex];
			BinaryUtility.writeS16(outputStream, sample.v, false);
		}
	}
	
	/*
	public void writeTGAHeader(TGAHeader tgaHeader, OutputStream outputStream) throws IOException {
		BinaryUtility.writeU8(outputStream, tgaHeader.idLength);
		BinaryUtility.writeU8(outputStream, tgaHeader.colorMapType);
		BinaryUtility.writeU8(outputStream, tgaHeader.imageType);
		
		BinaryUtility.writeU16(outputStream, tgaHeader.colorMapSpecification.tableOffset, false);
		BinaryUtility.writeU16(outputStream, tgaHeader.colorMapSpecification.entryCount, false);
		BinaryUtility.writeU8(outputStream, tgaHeader.colorMapSpecification.bitsPerPixel);
		
		BinaryUtility.writeU16(outputStream, tgaHeader.imageSpecification.xOrigin, false);
		BinaryUtility.writeU16(outputStream, tgaHeader.imageSpecification.yOrigin, false);
		BinaryUtility.writeU16(outputStream, tgaHeader.imageSpecification.width, false);
		BinaryUtility.writeU16(outputStream, tgaHeader.imageSpecification.height, false);
		BinaryUtility.writeU8(outputStream, tgaHeader.imageSpecification.bitsPerPixel);
		BinaryUtility.writeU8(outputStream, tgaHeader.imageSpecification.descriptor);
	}
	
	public Texture.TexelRGBA[] readImageDataBGRA(int pixelCount, InputStream inputStream) throws IOException {
		Texture.TexelRGBA[] texelArray = new Texture.TexelRGBA[pixelCount];
		for (int pixelIndex = 0; pixelIndex < pixelCount; ++pixelIndex) {
			Texture.TexelRGBA texel = new Texture.TexelRGBA();
			texel.b = BinaryUtility.readU8(inputStream);
			texel.g = BinaryUtility.readU8(inputStream);
			texel.r = BinaryUtility.readU8(inputStream);
			texel.a = BinaryUtility.readU8(inputStream);
			texelArray[pixelIndex] = texel;
		}
		
		return texelArray;
	}
	
	public void writeImageDataBGRA(Texture.TexelRGBA[] texelArray, OutputStream outputStream) throws IOException {
		for (int pixelIndex = 0; pixelIndex < texelArray.length; ++pixelIndex) {
			Texture.TexelRGBA texel = texelArray[pixelIndex];
			BinaryUtility.writeU8(outputStream, texel.b);
			BinaryUtility.writeU8(outputStream, texel.g);
			BinaryUtility.writeU8(outputStream, texel.r);
			BinaryUtility.writeU8(outputStream, texel.a);
		}
	}
	*/
}