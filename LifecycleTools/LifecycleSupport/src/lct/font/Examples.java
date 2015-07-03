package lct.font;

import java.awt.Color;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.font.FontRenderContext;
import java.awt.font.GlyphMetrics;
import java.awt.font.GlyphVector;
import java.awt.image.BufferedImage;
import java.util.Vector;

import lct.imag.Texture;
import lct.imag.TextureUtility;
import lct.util.PathUtility;
import lct.pack.Manifest;

public class Examples {
	
	static final String SHEET_NAME = "example_sheet";
	static final String SHEET_TEXTURE_NAME = "example_sheet_texture";
	static final String GRID_TEXTURE_NAME = "example_grid_texture";
	static final String MANIFEST_NAME = "example_manifest";
	
	static final int HORIZONTAL_CELL_COUNT = 8;
	static final int VERTICAL_CELL_COUNT = 16;
	static final char BEGIN_CHARACTER = ' ';
	static final char END_CHARACTER = '~';
	
	static char[] s_characterArray;
	
	static {
		int characterCount = END_CHARACTER - BEGIN_CHARACTER + 1;
		s_characterArray = new char[characterCount];
		char character = BEGIN_CHARACTER;
		for (int characterIndex = 0; characterIndex < characterCount; ++characterIndex) {
			s_characterArray[characterIndex] = character;
			++character;
		}
	}
	
	public static Sheet createSheet(int cellWidth, int cellHeight, Font font) {
		BufferedImage bufferedImage = new BufferedImage(1, 1, BufferedImage.TYPE_INT_ARGB);
		Graphics graphics = bufferedImage.getGraphics();
		graphics.setFont(font);
		
		Graphics2D graphics2D = (Graphics2D)graphics;
		FontMetrics fontMetrics = graphics2D.getFontMetrics();
		FontRenderContext fontRenderContext = graphics2D.getFontRenderContext();
		GlyphVector glyphVector = font.createGlyphVector(fontRenderContext, s_characterArray);
		
		int cellBaselineX = cellWidth / 4;
		int cellBaselineY = cellHeight / 4;
		
		Sheet sheet = new Sheet();
		sheet.name = SHEET_NAME;
		{
			sheet.textureName = SHEET_TEXTURE_NAME;
			sheet.cellWidth = (float)cellWidth;
			sheet.cellHeight = (float)cellHeight;
			sheet.cellBaselineX = (float)cellBaselineX;
			sheet.cellBaselineY = (float)cellBaselineY;
			sheet.ascent = (float)fontMetrics.getAscent();
			sheet.descent = (float)fontMetrics.getDescent();
			sheet.lineHeight = (float)fontMetrics.getHeight();			
			sheet.horizontalCellCount = HORIZONTAL_CELL_COUNT;
			sheet.verticalCellCount = VERTICAL_CELL_COUNT;
			
			fontMetrics.getHeight();
			
			sheet.glyphVector = new Vector<Sheet.Glyph>();
			for (int characterIndex = 0; characterIndex < s_characterArray.length; ++characterIndex) {
				GlyphMetrics glyphMetrics = glyphVector.getGlyphMetrics(characterIndex);
				
				Sheet.Glyph glyph = new Sheet.Glyph();
				glyph.character = s_characterArray[characterIndex];
				glyph.cellX = characterIndex % HORIZONTAL_CELL_COUNT;
				glyph.cellY = characterIndex / HORIZONTAL_CELL_COUNT;
				glyph.width = (float)glyphMetrics.getBounds2D().getWidth();
				glyph.leftBearing = glyphMetrics.getLSB();
				glyph.rightBearing = glyphMetrics.getRSB();
				glyph.advance = glyphMetrics.getAdvance();
				sheet.glyphVector.add(glyph);
			}
		}
		
		return sheet;
	}
	
