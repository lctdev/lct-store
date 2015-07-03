package lct.spri;

import lct.util.BinaryUtility;
import lct.util.BinaryHeader;

import java.io.ByteArrayOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

public class BinaryConverter {
	private static final int MAX_NAME_SIZE = 32;
	
	private static final int VERSION = 0;
	
	public void storeFigure(Figure figure, String filePath, boolean bigEndian) {
		try {
			FileOutputStream fileOutputStream = new FileOutputStream(filePath);
			
			ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
			writeFigureData(figure, byteArrayOutputStream, bigEndian);
			byte[] dataArray = byteArrayOutputStream.toByteArray();			
			
			BinaryHeader binaryHeader = new BinaryHeader();
			binaryHeader.groupCode = Constants.GROUP_CODE.toCharArray();
			binaryHeader.typeCode = Constants.FIGURE_TYPE_CODE.toCharArray();
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
	
	private void writeFigureData(Figure figure, OutputStream outputStream, boolean bigEndian) throws IOException {
		BinaryUtility.writeStringBuffer(outputStream, figure.name, MAX_NAME_SIZE);
		BinaryUtility.writeU32(outputStream, figure.partVector.size(), bigEndian);
		BinaryUtility.writeU32(outputStream, figure.jointVector.size(), bigEndian);
		BinaryUtility.writeU32(outputStream, figure.patternVector.size(), bigEndian);
		
		for (Figure.Part part : figure.partVector) {
			BinaryUtility.writeStringBuffer(outputStream, part.name, MAX_NAME_SIZE);
			Figure.Part.Placement placement = part.placement;
			{
				BinaryUtility.writeF32(outputStream, placement.width, bigEndian);
				BinaryUtility.writeF32(outputStream, placement.height, bigEndian);
				BinaryUtility.writeF32(outputStream, placement.x, bigEndian);
				BinaryUtility.writeF32(outputStream, placement.y, bigEndian);
			}
			
			for (int partPropertyIndex = 0; partPropertyIndex < Figure.Part.PropertyType.values().length; ++partPropertyIndex) {
				float partProperty = part.propertyArray[partPropertyIndex];
				BinaryUtility.writeF32(outputStream, partProperty, bigEndian);
			}
			
			int jointIndex = FigureUtility.findJointIndex(figure, part.jointName);
			BinaryUtility.writeS32(outputStream, jointIndex, bigEndian);
			int patternIndex = FigureUtility.findPatternIndex(figure, part.patternName);
			BinaryUtility.writeS32(outputStream, patternIndex, bigEndian);
		}
		
		for (Figure.Joint joint : figure.jointVector) {
			BinaryUtility.writeStringBuffer(outputStream, joint.name, MAX_NAME_SIZE);
			
			for (int jointPropertyIndex = 0; jointPropertyIndex < Figure.Joint.PropertyType.values().length; ++jointPropertyIndex) {
				float jointProperty = joint.propertyArray[jointPropertyIndex];
				BinaryUtility.writeF32(outputStream, jointProperty, bigEndian);
			}
			
			int jointFlags = 0;
			for (int jointFlagIndex = 0; jointFlagIndex < Figure.Joint.FlagType.values().length; ++jointFlagIndex) {
				boolean jointFlag = joint.flagArray[jointFlagIndex];
				if (jointFlag)
				{
					jointFlags |= (1 << jointFlagIndex);
				}
			}
			BinaryUtility.writeU32(outputStream, jointFlags, bigEndian);
			
			int parentIndex = FigureUtility.findJointIndex(figure, joint.parentName);
			BinaryUtility.writeS32(outputStream, parentIndex, bigEndian);
		}
		
		for (Figure.Pattern pattern : figure.patternVector) {
			BinaryUtility.writeStringBuffer(outputStream, pattern.name, MAX_NAME_SIZE);
			BinaryUtility.writeStringBuffer(outputStream, pattern.textureTableName, MAX_NAME_SIZE);
			
			for (int patternPropertyIndex = 0; patternPropertyIndex < Figure.Pattern.PropertyType.values().length; ++patternPropertyIndex) {
				float patternProperty = pattern.propertyArray[patternPropertyIndex];
				BinaryUtility.writeF32(outputStream, patternProperty, bigEndian);
			}
		}
	}
	
	public void storeAnimation(Animation animation, String filePath, boolean bigEndian) {
		try {
			FileOutputStream fileOutputStream = new FileOutputStream(filePath);
			
			ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
			writeAnimationData(animation, byteArrayOutputStream, bigEndian);
			byte[] dataArray = byteArrayOutputStream.toByteArray();			
			
			BinaryHeader binaryHeader = new BinaryHeader();
			binaryHeader.groupCode = Constants.GROUP_CODE.toCharArray();
			binaryHeader.typeCode = Constants.ANIMATION_TYPE_CODE.toCharArray();
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
	
	private void writeAnimationData(Animation animation, OutputStream outputStream, boolean bigEndian) throws IOException {
		BinaryUtility.writeStringBuffer(outputStream, animation.name, MAX_NAME_SIZE);
		BinaryUtility.writeU32(outputStream, animation.trackVector.size(), bigEndian);
		for (Animation.Track track : animation.trackVector) {
			BinaryUtility.writeStringBuffer(outputStream, track.targetName, MAX_NAME_SIZE);
			BinaryUtility.writeU8(outputStream, track.targetType.ordinal());
			switch (track.targetType) {
				case PART:
					BinaryUtility.writeU8(outputStream, ((Figure.Part.PropertyType)track.targetPropertyType).ordinal());
					break;
				case JOINT:
					BinaryUtility.writeU8(outputStream, ((Figure.Joint.PropertyType)track.targetPropertyType).ordinal());
					break;
				case PATTERN:
					BinaryUtility.writeU8(outputStream, ((Figure.Pattern.PropertyType)track.targetPropertyType).ordinal());
					break;
			}
			BinaryUtility.writeU8(outputStream, track.finishType.ordinal());
			BinaryUtility.writePadding(outputStream, 1);
			BinaryUtility.writeU32(outputStream, track.keyVector.size(), bigEndian);
			
			for (Animation.Track.Key key : track.keyVector) {
				BinaryUtility.writeF32(outputStream, key.frame, bigEndian);
				BinaryUtility.writeF32(outputStream, key.value, bigEndian);
				BinaryUtility.writeF32(outputStream, key.slope, bigEndian);
				BinaryUtility.writeU32(outputStream, key.curveType.ordinal(), bigEndian);
			}
		}
	}
}
