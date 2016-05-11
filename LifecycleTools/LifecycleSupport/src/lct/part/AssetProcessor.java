package lct.part;

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
		case Constants.FIELD_TYPE_CODE:
		{
			Field field = m_xmlConverter.loadField(inputFilePath);
			if (field == null) {
				throw new DataException("Error loading field file");
			}
			
			m_binaryConverter.storeField(field, outputFilePath, bigEndian);
		}
		break;
		
		default:
		{
			throw new DataException("Unhandled type code: " + typeCode);
		}
		}
	}
}
