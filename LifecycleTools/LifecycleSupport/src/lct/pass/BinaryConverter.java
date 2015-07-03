package lct.pass;

import lct.util.BinaryUtility;
import lct.util.BinaryHeader;

import java.io.ByteArrayOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

public class BinaryConverter {
	private static final int MAX_NAME_SIZE = 32;
	
	private static final int VERSION = 0;
	
	public void storeString(StringAsset stringAsset, String filePath, boolean bigEndian) {
		try {
			FileOutputStream fileOutputStream = new FileOutputStream(filePath);
			
			ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
			writeStringData(stringAsset, byteArrayOutputStream, bigEndian);
			byte[] dataArray = byteArrayOutputStream.toByteArray();
			
			BinaryHeader binaryHeader = new BinaryHeader();
			binaryHeader.groupCode = Constants.GROUP_CODE.toCharArray();
			binaryHeader.typeCode = Constants.STRING_TYPE_CODE.toCharArray();
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
	
	private void writeStringData(StringAsset stringAsset, OutputStream outputStream, boolean bigEndian) throws IOException {
		BinaryUtility.writeStringBuffer(outputStream, stringAsset.name, MAX_NAME_SIZE);
		int stringLength = stringAsset.string.length();
		int stringSize = stringLength + 1;
		int roundedSize = stringSize + 3 & ~3;		
		BinaryUtility.writeU32(outputStream, stringLength, bigEndian);
		BinaryUtility.writeU32(outputStream, roundedSize, bigEndian);
		
		BinaryUtility.writeString(outputStream, stringAsset.string);
		
		int paddingSize = roundedSize - stringSize;
		if (paddingSize > 0) {
			BinaryUtility.writePadding(outputStream, paddingSize);
		}
	}
}
