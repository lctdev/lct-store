package lct.pack;

import java.util.Vector;

public class Manifest {
	public static class AssetEntry {
		public String groupCode;
		public String typeCode;
		public String filePath;
	}
	
	public String name;
	public Vector<AssetEntry> assetEntryVector;
}
