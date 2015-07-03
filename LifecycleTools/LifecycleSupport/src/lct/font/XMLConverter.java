package lct.font;

import lct.util.PathUtility;
import lct.util.XMLUtility;

import java.util.Vector;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

public class XMLConverter {
	public XMLConverter() {		
	}
	
	public void storeSheet(Sheet sheet, String filePath) {
		try {
			Document document = XMLUtility.createDocument();
			
			Element sheetElement = document.createElement("font:sheet");
			{
				XMLUtility.writeStringElement(document, "textureName", sheet.textureName, sheetElement);
				XMLUtility.writeFloatElement(document, "cellWidth", sheet.cellWidth, sheetElement);
				XMLUtility.writeFloatElement(document, "cellHeight", sheet.cellHeight, sheetElement);
				XMLUtility.writeFloatElement(document, "cellBaselineX", sheet.cellBaselineX, sheetElement);
				XMLUtility.writeFloatElement(document, "cellBaselineY", sheet.cellBaselineY, sheetElement);
				XMLUtility.writeFloatElement(document, "ascent", sheet.ascent, sheetElement);
				XMLUtility.writeFloatElement(document, "descent", sheet.descent, sheetElement);
				XMLUtility.writeFloatElement(document, "lineHeight", sheet.lineHeight, sheetElement);
				XMLUtility.writeIntegerElement(document, "horizontalCellCount", sheet.horizontalCellCount, sheetElement);
				XMLUtility.writeIntegerElement(document, "verticalCellCount", sheet.verticalCellCount, sheetElement);
				
				Element glyphsElement = document.createElement("glyphs");
				for (Sheet.Glyph glyph : sheet.glyphVector) {
					Element glyphElement = document.createElement("glyph");
					{
						XMLUtility.writeStringElement(document, "character", Character.toString(glyph.character), glyphElement);
						XMLUtility.writeFloatElement(document, "cellX", glyph.cellX, glyphElement);
						XMLUtility.writeFloatElement(document, "cellY", glyph.cellY, glyphElement);
						XMLUtility.writeFloatElement(document, "width", glyph.width, glyphElement);
						XMLUtility.writeFloatElement(document, "leftBearing", glyph.leftBearing, glyphElement);
						XMLUtility.writeFloatElement(document, "rightBearing", glyph.rightBearing, glyphElement);
						XMLUtility.writeFloatElement(document, "advance", glyph.advance, glyphElement);
					}
					glyphsElement.appendChild(glyphElement);
				}
				sheetElement.appendChild(glyphsElement);
			}
			document.appendChild(sheetElement);
			
			XMLUtility.storeDocument(document, filePath);
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			System.out.println(e.getStackTrace());
		}
	}
	
	public Sheet loadSheet(String filePath) {
		try {
			Document document = XMLUtility.loadDocument(filePath);
			
			Element sheetElement = (Element)document.getElementsByTagName("font:sheet").item(0);
			Sheet sheet = new Sheet();
			sheet.name = PathUtility.getFileNameNoExtension(filePath);
			{
				sheet.textureName = XMLUtility.readStringElement(sheetElement, "textureName", 0);
				sheet.cellWidth = XMLUtility.readFloatElement(sheetElement, "cellWidth", 0);
				sheet.cellHeight = XMLUtility.readFloatElement(sheetElement, "cellHeight", 0);
				sheet.cellBaselineX = XMLUtility.readFloatElement(sheetElement, "cellBaselineX", 0);
				sheet.cellBaselineY = XMLUtility.readFloatElement(sheetElement, "cellBaselineY", 0);
				sheet.ascent = XMLUtility.readFloatElement(sheetElement, "ascent", 0);
				sheet.descent = XMLUtility.readFloatElement(sheetElement, "descent", 0);
				sheet.lineHeight = XMLUtility.readFloatElement(sheetElement, "lineHeight", 0);
				sheet.horizontalCellCount = XMLUtility.readIntegerElement(sheetElement, "horizontalCellCount", 0);
				sheet.verticalCellCount = XMLUtility.readIntegerElement(sheetElement, "verticalCellCount", 0);
				
				Element glyphsElement = (Element)sheetElement.getElementsByTagName("glyphs").item(0);
				NodeList glyphNodeList = glyphsElement.getElementsByTagName("glyph");
				sheet.glyphVector = new Vector<Sheet.Glyph>();
				for (int glyphIndex = 0; glyphIndex < glyphNodeList.getLength(); ++glyphIndex) {
					Element glyphElement = (Element)glyphNodeList.item(glyphIndex);
					Sheet.Glyph glyph = new Sheet.Glyph();
					{
						glyph.character = XMLUtility.readStringElement(glyphElement, "character", 0).charAt(0);
						glyph.cellX = XMLUtility.readFloatElement(glyphElement, "cellX", 0);
						glyph.cellY = XMLUtility.readFloatElement(glyphElement, "cellY", 0);
						glyph.width = XMLUtility.readFloatElement(glyphElement, "width", 0);
						glyph.leftBearing = XMLUtility.readFloatElement(glyphElement, "leftBearing", 0);
						glyph.rightBearing = XMLUtility.readFloatElement(glyphElement, "rightBearing", 0);
						glyph.advance = XMLUtility.readFloatElement(glyphElement, "advance", 0);												
					}
					sheet.glyphVector.add(glyph);
				}
			}
			return sheet;
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			System.out.println(e.getStackTrace());
			
			return null;
		}
	}
}
