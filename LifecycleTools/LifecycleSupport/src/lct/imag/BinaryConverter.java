package lct.imag;

import java.io.ByteArrayOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

import lct.util.BinaryHeader;
import lct.util.BinaryUtility;

public class BinaryConverter {
	private static final int MAX_NAME_SIZE = 32;
	
	private static final int VERSION = 0;
	
	public void storeTexture(Texture texture, String filePath, boolean bigEndian) {
		try {
			FileOutputStream fileOutputStream = new FileOutputStream(filePath);
			
			ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
			writeTextureData(texture, byteArrayOutputStream, bigEndian);
			byte[] dataArray = byteArrayOutputStream.toByteArray();			
			
			BinaryHeader binaryHeader = new BinaryHeader();
			binaryHeader.groupCode = Constants.GROUP_CODE.toCharArray();
			binaryHeader.typeCode = Constants.TEXTURE_TYPE_CODE.toCharArray();
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
	
	private void writeTextureData(Texture texture, OutputStream outputStream, boolean bigEndian) throws IOException {
		BinaryUtility.writeStringBuffer(outputStream, texture.name, MAX_NAME_SIZE);
		BinaryUtility.writeU32(outputStream, texture.width, bigEndian);
		BinaryUtility.writeU32(outputStream, texture.height, bigEndian);
		int size = texture.width * texture.height * 4;
		BinaryUtility.writeU32(outputStream, size, bigEndian);

		Texture.TexelRGBA[] texelArray = (Texture.TexelRGBA[])texture.texelArray;
		for (int texelIndex = 0; texelIndex < texelArray.length; ++texelIndex) {
			Texture.TexelRGBA texel = texelArray[texelIndex];
			BinaryUtility.writeU8(outputStream, texel.r);
			BinaryUtility.writeU8(outputStream, texel.g);
			BinaryUtility.writeU8(outputStream, texel.b);
			BinaryUtility.writeU8(outputStream, texel.a);
		}
	}
	
	public void storeTextureTable(TextureTable textureTable, String filePath, boolean bigEndian) {
		try {
			FileOutputStream fileOutputStream = new FileOutputStream(filePath);
			
			ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
			writeTextureTableData(textureTable, byteArrayOutputStream, bigEndian);
			byte[] dataArray = byteArrayOutputStream.toByteArray();			
			
			BinaryHeader binaryHeader = new BinaryHeader();
			binaryHeader.groupCode = Constants.GROUP_CODE.toCharArray();
			binaryHeader.typeCode = Constants.TEXTURE_TABLE_TYPE_CODE.toCharArray();
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
	
	private void writeTextureTableData(TextureTable textureTable, OutputStream outputStream, boolean bigEndian) throws IOException {
		BinaryUtility.writeStringBuffer(outputStream, textureTable.name, MAX_NAME_SIZE);
		BinaryUtility.writeU32(outputStream, textureTable.width, bigEndian);
		BinaryUtility.writeU32(outputStream, textureTable.height, bigEndian);
		int size = textureTable.width * textureTable.height * 4 * textureTable.texelArrayArray.length;
		BinaryUtility.writeU32(outputStream, size, bigEndian);
		BinaryUtility.writeU32(outputStream, textureTable.texelArrayArray.length, bigEndian);
		
		for (int textureIndex = 0; textureIndex < textureTable.texelArrayArray.length; ++textureIndex) {
			Texture.TexelRGBA[] texelArray = (Texture.TexelRGBA[])textureTable.texelArrayArray[textureIndex];
			for (int texelIndex = 0; texelIndex < texelArray.length; ++texelIndex) {
				Texture.TexelRGBA texel = texelArray[texelIndex];
				BinaryUtility.writeU8(outputStream, texel.r);
				BinaryUtility.writeU8(outputStream, texel.g);
				BinaryUtility.writeU8(outputStream, texel.b);
				BinaryUtility.writeU8(outputStream, texel.a);
			}
		}
	}
	
	public void storeTextureAtlas(TextureAtlas textureAtlas, String filePath, boolean bigEndian) {
		try {
			FileOutputStream fileOutputStream = new FileOutputStream(filePath);
			
			ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
			writeTextureAtlasData(textureAtlas, byteArrayOutputStream, bigEndian);
			byte[] dataArray = byteArrayOutputStream.toByteArray();			
			
			BinaryHeader binaryHeader = new BinaryHeader();
			binaryHeader.groupCode = Constants.GROUP_CODE.toCharArray();
			binaryHeader.typeCode = Constants.TEXTURE_ATLAS_TYPE_CODE.toCharArray();
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
	
	private void writeTextureAtlasData(TextureAtlas textureAtlas, OutputStream outputStream, boolean bigEndian) throws IOException {
		BinaryUtility.writeStringBuffer(outputStream, textureAtlas.name, MAX_NAME_SIZE);
		BinaryUtility.writeU32(outputStream, textureAtlas.width, bigEndian);
		BinaryUtility.writeU32(outputStream, textureAtlas.height, bigEndian);
		Texture mergedTexture = textureAtlas.mergedTexture;
		int size = mergedTexture.width * mergedTexture.height * 4;
		BinaryUtility.writeU32(outputStream, size, bigEndian);
		BinaryUtility.writeU32(outputStream, textureAtlas.regionVector.size(), bigEndian);
		
		Texture.TexelRGBA[] texelArray = (Texture.TexelRGBA[])mergedTexture.texelArray;
		for (int texelIndex = 0; texelIndex < texelArray.length; ++texelIndex) {
			Texture.TexelRGBA texel = texelArray[texelIndex];
			BinaryUtility.writeU8(outputStream, texel.r);
			BinaryUtility.writeU8(outputStream, texel.g);
			BinaryUtility.writeU8(outputStream, texel.b);
			BinaryUtility.writeU8(outputStream, texel.a);
		}				
		
		for (TextureAtlas.Region region : textureAtlas.regionVector) {
			String name = region.texture.name;
			float translateS = (float)region.x / (float)textureAtlas.width;
			float translateT = (float)region.y / (float)textureAtlas.height;
			float scaleS = (float)region.texture.width / (float)textureAtlas.width;
			float scaleT = (float)region.texture.height / (float)textureAtlas.height;
			BinaryUtility.writeStringBuffer(outputStream, name, MAX_NAME_SIZE);
			BinaryUtility.writeF32(outputStream, translateS, bigEndian);
			BinaryUtility.writeF32(outputStream, translateT, bigEndian);
			BinaryUtility.writeF32(outputStream, scaleS, bigEndian);
			BinaryUtility.writeF32(outputStream, scaleT, bigEndian);
		}		
	}
}
