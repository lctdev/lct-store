package lct.pass;

import lct.util.DataException;

public class AssetProcessor implements lct.pack.AssetProcessor {
	PassthroughConverter m_passthroughConverter = new PassthroughConverter();
	BinaryConverter m_binaryConverter = new BinaryConverter();
	
	public AssetProcessor() {
	}
	
	public String getGroupCode() {
		return Constants.GROUP_CODE;
	}
	
	public void processAsset(String typeCode, String inputFilePath, String outputFilePath, boolean bigEndian) throws DataException {
		switch (typeCode) {
		case Constants.STRING_TYPE_CODE:
		{
			StringAsset stringAsset = m_passthroughConverter.loadString(inputFilePath);
			if (stringAsset == null) {
				throw new DataException("Error loading string file");
			}
			
			m_binaryConverter.storeString(stringAsset, outputFilePath, bigEndian);
		}
		break;
		
		default:
		{
			throw new DataException("Unhandled type code: " + typeCode);
		}
		}
	}
}