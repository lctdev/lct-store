package lct.imag;

import java.util.Vector;

import lct.util.DataException;

public class AssetProcessor implements lct.pack.AssetProcessor {
	ImageConverter m_imageConverter = new ImageConverter();
	XMLConverter m_xmlConverter = new XMLConverter();
	BinaryConverter m_binaryConverter = new BinaryConverter();
	
	public AssetProcessor() {
	}
	
	public String getGroupCode() {
		return Constants.GROUP_CODE;
	}
	
	public Vector<String> getDependencies(String typeCode, String inputFilePath) {
		switch (typeCode) {
		case Constants.TEXTURE_ATLAS_TYPE_CODE:
		{
			return m_xmlConverter.readTextureAtlasDependencies(inputFilePath);
		}
		
		default:
		{
			return new Vector<String>();
		}
		}		
	}
	
	public void processAsset(String typeCode, String inputFilePath, String outputFilePath, boolean bigEndian) throws DataException {
		switch (typeCode) {
		case Constants.TEXTURE_TYPE_CODE:
		{
			Texture texture = m_imageConverter.loadTextureTGA(inputFilePath);
			if (texture == null) {
				throw new DataException("Error loading texture file");
			}
			
			m_binaryConverter.storeTexture(texture, outputFilePath, bigEndian);
		}
		break;
		
		case Constants.TEXTURE_TABLE_TYPE_CODE:
		{
			Texture texture = m_imageConverter.loadTextureTGA(inputFilePath);
			if (texture == null) {
				throw new DataException("Error loading texture file");
			}
			Vector<Texture> textureVector = TextureUtility.splitStrip(texture, texture.height, texture.height);
			TextureTable textureTable = new TextureTable();
			textureTable.name = texture.name;
			textureTable.width = textureVector.elementAt(0).width;
			textureTable.height = textureVector.elementAt(0).height;
			textureTable.texelArrayArray = new Texture.TexelRGBA[textureVector.size()][];
			for (int textureIndex = 0; textureIndex < textureVector.size(); ++textureIndex) {
				textureTable.texelArrayArray[textureIndex] = textureVector.elementAt(textureIndex).texelArray;
			}
			
			m_binaryConverter.storeTextureTable(textureTable, outputFilePath, bigEndian);
		}
		break;
		
		case Constants.TEXTURE_ATLAS_TYPE_CODE:
		{
			TextureAtlas textureAtlas = m_xmlConverter.loadTextureAtlas(inputFilePath, m_imageConverter);
			if (textureAtlas == null) {
				throw new DataException("Error loading texture atlas file");
			}
			
			m_binaryConverter.storeTextureAtlas(textureAtlas, outputFilePath, bigEndian);
		}
		break;
		
		default:
		{
			throw new DataException("Unhandled type code: " + typeCode);
		}
		}
	}
}