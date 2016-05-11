package lct.imag;

import lct.util.PathUtility;
import lct.util.XMLUtility;

import java.util.Vector;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

public class XMLConverter {
	public XMLConverter() {		
	}
	
	public void storeTextureAtlas(TextureAtlas textureAtlas, String filePath, ImageConverter imageConverter) {
		try {
			Document document = XMLUtility.createDocument();
			
			Element textureAtlasElement = document.createElement("imag:textureAtlas");
			{
				XMLUtility.writeIntegerElement(document, "width", textureAtlas.width, textureAtlasElement);
				XMLUtility.writeIntegerElement(document, "height", textureAtlas.height, textureAtlasElement);
				
				Element regionsElement = document.createElement("regions");
				for (TextureAtlas.Region region : textureAtlas.regionVector) {
					Element regionElement = document.createElement("region");
					{
						String relativeFilePath = region.texture.name + ".tga";
						XMLUtility.writeStringElement(document, "filePath", relativeFilePath, regionElement);
						
						XMLUtility.writeIntegerElement(document, "x", region.x, regionElement);
						XMLUtility.writeIntegerElement(document, "y", region.y, regionElement);
					}
					regionsElement.appendChild(regionElement);
				}
				textureAtlasElement.appendChild(regionsElement);
			}
			document.appendChild(textureAtlasElement);
			
			XMLUtility.storeDocument(document, filePath);
			
			
			String directoryPath = PathUtility.getDirectoryFull(filePath);
			for (TextureAtlas.Region region : textureAtlas.regionVector) {
				String textureFileName = region.texture.name + ".tga";
				String textureFilePath = PathUtility.combineFull(directoryPath, textureFileName);
				imageConverter.storeTextureTGA(region.texture, textureFilePath);
			}
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			System.out.println(e.getStackTrace());
		}
	}
	
	public TextureAtlas loadTextureAtlas(String filePath, ImageConverter imageConverter) {
		try {
			Document document = XMLUtility.loadDocument(filePath);
			
			String directoryPath = PathUtility.getDirectoryFull(filePath);
			Vector<String> textureFilePathVector = new Vector<String>();
			
			Element textureAtlasElement = (Element)document.getElementsByTagName("imag:textureAtlas").item(0);
			TextureAtlas textureAtlas = new TextureAtlas();
			textureAtlas.name = PathUtility.getFileNameNoExtension(filePath);
			{
				textureAtlas.width = XMLUtility.readIntegerElement(textureAtlasElement, "width", 0);
				textureAtlas.height = XMLUtility.readIntegerElement(textureAtlasElement, "height", 0);
				
				Element regionsElement = (Element)textureAtlasElement.getElementsByTagName("regions").item(0);
				NodeList regionNodeList = regionsElement.getElementsByTagName("region");
				textureAtlas.regionVector = new Vector<TextureAtlas.Region>();
				for (int regionIndex = 0; regionIndex < regionNodeList.getLength(); ++regionIndex) {
					Element regionElement = (Element)regionNodeList.item(regionIndex);
					TextureAtlas.Region region = new TextureAtlas.Region();
					{
						String relativeFilePath = XMLUtility.readStringElement(regionElement, "filePath", 0);
						String textureFilePath = PathUtility.combineFull(directoryPath, relativeFilePath);
						textureFilePathVector.add(textureFilePath);
						
						region.x = XMLUtility.readIntegerElement(regionElement, "x", 0);
						region.y = XMLUtility.readIntegerElement(regionElement, "y", 0);
					}
					textureAtlas.regionVector.add(region);
				}								
			}
			
			Texture mergedTexture = TextureUtility.createEmptyTexture(textureAtlas.width, textureAtlas.height);
			for (int regionIndex = 0; regionIndex < textureAtlas.regionVector.size(); ++regionIndex) {
				String textureFilePath = textureFilePathVector.get(regionIndex);
				Texture texture = imageConverter.loadTextureTGA(textureFilePath);
				if (texture == null) {
					return null;
				}
				
				TextureAtlas.Region region = textureAtlas.regionVector.get(regionIndex);
				region.texture = texture;
				
				TextureUtility.copyTexture(mergedTexture, region.x, region.y, texture);
			}
			textureAtlas.mergedTexture = mergedTexture;
			
			return textureAtlas;
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			System.out.println(e.getStackTrace());
			
			return null;
		}
	}
	
	public Vector<String> readTextureAtlasDependencies(String filePath) {
		try {
			Document document = XMLUtility.loadDocument(filePath);
			
			String directoryPath = PathUtility.getDirectoryFull(filePath);
			Vector<String> dependencyVector = new Vector<String>();
			
			Element textureAtlasElement = (Element)document.getElementsByTagName("imag:textureAtlas").item(0);
			{
				Element regionsElement = (Element)textureAtlasElement.getElementsByTagName("regions").item(0);
				NodeList regionNodeList = regionsElement.getElementsByTagName("region");
				for (int regionIndex = 0; regionIndex < regionNodeList.getLength(); ++regionIndex) {
					Element regionElement = (Element)regionNodeList.item(regionIndex);
					TextureAtlas.Region region = new TextureAtlas.Region();
					{
						String relativeFilePath = XMLUtility.readStringElement(regionElement, "filePath", 0);
						String textureFilePath = PathUtility.combineFull(directoryPath, relativeFilePath);
						dependencyVector.add(textureFilePath);
					}
				}
			}
			
			return dependencyVector;
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			System.out.println(e.getStackTrace());
			
			return null;
		}
	}
}
