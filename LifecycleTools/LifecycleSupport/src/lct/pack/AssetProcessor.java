package lct.pack;

import java.util.Vector;

import lct.util.DataException;

public interface AssetProcessor {
	String getGroupCode();
	Vector<String> getDependencies(String typeCode, String inputFilePath);
	void processAsset(String typeCode, String inputFilePath, String outputFilePath, boolean bigEndian) throws DataException;
}
