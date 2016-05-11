package lct.spri;

import java.util.Vector;

import lct.util.DataException;

public class AssetProcessor implements lct.pack.AssetProcessor {
	XMLConverter m_xmlConverter = new XMLConverter();
	BinaryConverter m_binaryConverter = new BinaryConverter();
	
	public AssetProcessor() {
	}
	
	public String getGroupCode() {
		return Constants.GROUP_CODE;
	}
	
	public Vector<String> getDependencies(String typeCode, String inputFilePath) {
		return new Vector<String>();
	}
	
	public void processAsset(String typeCode, String inputFilePath, String outputFilePath, boolean bigEndian) throws DataException {
		switch (typeCode) {
		case Constants.FIGURE_TYPE_CODE:
		{
			Figure figure = m_xmlConverter.loadFigure(inputFilePath);
			if (figure == null) {
				throw new DataException("Error loading figure file");
			}
			
			m_binaryConverter.storeFigure(figure, outputFilePath, bigEndian);
		}
		break;
		
		case Constants.ANIMATION_TYPE_CODE:
		{
			Animation animation = m_xmlConverter.loadAnimation(inputFilePath);
			if (animation == null) {
				throw new DataException("Error loading animation file");
			}			
			
			m_binaryConverter.storeAnimation(animation, outputFilePath, bigEndian);
		}
		break;
		
		default:
		{
			throw new DataException("Unhandled type code: " + typeCode);
		}
		}
	}
}
