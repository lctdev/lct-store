package lct.util;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;

public class BinaryUtility {
	private static final int BYTE_0_MASK = 0xFF000000;
	private static final int BYTE_1_MASK = 0x00FF0000;
	private static final int BYTE_2_MASK = 0x0000FF00;
	private static final int BYTE_3_MASK = 0x000000FF;
	
	private static final int BYTE_0_SHIFT = 24;
	private static final int BYTE_1_SHIFT = 16;
	private static final int BYTE_2_SHIFT = 8;
	private static final int BYTE_3_SHIFT = 0;
	
	public static int roundSize(int size) {
		int roundedSize = (size + 3) & ~0x03;
		return roundedSize;
	}
	
	public static void writeCharArray(OutputStream outputStream, char[] value) throws IOException {
		byte[] dataArray = new byte[value.length];
		for (int index = 0; index < value.length; ++index) {
			dataArray[index] = (byte)value[index];
		}
		outputStream.write(dataArray);
	}
	
	public static char[] readCharArray(InputStream inputStream, int length) throws IOException {
		byte[] dataArray = new byte[length];
		inputStream.read(dataArray);
		char[] value = new char[length];
		for (int index = 0; index < value.length; ++index) {
			value[index] = (char)dataArray[index];
		}
		return value;
	}
	
	public static void writeString(OutputStream outputStream, String value) throws IOException {
		byte[] dataArray = value.getBytes(StandardCharsets.US_ASCII);
		byte[] nullArray = new byte[1];
		outputStream.write(dataArray);
		outputStream.write(nullArray);
	}
	
	public static void writeStringBuffer(OutputStream outputStream, String value, int bufferSize) throws IOException {
		if (value.length() >= bufferSize) {
			throw new IllegalArgumentException("String size is greater than the buffer size");
		}
		byte[] dataArray = value.getBytes(StandardCharsets.US_ASCII);
		byte[] nullArray = new byte[bufferSize - value.length()];
		outputStream.write(dataArray);
		outputStream.write(nullArray);
	}
	
	public static void writeBool(OutputStream outputStream, boolean value) throws IOException {
		byte data = (byte)(value ? 1 : 0);
		outputStream.write(data);
	}
	
	public static void writeU8(OutputStream outputStream, int value) throws IOException {
		writeS8(outputStream, value);
	}
	
	public static int readU8(InputStream inputStream) throws IOException {
		return readS8(inputStream);
	}
	
	public static void writeS8(OutputStream outputStream, int value) throws IOException {
		byte data = (byte)value;
		outputStream.write(data);
	}
	
	public static int readS8(InputStream inputStream) throws IOException {
		int value = inputStream.read();
		return value;
	}
	
	public static void writeU16(OutputStream outputStream, int value, boolean bigEndian) throws IOException {
		writeS16(outputStream, value, bigEndian);
	}
	
	public static int readU16(InputStream inputStream, boolean bigEndian) throws IOException {
		byte[] dataArray = new byte[2];
		inputStream.read(dataArray);
		int value;
		if (bigEndian) {
			value = (((dataArray[0] << BYTE_2_SHIFT) & BYTE_2_MASK) | ((dataArray[1] << BYTE_3_SHIFT) & BYTE_3_MASK));
		}
		else {
			value = (((dataArray[0] << BYTE_3_SHIFT) & BYTE_3_MASK) | ((dataArray[1] << BYTE_2_SHIFT) & BYTE_2_MASK));
		}
		return value;
	}
	
	public static void writeS16(OutputStream outputStream, int value, boolean bigEndian) throws IOException {
		byte[] dataArray;
		if (bigEndian) {
			byte[] bigEndianDataArray = { 
					(byte)((value & BYTE_2_MASK) >> BYTE_2_SHIFT),
					(byte)((value & BYTE_3_MASK) >> BYTE_3_SHIFT),
			};
			dataArray = bigEndianDataArray;
		}
		else {
			byte[] littleEndianDataArray = { 
					(byte)((value & BYTE_3_MASK) >> BYTE_3_SHIFT),
					(byte)((value & BYTE_2_MASK) >> BYTE_2_SHIFT),
			};
			dataArray = littleEndianDataArray;
		}
		outputStream.write(dataArray);
	}
	
