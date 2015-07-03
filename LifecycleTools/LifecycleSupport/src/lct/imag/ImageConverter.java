package lct.imag;

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

public class ImageConverter {
	private static final int IMAGE_TYPE_TRUE_COLOR = 2;
	private static final int IMAGE_TYPE_RLE_TRUE_COLOR = 10;
	
	private static final int DESCRIPTOR_NO_ALPHA = 0;
	private static final int DESCRIPTOR_FULL_ALPHA = 8;
	
	static class TGAHeader {
		static class ColorMapSpecification {
			int tableOffset;	// u16
			int entryCount;		// u16
			int bitsPerPixel;	// u8
		}
		
		static class ImageSpecification {
			int xOrigin;		// u16
			int yOrigin;		// u16
			int width;			// u16
			int height;			// u16
			int bitsPerPixel;	// u8
			int descriptor;		// u8
		}
		
		int idLength;			// u8
		int colorMapType;		// u8
		int imageType;			// u8
		ColorMapSpecification colorMapSpecification;
		ImageSpecification imageSpecification;
	}
	
	public Texture loadTextureTGA(String filePath) {
		try {
			BufferedInputStream bufferedInputStream = new BufferedInputStream(new FileInputStream(filePath));
			
			TGAHeader tgaHeader = readTGAHeader(bufferedInputStream);
			
			if (tgaHeader.idLength > 0) {
				bufferedInputStream.skip(tgaHeader.idLength);
			}
			
			if ((tgaHeader.imageType == IMAGE_TYPE_TRUE_COLOR)) {
				if ((tgaHeader.imageSpecification.bitsPerPixel == 32) && (tgaHeader.imageSpecification.descriptor == DESCRIPTOR_FULL_ALPHA)) {
					Texture texture = new Texture();
					texture.name = PathUtility.getFileNameNoExtension(filePath);
					texture.width = tgaHeader.imageSpecification.width;
					texture.height = tgaHeader.imageSpecification.height;
					
					int pixelCount = tgaHeader.imageSpecification.width * tgaHeader.imageSpecification.height;
					texture.texelArray = readImageDataBGRA(pixelCount, bufferedInputStream);
					
					bufferedInputStream.close();
					
					return texture;
				}
				else if (tgaHeader.imageSpecification.descriptor == DESCRIPTOR_NO_ALPHA) {
					throw new DataException("Unsupported TGA Format(No Alpha)");
				}
				else {
					throw new DataException("Unsupported TGA Format(Unknown)");
				}
			}
			else if (tgaHeader.imageType == IMAGE_TYPE_RLE_TRUE_COLOR) {
				throw new DataException("Unsupported TGA Format(RLE True Color)");
			}
			else {
				throw new DataException("Unsupported TGA Format(Unknown)");
			}
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
			return null;
		}
	}
	
	public void storeTextureTGA(Texture texture, String filePath) {
		try {
			BufferedOutputStream bufferedOutputStream = new BufferedOutputStream(new FileOutputStream(filePath));
			
			TGAHeader tgaHeader = new TGAHeader();
			tgaHeader.idLength = 0;
			tgaHeader.colorMapType = 0;
			tgaHeader.imageType = IMAGE_TYPE_TRUE_COLOR;
			
			tgaHeader.colorMapSpecification = new TGAHeader.ColorMapSpecification();
			tgaHeader.colorMapSpecification.tableOffset = 0;
			tgaHeader.colorMapSpecification.entryCount = 0;
			tgaHeader.colorMapSpecification.bitsPerPixel = 0;
			
			tgaHeader.imageSpecification = new TGAHeader.ImageSpecification();
			tgaHeader.imageSpecification.xOrigin = 0;
			tgaHeader.imageSpecification.yOrigin = 0;
			tgaHeader.imageSpecification.width = texture.width;
			tgaHeader.imageSpecification.height = texture.height;
			tgaHeader.imageSpecification.bitsPerPixel = 32;
			tgaHeader.imageSpecification.descriptor = 8;
			
			writeTGAHeader(tgaHeader, bufferedOutputStream);
				
			writeImageDataBGRA((Texture.TexelRGBA[])texture.texelArray, bufferedOutputStream);
			
			bufferedOutputStream.close();
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
	}
	
	public TGAHeader readTGAHeader(InputStream inputStream) throws IOException {
		TGAHeader tgaHeader = new TGAHeader();
		tgaHeader.idLength = BinaryUtility.readU8(inputStream);
		tgaHeader.colorMapType = BinaryUtility.readU8(inputStream);
		tgaHeader.imageType = BinaryUtility.readU8(inputStream);
		
		tgaHeader.colorMapSpecification = new TGAHeader.ColorMapSpecification();
		tgaHeader.colorMapSpecification.tableOffset = BinaryUtility.readU16(inputStream, false);
		tgaHeader.colorMapSpecification.entryCount = BinaryUtility.readU16(inputStream, false);
		tgaHeader.colorMapSpecification.bitsPerPixel = BinaryUtility.readU8(inputStream);
		
		tgaHeader.imageSpecification = new TGAHeader.ImageSpecification();
		tgaHeader.imageSpecification.xOrigin = BinaryUtility.readU16(inputStream, false);
		tgaHeader.imageSpecification.yOrigin = BinaryUtility.readU16(inputStream, false);
		tgaHeader.imageSpecification.width = BinaryUtility.readU16(inputStream, false);
		tgaHeader.imageSpecification.height = BinaryUtility.readU16(inputStream, false);
		tgaHeader.imageSpecification.bitsPerPixel = BinaryUtility.readU8(inputStream);
		tgaHeader.imageSpecification.descriptor = BinaryUtility.readU8(inputStream);
		
		return tgaHeader;
	}
	
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
}
