package lct.font;

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
		case Constants.SHEET_TYPE_CODE:
		{
			Sheet sheet = m_xmlConverter.loadSheet(inputFilePath);
			if (sheet == null) {
				throw new DataException("Error loading sheet file");
			}
			
			m_binaryConverter.storeSheet(sheet, outputFilePath, bigEndian);
		}
		break;
		
		default:
		{
			throw new DataException("Unhandled type code: " + typeCode);
		}
		}
	}
}
