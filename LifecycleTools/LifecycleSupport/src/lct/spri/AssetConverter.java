package lct.spri;

public class AssetConverter {
	private int m_assetProcessorAddr;
	
	public AssetConverter() {
		createAssetProcessor();
	}
	
	public void finalize() {
		destroyAssetProcessor();
	}
	
	private native void createAssetProcessor();
	
	private native void destroyAssetProcessor();
}
