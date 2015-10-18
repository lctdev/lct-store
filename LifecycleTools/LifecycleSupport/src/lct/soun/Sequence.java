package lct.soun;

import java.util.Vector;

public class Sequence {
	public static class ClipReference {
		public String clipName;
	}
	
	public static class RampReference {
		public String rampName;
	}
	
	public static class Timeline {
		
		public enum FinishType
		{
			HOLD,
			LOOP,
		};		
		
		public static class Action {
			
			public enum Type
			{
				NONE,
				BEGIN_CLIP,
				END_CLIP,
				SET_RAMP
			}
			
			public float second;
			public Type type;
		}
		public static class BeginClipAction extends Action {
			public int handleIndex;
			public int clipIndex;
		}
		public static class EndClipAction extends Action {
			public int handleIndex;
		}
		public static class SetRampAction extends Action {
			public int handleIndex;
			public int rampIndex;
		}
		
		public FinishType finishType;
		public float loopSecond;
		public Vector<Action> actionVector;
	}
	
	public String name;
	public int clipHandleCount;
	public Vector<ClipReference> clipReferenceVector;
	public Vector<RampReference> rampReferenceVector;
	public Vector<Timeline> timelineVector;
}
