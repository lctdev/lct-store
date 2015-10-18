package lct.soun;

import java.util.Vector;

public class Ramp {
	public static class Track {
		
		public enum FinishType
		{
			HOLD,
			LOOP,
		};
		
		public static class Key {
			public enum CurveType
			{
				FLAT,
				LINEAR,
			};
			
			public float second;
			public float value;
			public float slope;
			public CurveType curveType;
		}
		
		public Object targetPropertyType;
		public FinishType finishType;
		public float loopSecond;
		public Vector<Key> keyVector;		
	}
	
	public String name;
	public Vector<Track> trackVector;
}
