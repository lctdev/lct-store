package lct.spri;

import java.util.Vector;

public class Figure {
	public static class Part {
		public static class Placement {
			public float width;
			public float height;
			public float x;
			public float y;
		}
		
		public enum PropertyType {
			MODULATE_COLOR_R,
			MODULATE_COLOR_G,
			MODULATE_COLOR_B,
			MODULATE_COLOR_A,
			INTERPOLATE_COLOR_R,
			INTERPOLATE_COLOR_G,
			INTERPOLATE_COLOR_B,
			INTERPOLATE_COLOR_A,
		}
		
		public String name;
		public Placement placement;
		public float[] propertyArray;
		public String jointName;
		public String patternName;
	}
	
	public static class Joint {
		public enum PropertyType {
			SCALE_X,
			SCALE_Y,
			ROTATE,
			TRANSLATE_X,
			TRANSLATE_Y,
		}
		
		public enum FlagType {
			SCALE_CHILDREN
		}
		
		public String name;
		public float[] propertyArray;
		public boolean[] flagArray;
		public String parentName;
	}
	
	public static class Pattern {
		public enum PropertyType {
			TEXTURE_INDEX
		}
		
		public String name;
		public String textureTableName;
		public float[] propertyArray;
	}
	
	public String name;
	public Vector<Part> partVector;
	public Vector<Joint> jointVector;
	public Vector<Pattern> patternVector;
}
