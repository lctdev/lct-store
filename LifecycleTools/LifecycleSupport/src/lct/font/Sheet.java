package lct.font;

import java.util.Vector;

public class Sheet {
	public static class Glyph {
		public char character;
		public float cellX;
		public float cellY;
		public float width;
		public float leftBearing;
		public float rightBearing;
		public float advance;
	}
	
	public String name;
	public String textureName;
	public float cellWidth;
	public float cellHeight;
	public float cellBaselineX;
	public float cellBaselineY;
	public float ascent;
	public float descent;
	public float lineHeight;
	public int horizontalCellCount;
	public int verticalCellCount;
	public Vector<Glyph> glyphVector;
}
