package lct.soun;

import java.util.Vector;

public class Clip {
	public enum PropertyType {
		VOLUME,
		PITCH,
	}
	
	public static class Segment {	
		public String waveName;
	}
	
	public String name;
	public float[] propertyArray;
	public boolean loop;
	public Vector<Segment> segmentVector;
}
