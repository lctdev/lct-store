package lct.pack;

import lct.util.DataException;

public interface AssetProcessor {
	String getGroupCode();
	void processAsset(String typeCode, String inputFilePath, String outputFilePath, boolean bigEndian) throws DataException;
}
