package lct.part;

import java.util.Vector;

public class Field {
	public static class Range {
		public float min;
		public float max;
	}
	
	public static class Color {
		public float r;
		public float g;
		public float b;
	}
	
	public enum ParticlePropertyType {
		GLOBAL_DISTANCE,
		EXPEL_ANGLE,
		EXPEL_DISTANCE,	
		TANGENT_DISTANCE,
		TANGENT_FLUX,
		SIZE,
		SCALE_X,
		SCALE_X_FLUX,
		SCALE_Y,
		SCALE_Y_FLUX,
		ROTATION,
		COLOR_RATIO,
		ALPHA,
	}
	
	public static class ParticleControl {
		ParticlePropertyType propertyType;
		float initial;
		float velocity;
		float acceleration;
		Range velocityFrameRange;
		Range accelerationFrameRange;
	}
	
	public static class Emitter {				
		public enum ShapeType {
			POINT,
			CIRCLE,
		}
		
		public enum ArrangeType {
			EVEN_EDGE,
			RANDOM_EDGE,
			RANDOM_FILL,			
		};
		
		public enum FlagType {
			EXPEL_OUTWARD,
			ROTATE_OUTWARD,
			REVERSE_ORDER,
			ADDITIVE_BLEND
		};
		
		public String name;
		public String textureRegionName;
		float x;
		float y;
		float shapeParameterA;
		float shapeParameterB;
		float globalAngle;
		Range expelAngleRange;
		Range expelMultiplierRange;
		Range scaleMultiplierRange;
		Range rotationMultiplierRange;
		Range scaleFluxRange;
		Color color0;
		Color color1;
		float delayFrames;
		float frameDuration;
		float particleFrameLifetime;
		int particleCount;
		int emitCount;
		ShapeType shapeType;
		ArrangeType arrangeType;
		public Vector<FlagType> flagTypeVector;
		public Vector<ParticleControl> particleControlVector;
	}
	
	public String name;
	public String textureAtlasName;
	public Vector<Emitter> emitterVector;
}