	public static int readS16(InputStream inputStream, boolean bigEndian) throws IOException {
		byte[] dataArray = new byte[2];
		inputStream.read(dataArray);
		int value;
		if (bigEndian) {
			value = (
					((dataArray[0] << BYTE_2_SHIFT) & BYTE_2_MASK) | 
					((dataArray[1] << BYTE_3_SHIFT) & BYTE_3_MASK)
					);
		}
		else {
			value = (
					((dataArray[0] << BYTE_3_SHIFT) & BYTE_3_MASK) | 
					((dataArray[1] << BYTE_2_SHIFT) & BYTE_2_MASK)
					);
		}
		return value;
	}
	
	public static void writeU32(OutputStream outputStream, int value, boolean bigEndian) throws IOException {
		writeS32(outputStream, value, bigEndian);
	}
	
	public static int readU32(InputStream inputStream, boolean bigEndian) throws IOException {
		return readS32(inputStream, bigEndian);
	}
	
	public static void writeS32(OutputStream outputStream, int value, boolean bigEndian) throws IOException {
		byte[] dataArray;
		if (bigEndian) {
			byte[] bigEndianDataArray = { 
					(byte)((value & BYTE_0_MASK) >> BYTE_0_SHIFT),
					(byte)((value & BYTE_1_MASK) >> BYTE_1_SHIFT),
					(byte)((value & BYTE_2_MASK) >> BYTE_2_SHIFT),
					(byte)((value & BYTE_3_MASK) >> BYTE_3_SHIFT),
			};
			dataArray = bigEndianDataArray;
		}
		else {
			byte[] littleEndianDataArray = { 
					(byte)((value & BYTE_3_MASK) >> BYTE_3_SHIFT),
					(byte)((value & BYTE_2_MASK) >> BYTE_2_SHIFT),
					(byte)((value & BYTE_1_MASK) >> BYTE_1_SHIFT),
					(byte)((value & BYTE_0_MASK) >> BYTE_0_SHIFT),
			};
			dataArray = littleEndianDataArray;
		}
		outputStream.write(dataArray);
	}
	
	public static int readS32(InputStream inputStream, boolean bigEndian) throws IOException {
		byte[] dataArray = new byte[4];
		inputStream.read(dataArray);
		int value;
		if (bigEndian) {
			value = (
					((dataArray[0] << BYTE_0_SHIFT) & BYTE_0_MASK) | 
					((dataArray[1] << BYTE_1_SHIFT) & BYTE_1_MASK) |
					((dataArray[2] << BYTE_2_SHIFT) & BYTE_2_MASK) |
					((dataArray[3] << BYTE_3_SHIFT) & BYTE_3_MASK)
					);
		}
		else {
			value = (
					((dataArray[0] << BYTE_3_SHIFT) & BYTE_3_MASK) | 
					((dataArray[1] << BYTE_2_SHIFT) & BYTE_2_MASK) |
					((dataArray[2] << BYTE_1_SHIFT) & BYTE_1_MASK) |
					((dataArray[3] << BYTE_0_SHIFT) & BYTE_0_MASK)
					);
		}
		return value;
	}
	
	public static void writeF32(OutputStream outputStream, float value, boolean bigEndian) throws IOException  {
		int intValue = Float.floatToIntBits(value);
		writeS32(outputStream, intValue, bigEndian);
	}
	
	public static void writePadding(OutputStream outputStream, int size) throws IOException {
		byte[] paddingArray = new byte[size];
		outputStream.write(paddingArray);
	}
	
	public static void writeBinaryHeader(OutputStream outputStream, BinaryHeader binaryHeader, boolean bigEndian) throws IOException {
		writeCharArray(outputStream, binaryHeader.groupCode);
		writeCharArray(outputStream, binaryHeader.typeCode);
		writeBool(outputStream, binaryHeader.bigEndian);
		writePadding(outputStream, 1);
		writeU16(outputStream, binaryHeader.version, bigEndian);
		writeU32(outputStream, binaryHeader.dataSize, bigEndian);
	}
}