	public static Texture createSheetTexture(int cellWidth, int cellHeight, Font font) {	
		final int TEXTURE_WIDTH = HORIZONTAL_CELL_COUNT * cellWidth;
		final int TEXTURE_HEIGHT = VERTICAL_CELL_COUNT * cellHeight;
		
		BufferedImage bufferedImage = new BufferedImage(TEXTURE_WIDTH, TEXTURE_HEIGHT, BufferedImage.TYPE_INT_ARGB);
		Graphics graphics = bufferedImage.getGraphics();
		
		int cellBaselineX = cellWidth / 4;
		int cellBaselineY = cellHeight / 4;
					
		graphics.setFont(font);
		graphics.setColor(Color.WHITE);
							
		for (int characterIndex = 0; characterIndex < s_characterArray.length; ++characterIndex) {
			int cellX = characterIndex % HORIZONTAL_CELL_COUNT;
			int cellY = characterIndex / HORIZONTAL_CELL_COUNT;
			int drawX = cellX * cellWidth + cellBaselineX;
			int drawY = cellY * cellHeight + cellHeight - cellBaselineY;
			
			graphics.drawChars(s_characterArray, characterIndex, 1, drawX, drawY);
		}
		
		Texture texture = TextureUtility.fromBufferedImage(bufferedImage);
		texture.name = SHEET_TEXTURE_NAME;
		
		return texture;
	}
	
	public static Texture createGridTexture(int cellWidth, int cellHeight) {
		final int TEXTURE_WIDTH = HORIZONTAL_CELL_COUNT * cellWidth;
		final int TEXTURE_HEIGHT = VERTICAL_CELL_COUNT * cellHeight;
		
		BufferedImage bufferedImage = new BufferedImage(TEXTURE_WIDTH, TEXTURE_HEIGHT, BufferedImage.TYPE_INT_ARGB);
		Graphics graphics = bufferedImage.getGraphics();
		
		int cellBaselineX = cellWidth / 4;
		int cellBaselineY = cellHeight / 4;
				
		for (int cellY = 0; cellY < VERTICAL_CELL_COUNT; ++cellY) {
			int cellDrawY = cellY * cellHeight + cellHeight - 1;
			graphics.setColor(Color.GREEN);
			graphics.drawLine(0, cellDrawY, TEXTURE_WIDTH - 1, cellDrawY);
			int baseLineDrawY = cellDrawY - cellBaselineY;
			graphics.setColor(Color.MAGENTA);
			graphics.drawLine(0, baseLineDrawY, TEXTURE_WIDTH - 1, baseLineDrawY);
		}
		for (int cellX = 0; cellX < HORIZONTAL_CELL_COUNT; ++cellX) {
			int cellDrawX = cellX * cellWidth;
			graphics.setColor(Color.GREEN);
			graphics.drawLine(cellDrawX, 0, cellDrawX, TEXTURE_HEIGHT - 1);
			int baseLineDrawX = cellDrawX + cellBaselineX;
			graphics.setColor(Color.MAGENTA);
			graphics.drawLine(baseLineDrawX, 0, baseLineDrawX, TEXTURE_HEIGHT - 1);
		}
		
		Texture texture = TextureUtility.fromBufferedImage(bufferedImage);
		texture.name = GRID_TEXTURE_NAME;
		
		return texture;
	}
	
	public static Manifest createManifest(String directoryPath) {
		Manifest manifest = new Manifest();
		manifest.name = MANIFEST_NAME;
		{
			manifest.assetEntryVector = new Vector<Manifest.AssetEntry>();			
			{
				{
					String fileName = SHEET_NAME + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = Constants.GROUP_CODE;
					assetEntry.typeCode = Constants.SHEET_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);
					manifest.assetEntryVector.add(assetEntry);
				}
				{
					String fileName = SHEET_TEXTURE_NAME + ".tga";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = lct.imag.Constants.GROUP_CODE;
					assetEntry.typeCode = lct.imag.Constants.TEXTURE_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);
					manifest.assetEntryVector.add(assetEntry);
				}
			}
		}
		return manifest;
	}
	
}
