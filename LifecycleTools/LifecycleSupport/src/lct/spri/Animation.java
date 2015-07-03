package lct.spri;

import java.util.Vector;

public class Animation {
	public static class Track {
		public enum TargetType {
			PART,
			JOINT,
			PATTERN
		}
		
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
				HERMITE
			};
			
			public enum SlopeCalc
			{
				AUTO,
				MANUAL
			};
			
			public float frame;
			public float value;
			public float slope;
			public SlopeCalc slopeCalc;
			public CurveType curveType;
		}
		
		public String targetName;
		public TargetType targetType;
		public Object targetPropertyType;
		public FinishType finishType;
		public Vector<Key> keyVector;
	}
	
	public String name;
	public Vector<Track> trackVector;
}
