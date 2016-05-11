package lct.imag;

import java.awt.image.BufferedImage;
import java.awt.image.DataBufferInt;
import java.awt.image.Raster;
import java.util.Vector;

public class TextureUtility {
	public static Texture createEmptyTexture(int width, int height) {
		int texelCount = width * height;
		Texture texture = new Texture();
		texture.width = width;
		texture.height = height;
		texture.texelArray = new Texture.TexelRGBA[texelCount];
		for (int texelIndex = 0; texelIndex < texelCount; ++texelIndex) {
			Texture.TexelRGBA texel = new Texture.TexelRGBA();
			texel.r = 0;
			texel.g = 0;
			texel.b = 0;
			texel.a = 0;
			texture.texelArray[texelIndex] = texel;
		}
		return texture;
	}
	
	public static void copyTexture(Texture destinationTexture, int destinationX, int destinationY, Texture sourceTexture) {
		for (int currSourceY = 0; currSourceY < sourceTexture.height; ++currSourceY) {
			for (int currSourceX = 0; currSourceX < sourceTexture.width; ++currSourceX) {
				int currDestinationX = destinationX + currSourceX;
				int currDestinationY = destinationY + currSourceY;
				int destinationTexelIndex = (currDestinationY * destinationTexture.width) + currDestinationX;
				int sourceTexelIndex = (currSourceY * sourceTexture.width) + currSourceX;
				Texture.TexelRGBA destinationTexel = (Texture.TexelRGBA)destinationTexture.texelArray[destinationTexelIndex];
				Texture.TexelRGBA sourceTexel = (Texture.TexelRGBA)sourceTexture.texelArray[sourceTexelIndex];
				destinationTexel.r = sourceTexel.r;
				destinationTexel.g = sourceTexel.g;
				destinationTexel.b = sourceTexel.b;
				destinationTexel.a = sourceTexel.a;
			}
		}
	}
	
	public static Vector<Texture> splitStrip(Texture stripTexture, int cellWidth, int cellHeight) {
		int cellCount = stripTexture.width / cellWidth;
		Vector<Texture> cellTextureVector = new Vector<Texture>();
		for (int cellIndex = 0; cellIndex < cellCount; ++cellIndex) {
			Texture cellTexture = new Texture();
			cellTexture.width = cellWidth;
			cellTexture.height = cellHeight;
			int cellTexelCount = cellWidth * cellHeight;
			cellTexture.texelArray = new Texture.TexelRGBA[cellTexelCount];
			int baseX = cellIndex * cellWidth;
			int baseY = 0;
			for (int y = 0; y < cellHeight; ++y) {
				for (int x = 0; x < cellWidth; ++x) {
					int sourceTexelIndex = ((baseY + y) * stripTexture.width) + (baseX + x);
					int destinationTexelIndex = (y * cellWidth) + x;
					Texture.TexelRGBA sourceTexel = (Texture.TexelRGBA)stripTexture.texelArray[sourceTexelIndex];
					Texture.TexelRGBA destinationTexel = new Texture.TexelRGBA();
					destinationTexel.r = sourceTexel.r;
					destinationTexel.g = sourceTexel.g;
					destinationTexel.b = sourceTexel.b;
					destinationTexel.a = sourceTexel.a;
					cellTexture.texelArray[destinationTexelIndex] = destinationTexel;
				}
			}
			cellTextureVector.add(cellTexture);
		}
		return cellTextureVector;
	}
	
	public static Texture mergeStrip(Vector<Texture> cellTextureVector, int cellWidth, int cellHeight) {
		int cellCount = cellTextureVector.size();
		int stripWidth = cellCount * cellWidth;
		int stripHeight = cellHeight;
		int stripTexelCount = stripWidth * stripHeight;
		Texture stripTexture = new Texture();
		stripTexture.width = stripWidth;
		stripTexture.height = stripHeight;
		stripTexture.texelArray = new Texture.TexelRGBA[stripTexelCount];
		for (int cellIndex = 0; cellIndex < cellCount; ++cellIndex) {
			Texture cellTexture = cellTextureVector.elementAt(cellIndex);
			int baseX = cellIndex * cellWidth;
			int baseY = 0;
			for (int y = 0; y < cellHeight; ++y) {
				for (int x = 0; x < cellWidth; ++x) {
					int sourceTexelIndex = (y * cellWidth) + x;
					int destinationTexelIndex = ((baseY + y) * stripTexture.width) + (baseX + x);					
					Texture.TexelRGBA sourceTexel = (Texture.TexelRGBA)cellTexture.texelArray[sourceTexelIndex];
					Texture.TexelRGBA destinationTexel = new Texture.TexelRGBA();
					destinationTexel.r = sourceTexel.r;
					destinationTexel.g = sourceTexel.g;
					destinationTexel.b = sourceTexel.b;
					destinationTexel.a = sourceTexel.a;
					stripTexture.texelArray[destinationTexelIndex] = destinationTexel;
				}
			}
		}
		return stripTexture;
	}
	
	public static Texture fromBufferedImage(BufferedImage bufferedImage) {
		Raster raster = bufferedImage.getData();
		DataBufferInt dataBuffer = (DataBufferInt)raster.getDataBuffer();
		int[] pixelArray = dataBuffer.getData();
		
		Texture texture = new Texture();
		int textureWidth = bufferedImage.getWidth();
		int textureHeight = bufferedImage.getHeight();
		int texelCount = textureWidth * textureHeight;
		Texture.TexelRGBA[] texelArray = new Texture.TexelRGBA[texelCount];
		
		for (int texelY = 0; texelY < textureHeight; ++texelY) {
			for (int texelX = 0; texelX < textureWidth; ++texelX) {
				int texelIndex = texelY * textureWidth + texelX;
				int pixelY = textureHeight - texelY - 1;
				int pixelIndex = pixelY * textureWidth + texelX;
									
				Texture.TexelRGBA texel = new Texture.TexelRGBA();
				int pixel = pixelArray[pixelIndex];
				texel.a = ((pixel & 0xFF000000) >> 24) & 0xFF;
				texel.r = ((pixel & 0x00FF0000) >> 16) & 0xFF;
				texel.g = ((pixel & 0x0000FF00) >> 8) & 0xFF;
				texel.b = ((pixel & 0x000000FF) >> 0) & 0xFF;
				
				texelArray[texelIndex] = texel;
			}
		}
		
		texture.width = textureWidth;
		texture.height = textureHeight;
		texture.texelArray = texelArray;
		
		return texture;
	}
}
