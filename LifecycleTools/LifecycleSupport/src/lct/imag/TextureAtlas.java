package lct.imag;

import java.util.Vector;

public class TextureAtlas {
	public static class Region {
		public Texture texture;
		public int x;
		public int y;
	}
	
	public String name;
	public int width;
	public int height;
	public Vector<Region> regionVector;
	public Texture mergedTexture;
}

