package lct.part;

import lct.util.BinaryUtility;
import lct.util.BinaryHeader;

import java.io.ByteArrayOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

public class BinaryConverter {
	private static final int MAX_NAME_SIZE = 32;
	
	private static final int VERSION = 0;
	
	public void storeField(Field field, String filePath, boolean bigEndian) {
		try {
			FileOutputStream fileOutputStream = new FileOutputStream(filePath);
			
			ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
			writeFieldData(field, byteArrayOutputStream, bigEndian);
			byte[] dataArray = byteArrayOutputStream.toByteArray();			
			
			BinaryHeader binaryHeader = new BinaryHeader();
			binaryHeader.groupCode = Constants.GROUP_CODE.toCharArray();
			binaryHeader.typeCode = Constants.FIELD_TYPE_CODE.toCharArray();
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
	
	private void writeFieldData(Field field, OutputStream outputStream, boolean bigEndian) throws IOException {
		BinaryUtility.writeStringBuffer(outputStream, field.name, MAX_NAME_SIZE);
		BinaryUtility.writeStringBuffer(outputStream, field.textureAtlasName, MAX_NAME_SIZE);
		
		BinaryUtility.writeU32(outputStream, field.emitterVector.size(), bigEndian);
		
		for (Field.Emitter emitter : field.emitterVector) {
			BinaryUtility.writeStringBuffer(outputStream, emitter.name, MAX_NAME_SIZE);
			BinaryUtility.writeStringBuffer(outputStream, emitter.textureRegionName, MAX_NAME_SIZE);
			
			BinaryUtility.writeF32(outputStream, emitter.x, bigEndian);
			BinaryUtility.writeF32(outputStream, emitter.y, bigEndian);
			BinaryUtility.writeF32(outputStream, emitter.shapeParameterA, bigEndian);
			BinaryUtility.writeF32(outputStream, emitter.shapeParameterB, bigEndian);
			BinaryUtility.writeF32(outputStream, emitter.globalAngle, bigEndian);
			
			{
				Field.Range range = emitter.expelAngleRange;
				BinaryUtility.writeF32(outputStream, range.min, bigEndian);
				BinaryUtility.writeF32(outputStream, range.max, bigEndian);
			}
			{
				Field.Range range = emitter.expelMultiplierRange;
				BinaryUtility.writeF32(outputStream, range.min, bigEndian);
				BinaryUtility.writeF32(outputStream, range.max, bigEndian);
			}
			{
				Field.Range range = emitter.scaleMultiplierRange;
				BinaryUtility.writeF32(outputStream, range.min, bigEndian);
				BinaryUtility.writeF32(outputStream, range.max, bigEndian);
			}
			{
				Field.Range range = emitter.rotationMultiplierRange;
				BinaryUtility.writeF32(outputStream, range.min, bigEndian);
				BinaryUtility.writeF32(outputStream, range.max, bigEndian);
			}
			{
				Field.Range range = emitter.scaleFluxRange;
				BinaryUtility.writeF32(outputStream, range.min, bigEndian);
				BinaryUtility.writeF32(outputStream, range.max, bigEndian);
			}
			
			{
				Field.Color color = emitter.color0;
				BinaryUtility.writeF32(outputStream, color.r, bigEndian);
				BinaryUtility.writeF32(outputStream, color.g, bigEndian);
				BinaryUtility.writeF32(outputStream, color.b, bigEndian);
			}
			{
				Field.Color color = emitter.color1;
				BinaryUtility.writeF32(outputStream, color.r, bigEndian);
				BinaryUtility.writeF32(outputStream, color.g, bigEndian);
				BinaryUtility.writeF32(outputStream, color.b, bigEndian);
			}
			
			BinaryUtility.writeF32(outputStream, emitter.delayFrames, bigEndian);
			BinaryUtility.writeF32(outputStream, emitter.frameDuration, bigEndian);
			BinaryUtility.writeF32(outputStream, emitter.particleFrameLifetime, bigEndian);
			BinaryUtility.writeU16(outputStream, emitter.particleCount, bigEndian);
			BinaryUtility.writeU16(outputStream, emitter.emitCount, bigEndian);
			BinaryUtility.writeU8(outputStream, emitter.shapeType.ordinal());
			BinaryUtility.writeU8(outputStream, emitter.arrangeType.ordinal());
			
			int flags = 0;
			for (Field.Emitter.FlagType flagType : emitter.flagTypeVector) {
				int flag = 1 << flagType.ordinal();
				flags |= flag;
			}
			BinaryUtility.writeU8(outputStream, flags);
			
			BinaryUtility.writeU8(outputStream, emitter.particleControlVector.size());
			
			for (Field.ParticleControl particleControl : emitter.particleControlVector) {
				BinaryUtility.writeU32(outputStream, particleControl.propertyType.ordinal(), bigEndian);
				BinaryUtility.writeF32(outputStream, particleControl.initial, bigEndian);
				BinaryUtility.writeF32(outputStream, particleControl.velocity, bigEndian);
				BinaryUtility.writeF32(outputStream, particleControl.acceleration, bigEndian);
				
				{
					Field.Range range = particleControl.velocityFrameRange;
					BinaryUtility.writeF32(outputStream, range.min, bigEndian);
					BinaryUtility.writeF32(outputStream, range.max, bigEndian);
				}
				{
					Field.Range range = particleControl.accelerationFrameRange;
					BinaryUtility.writeF32(outputStream, range.min, bigEndian);
					BinaryUtility.writeF32(outputStream, range.max, bigEndian);
				}
			}
		}
	}
}
