package lct.font;

import lct.util.BinaryUtility;
import lct.util.BinaryHeader;

import java.io.ByteArrayOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

public class BinaryConverter {
	private static final int MAX_NAME_SIZE = 32;
	
	private static final int VERSION = 0;
	
	public void storeSheet(Sheet sheet, String filePath, boolean bigEndian) {
		try {
			FileOutputStream fileOutputStream = new FileOutputStream(filePath);
			
			ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
			writeSheetData(sheet, byteArrayOutputStream, bigEndian);
			byte[] dataArray = byteArrayOutputStream.toByteArray();			
			
			BinaryHeader binaryHeader = new BinaryHeader();
			binaryHeader.groupCode = Constants.GROUP_CODE.toCharArray();
			binaryHeader.typeCode = Constants.SHEET_TYPE_CODE.toCharArray();
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
	
	private void writeSheetData(Sheet sheet, OutputStream outputStream, boolean bigEndian) throws IOException {
		BinaryUtility.writeStringBuffer(outputStream, sheet.name, MAX_NAME_SIZE);
		BinaryUtility.writeStringBuffer(outputStream, sheet.textureName, MAX_NAME_SIZE);
		BinaryUtility.writeF32(outputStream, sheet.cellWidth, bigEndian);
		BinaryUtility.writeF32(outputStream, sheet.cellHeight, bigEndian);
		BinaryUtility.writeF32(outputStream, sheet.cellBaselineX, bigEndian);
		BinaryUtility.writeF32(outputStream, sheet.cellBaselineY, bigEndian);
		BinaryUtility.writeF32(outputStream, sheet.ascent, bigEndian);
		BinaryUtility.writeF32(outputStream, sheet.descent, bigEndian);
		BinaryUtility.writeF32(outputStream, sheet.lineHeight, bigEndian);
		BinaryUtility.writeU32(outputStream, sheet.horizontalCellCount, bigEndian);
		BinaryUtility.writeU32(outputStream, sheet.verticalCellCount, bigEndian);
		BinaryUtility.writeU32(outputStream, sheet.glyphVector.size(), bigEndian);
		
		for (Sheet.Glyph glyph : sheet.glyphVector) {
			BinaryUtility.writeU16(outputStream, glyph.character, bigEndian);
			BinaryUtility.writePadding(outputStream, 2);
			BinaryUtility.writeF32(outputStream, glyph.cellX, bigEndian);
			BinaryUtility.writeF32(outputStream, glyph.cellY, bigEndian);
			BinaryUtility.writeF32(outputStream, glyph.width, bigEndian);
			BinaryUtility.writeF32(outputStream, glyph.leftBearing, bigEndian);
			BinaryUtility.writeF32(outputStream, glyph.rightBearing, bigEndian);
			BinaryUtility.writeF32(outputStream, glyph.advance, bigEndian);
		}
	}
}
