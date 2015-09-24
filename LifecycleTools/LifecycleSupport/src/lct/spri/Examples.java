package lct.spri;

import java.util.Vector;

import lct.imag.Texture;
import lct.imag.TextureUtility;
import lct.util.PathUtility;
import lct.pack.Manifest;

public class Examples {
	
	static final String FIGURE_NAME = "example_figure";
	static final String ANIMATION_1_NAME = "example_animation_1";
	static final String ANIMATION_2_NAME = "example_animation_2";
	static final String ANIMATION_3_NAME = "example_animation_3";
	static final String TEXTURE_STRIP_A_NAME = "example_texture_strip_A";
	static final String TEXTURE_STRIP_B_NAME = "example_texture_strip_B";
	static final String TEXTURE_STRIP_C_NAME = "example_texture_strip_C";
	static final String MANIFEST_NAME = "example_manifest";
	
	public static Figure createFigure() {
		Figure figure = new Figure();
		figure.name = FIGURE_NAME;
		{
			// parts
			figure.partVector = new Vector<Figure.Part>();
			{
				Figure.Part part = new Figure.Part();
				part.name = "example_part_A";
				{
					Figure.Part.Placement placement = new Figure.Part.Placement();
					placement.width = 64.0f;
					placement.height = 64.0f;
					placement.x = 0.0f;
					placement.y = 0.0f;
					part.placement = placement;
				}
				part.propertyArray = new float[Figure.Part.PropertyType.values().length];
				{
					part.propertyArray[Figure.Part.PropertyType.MODULATE_COLOR_R.ordinal()] = 1.0f;
					part.propertyArray[Figure.Part.PropertyType.MODULATE_COLOR_G.ordinal()] = 1.0f;
					part.propertyArray[Figure.Part.PropertyType.MODULATE_COLOR_B.ordinal()] = 1.0f;
					part.propertyArray[Figure.Part.PropertyType.MODULATE_COLOR_A.ordinal()] = 1.0f;
					
					part.propertyArray[Figure.Part.PropertyType.INTERPOLATE_COLOR_R.ordinal()] = 1.0f;
					part.propertyArray[Figure.Part.PropertyType.INTERPOLATE_COLOR_G.ordinal()] = 1.0f;
					part.propertyArray[Figure.Part.PropertyType.INTERPOLATE_COLOR_B.ordinal()] = 1.0f;
					part.propertyArray[Figure.Part.PropertyType.INTERPOLATE_COLOR_A.ordinal()] = 0.0f;
				}
				part.jointName = "example_joint_A";
				part.patternName = "example_pattern_A";
				figure.partVector.add(part);
			}
			{
				Figure.Part part = new Figure.Part();
				part.name = "example_part_B";				
				{
					Figure.Part.Placement placement = new Figure.Part.Placement();
					placement.width = 32.0f;
					placement.height = 32.0f;
					placement.x = 0.0f;
					placement.y = 0.0f;
					part.placement = placement;
				}
				part.propertyArray = new float[Figure.Part.PropertyType.values().length];
				{
					part.propertyArray[Figure.Part.PropertyType.MODULATE_COLOR_R.ordinal()] = 1.0f;
					part.propertyArray[Figure.Part.PropertyType.MODULATE_COLOR_G.ordinal()] = 1.0f;
					part.propertyArray[Figure.Part.PropertyType.MODULATE_COLOR_B.ordinal()] = 1.0f;
					part.propertyArray[Figure.Part.PropertyType.MODULATE_COLOR_A.ordinal()] = 1.0f;
					
					part.propertyArray[Figure.Part.PropertyType.INTERPOLATE_COLOR_R.ordinal()] = 0.0f;
					part.propertyArray[Figure.Part.PropertyType.INTERPOLATE_COLOR_G.ordinal()] = 0.0f;
					part.propertyArray[Figure.Part.PropertyType.INTERPOLATE_COLOR_B.ordinal()] = 0.0f;
					part.propertyArray[Figure.Part.PropertyType.INTERPOLATE_COLOR_A.ordinal()] = 0.0f;
				}
				part.jointName = "example_joint_B";
				part.patternName = "example_pattern_B";
				figure.partVector.add(part);
			}
			{
				Figure.Part part = new Figure.Part();
				part.name = "example_part_C";
				{
					Figure.Part.Placement placement = new Figure.Part.Placement();
					placement.width = 8.0f;
					placement.height = 8.0f;
					placement.x = 0.0f;
					placement.y = 0.0f;
					part.placement = placement;
				}
				part.propertyArray = new float[Figure.Part.PropertyType.values().length];
				{
					part.propertyArray[Figure.Part.PropertyType.MODULATE_COLOR_R.ordinal()] = 1.0f;
					part.propertyArray[Figure.Part.PropertyType.MODULATE_COLOR_G.ordinal()] = 1.0f;
					part.propertyArray[Figure.Part.PropertyType.MODULATE_COLOR_B.ordinal()] = 1.0f;
					part.propertyArray[Figure.Part.PropertyType.MODULATE_COLOR_A.ordinal()] = 1.0f;
					
					part.propertyArray[Figure.Part.PropertyType.INTERPOLATE_COLOR_R.ordinal()] = 0.0f;
					part.propertyArray[Figure.Part.PropertyType.INTERPOLATE_COLOR_G.ordinal()] = 0.0f;
					part.propertyArray[Figure.Part.PropertyType.INTERPOLATE_COLOR_B.ordinal()] = 0.0f;
					part.propertyArray[Figure.Part.PropertyType.INTERPOLATE_COLOR_A.ordinal()] = 0.0f;
				}
				part.jointName = "example_joint_C";
				part.patternName = "example_pattern_C";
				figure.partVector.add(part);
			}
			
			// joints
			figure.jointVector = new Vector<Figure.Joint>();
			{
				Figure.Joint joint = new Figure.Joint();
				joint.name = "example_joint_A";				
				joint.propertyArray = new float[Figure.Joint.PropertyType.values().length];
				{
					joint.propertyArray[Figure.Joint.PropertyType.SCALE_X.ordinal()] = 1.0f;
					joint.propertyArray[Figure.Joint.PropertyType.SCALE_Y.ordinal()] = 1.0f;
					joint.propertyArray[Figure.Joint.PropertyType.ROTATE.ordinal()] = 0.0f;
					joint.propertyArray[Figure.Joint.PropertyType.TRANSLATE_X.ordinal()] = 0.0f;
					joint.propertyArray[Figure.Joint.PropertyType.TRANSLATE_Y.ordinal()] = 0.0f;								
				}
				joint.flagArray = new boolean[Figure.Joint.PropertyType.values().length];
				{
					joint.flagArray[Figure.Joint.FlagType.SCALE_CHILDREN.ordinal()] = false;
				}
				joint.parentName = null;
				figure.jointVector.add(joint);
			}
			{
				Figure.Joint joint = new Figure.Joint();
				joint.name = "example_joint_B";
				joint.propertyArray = new float[Figure.Joint.PropertyType.values().length];
				{
					joint.propertyArray[Figure.Joint.PropertyType.SCALE_X.ordinal()] = 1.0f;
					joint.propertyArray[Figure.Joint.PropertyType.SCALE_Y.ordinal()] = 1.0f;
					joint.propertyArray[Figure.Joint.PropertyType.ROTATE.ordinal()] = 0.0f;
					joint.propertyArray[Figure.Joint.PropertyType.TRANSLATE_X.ordinal()] = 16.0f;
					joint.propertyArray[Figure.Joint.PropertyType.TRANSLATE_Y.ordinal()] = 16.0f;
				}
				joint.flagArray = new boolean[Figure.Joint.PropertyType.values().length];
				{
					joint.flagArray[Figure.Joint.FlagType.SCALE_CHILDREN.ordinal()] = false;
				}
				joint.parentName = "example_joint_A";
				figure.jointVector.add(joint);
			}
			{
				Figure.Joint joint = new Figure.Joint();
				joint.name = "example_joint_C";
				joint.propertyArray = new float[Figure.Joint.PropertyType.values().length];
				{
					joint.propertyArray[Figure.Joint.PropertyType.SCALE_X.ordinal()] = 1.0f;
					joint.propertyArray[Figure.Joint.PropertyType.SCALE_Y.ordinal()] = 1.0f;
					joint.propertyArray[Figure.Joint.PropertyType.ROTATE.ordinal()] = 0.0f;
					joint.propertyArray[Figure.Joint.PropertyType.TRANSLATE_X.ordinal()] = -28.0f;
					joint.propertyArray[Figure.Joint.PropertyType.TRANSLATE_Y.ordinal()] = 28.0f;
				}
				joint.flagArray = new boolean[Figure.Joint.PropertyType.values().length];
				{
					joint.flagArray[Figure.Joint.FlagType.SCALE_CHILDREN.ordinal()] = false;
				}
				joint.parentName = "example_joint_A";
				figure.jointVector.add(joint);
			}
			
			figure.patternVector = new Vector<Figure.Pattern>();
			{
				Figure.Pattern pattern = new Figure.Pattern();
				pattern.name = "example_pattern_A";
				pattern.textureTableName = TEXTURE_STRIP_A_NAME;
				pattern.propertyArray = new float[Figure.Pattern.PropertyType.values().length];
				{
					pattern.propertyArray[Figure.Pattern.PropertyType.TEXTURE_INDEX.ordinal()] = 0.0f;
				}
				figure.patternVector.add(pattern);
			}
			{
				Figure.Pattern pattern = new Figure.Pattern();
				pattern.name = "example_pattern_B";
				pattern.textureTableName = TEXTURE_STRIP_B_NAME;
				pattern.propertyArray = new float[Figure.Pattern.PropertyType.values().length];
				{
					pattern.propertyArray[Figure.Pattern.PropertyType.TEXTURE_INDEX.ordinal()] = 0.0f;
				}
				figure.patternVector.add(pattern);
			}
			{
				Figure.Pattern pattern = new Figure.Pattern();
				pattern.name = "example_pattern_C";
				pattern.textureTableName = TEXTURE_STRIP_C_NAME;
				pattern.propertyArray = new float[Figure.Pattern.PropertyType.values().length];
				{
					pattern.propertyArray[Figure.Pattern.PropertyType.TEXTURE_INDEX.ordinal()] = 0.0f;
				}
				figure.patternVector.add(pattern);
			}
		}
		
		return figure;
	}
	
	public static Animation createAnimation1() {
		Animation animation = new Animation();
		animation.name = ANIMATION_1_NAME;
		{
			animation.trackVector = new Vector<Animation.Track>();
			{
				Animation.Track track = new Animation.Track();
				track.targetName = "example_joint_A";				
				track.targetType = Animation.Track.TargetType.JOINT;
				track.targetPropertyType = Figure.Joint.PropertyType.TRANSLATE_X;
				track.finishType = Animation.Track.FinishType.LOOP;
				track.keyVector = new Vector<Animation.Track.Key>();
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 0.0f;
					key.value = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.LINEAR;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 60.0f;
					key.value = 128.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.LINEAR;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 120.0f;
					key.value = 128.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.LINEAR;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 180.0f;
					key.value = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				animation.trackVector.add(track);
			}
			{
				Animation.Track track = new Animation.Track();
				track.targetName = "example_joint_B";
				track.targetType = Animation.Track.TargetType.JOINT;
				track.targetPropertyType = Figure.Joint.PropertyType.ROTATE;
				track.finishType = Animation.Track.FinishType.LOOP;
				track.keyVector = new Vector<Animation.Track.Key>();
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 0.0f;
					key.value = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.LINEAR;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 60.0f;
					key.value = 1.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				animation.trackVector.add(track);
			}
			{
				Animation.Track track = new Animation.Track();
				track.targetName = "example_joint_B";		
				track.targetType = Animation.Track.TargetType.JOINT;
				track.targetPropertyType = Figure.Joint.PropertyType.TRANSLATE_Y;
				track.finishType = Animation.Track.FinishType.LOOP;
				track.keyVector = new Vector<Animation.Track.Key>();
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 0.0f;
					key.value = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 60.0f;
					key.value = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.LINEAR;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 90.0f;
					key.value = 64.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.LINEAR;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 120.0f;
					key.value = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 180.0f;
					key.value = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				animation.trackVector.add(track);
			}
			{
				Animation.Track track = new Animation.Track();
				track.targetName = "example_pattern_C";		
				track.targetType = Animation.Track.TargetType.PATTERN;
				track.targetPropertyType = Figure.Pattern.PropertyType.TEXTURE_INDEX;
				track.finishType = Animation.Track.FinishType.LOOP;
				track.keyVector = new Vector<Animation.Track.Key>();
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 0.0f;
					key.value = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 60.0f;
					key.value = 1.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 70.0f;
					key.value = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 80.0f;
					key.value = 1.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 90.0f;
					key.value = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 100.0f;
					key.value = 1.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 110.0f;
					key.value = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 120.0f;
					key.value = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 180.0f;
					key.value = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				animation.trackVector.add(track);
			}
		}
		
		AnimationUtility.calculateSlopes(animation);
		
		return animation;
	}
	
	public static Animation createAnimation2() {
		Animation animation = new Animation();
		animation.name = ANIMATION_2_NAME;
		{
			animation.trackVector = new Vector<Animation.Track>();
			{
				Animation.Track track = new Animation.Track();
				track.targetName = "example_part_A";
				track.targetType = Animation.Track.TargetType.PART;
				track.targetPropertyType = Figure.Part.PropertyType.INTERPOLATE_COLOR_A;
				track.finishType = Animation.Track.FinishType.LOOP;
				track.keyVector = new Vector<Animation.Track.Key>();
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 0.0f;
					key.value = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.LINEAR;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 60.0f;
					key.value = 1.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.LINEAR;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 120.0f;
					key.value = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				animation.trackVector.add(track);
			}
		}
		
		AnimationUtility.calculateSlopes(animation);
		
		return animation;
	}
	
	public static Animation createAnimation3() {
		Animation animation = new Animation();
		animation.name = ANIMATION_3_NAME;
		{
			animation.trackVector = new Vector<Animation.Track>();
			{
				Animation.Track track = new Animation.Track();
				track.targetName = "example_joint_A";
				track.targetType = Animation.Track.TargetType.JOINT;
				track.targetPropertyType = Figure.Joint.PropertyType.TRANSLATE_Y;
				track.finishType = Animation.Track.FinishType.LOOP;
				track.keyVector = new Vector<Animation.Track.Key>();
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 0.0f;
					key.value = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 30.0f;
					key.value = 0.0f;
					key.slope = 300.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.MANUAL;
					key.curveType = Animation.Track.Key.CurveType.HERMITE;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 60.0f;
					key.value = 128.0f;
					key.slope = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.MANUAL;
					key.curveType = Animation.Track.Key.CurveType.HERMITE;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 90.0f;
					key.value = 0.0f;
					key.slope = -300.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.MANUAL;
					key.curveType = Animation.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 150.0f;
					key.value = 0.0f;
					key.slope = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				animation.trackVector.add(track);
			}
			{
				Animation.Track track = new Animation.Track();
				track.targetName = "example_joint_A";
				track.targetType = Animation.Track.TargetType.JOINT;
				track.targetPropertyType = Figure.Joint.PropertyType.SCALE_X;
				track.finishType = Animation.Track.FinishType.LOOP;
				track.keyVector = new Vector<Animation.Track.Key>();
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 0.0f;
					key.value = 1.0f;
					key.slope = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.MANUAL;
					key.curveType = Animation.Track.Key.CurveType.HERMITE;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 15.0f;
					key.value = 2.0f;
					key.slope = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.MANUAL;
					key.curveType = Animation.Track.Key.CurveType.HERMITE;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 30.0f;
					key.value = 1.0f;
					key.slope = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.MANUAL;
					key.curveType = Animation.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 150.0f;
					key.value = 0.0f;
					key.slope = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				animation.trackVector.add(track);
			}
			{
				Animation.Track track = new Animation.Track();
				track.targetName = "example_joint_A";
				track.targetType = Animation.Track.TargetType.JOINT;
				track.targetPropertyType = Figure.Joint.PropertyType.SCALE_Y;
				track.finishType = Animation.Track.FinishType.LOOP;
				track.keyVector = new Vector<Animation.Track.Key>();
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 0.0f;
					key.value = 1.0f;
					key.slope = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.MANUAL;
					key.curveType = Animation.Track.Key.CurveType.HERMITE;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 15.0f;
					key.value = 0.5f;
					key.slope = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.MANUAL;
					key.curveType = Animation.Track.Key.CurveType.HERMITE;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 30.0f;
					key.value = 1.0f;
					key.slope = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.MANUAL;
					key.curveType = Animation.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				{
					Animation.Track.Key key = new Animation.Track.Key();
					key.frame = 150.0f;
					key.value = 0.0f;
					key.slope = 0.0f;
					key.slopeCalc = Animation.Track.Key.SlopeCalc.AUTO;
					key.curveType = Animation.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				animation.trackVector.add(track);
			}
		}
		
		AnimationUtility.calculateSlopes(animation);
		
		return animation;
	}
	
	public static Texture createTextureStripA() {
		final int TEXTURE_WIDTH = 32;
		final int TEXTURE_HEIGHT = 32;		
		
		Vector<Texture> textureVector = new Vector<Texture>();
		{
			Texture.TexelRGBA stripeATexel = new Texture.TexelRGBA();
			stripeATexel.r = 0;
			stripeATexel.g = 255;
			stripeATexel.b = 0;
			stripeATexel.a = 255;
			Texture.TexelRGBA stripeBTexel = new Texture.TexelRGBA();
			stripeBTexel.r = 0;
			stripeBTexel.g = 0;
			stripeBTexel.b = 255;
			stripeBTexel.a = 255;
			
			Texture texture = createStripeTexture(TEXTURE_WIDTH, TEXTURE_HEIGHT, stripeATexel, stripeBTexel);
			
			textureVector.add(texture);
		}
		
		Texture stripTexture = TextureUtility.mergeStrip(textureVector, TEXTURE_WIDTH, TEXTURE_HEIGHT);
		stripTexture.name = TEXTURE_STRIP_A_NAME;
		
		return stripTexture;
	}
	
	public static Texture createTextureStripB() {
		final int TEXTURE_WIDTH = 8;
		final int TEXTURE_HEIGHT = 8;		
		
		Vector<Texture> textureVector = new Vector<Texture>();
		{
			Texture.TexelRGBA checkerATexel = new Texture.TexelRGBA();
			checkerATexel.r = 255;
			checkerATexel.g = 255;
			checkerATexel.b = 255;
			checkerATexel.a = 255;
			Texture.TexelRGBA checkerBTexel = new Texture.TexelRGBA();
			checkerBTexel.r = 0;
			checkerBTexel.g = 0;
			checkerBTexel.b = 0;
			checkerBTexel.a = 0;
			
			Texture texture = createCheckerTexture(TEXTURE_WIDTH, TEXTURE_HEIGHT, checkerATexel, checkerBTexel);
			
			textureVector.add(texture);
		}
		
		Texture stripTexture = TextureUtility.mergeStrip(textureVector, TEXTURE_WIDTH, TEXTURE_HEIGHT);
		stripTexture.name = TEXTURE_STRIP_B_NAME;
		
		return stripTexture;
	}
	
	public static Texture createTextureStripC() {
		final int TEXTURE_WIDTH = 8;
		final int TEXTURE_HEIGHT = 8;		
		
		Vector<Texture> textureVector = new Vector<Texture>();
		{
			Texture.TexelRGBA circleATexel = new Texture.TexelRGBA();
			circleATexel.r = 127;
			circleATexel.g = 0;
			circleATexel.b = 0;
			circleATexel.a = 255;
			Texture.TexelRGBA circleBTexel = new Texture.TexelRGBA();
			circleBTexel.r = 127;
			circleBTexel.g = 0;
			circleBTexel.b = 0;
			circleBTexel.a = 255;
			
			Texture texture = createCircleTexture(TEXTURE_WIDTH, TEXTURE_HEIGHT, circleATexel, circleBTexel, 2.0f, 2.0f);			
			
			textureVector.add(texture);
		}
		{
			Texture.TexelRGBA circleATexel = new Texture.TexelRGBA();
			circleATexel.r = 255;
			circleATexel.g = 127;
			circleATexel.b = 127;
			circleATexel.a = 255;
			Texture.TexelRGBA circleBTexel = new Texture.TexelRGBA();
			circleBTexel.r = 255;
			circleBTexel.g = 0;
			circleBTexel.b = 0;
			circleBTexel.a = 255;
			
			Texture texture = createCircleTexture(TEXTURE_WIDTH, TEXTURE_HEIGHT, circleATexel, circleBTexel, 2.0f, 4.0f);
			
			textureVector.add(texture);
		}
		
		Texture stripTexture = TextureUtility.mergeStrip(textureVector, TEXTURE_WIDTH, TEXTURE_HEIGHT);
		stripTexture.name = TEXTURE_STRIP_C_NAME;
		
		return stripTexture;
	}
	
	public static Texture createStripeTexture(int width, int height, Texture.TexelRGBA stripeATexel, Texture.TexelRGBA stripeBTexel) {
		Texture texture = new Texture();					
		
		int interceptA = width / 2;
		int interceptB = 0;
		int interceptC = -height / 2;
		
		int texelCount = width * height;
		Texture.TexelRGBA[] texelArray = new Texture.TexelRGBA[texelCount];
		
		for (int texelY = 0; texelY < height; ++texelY) {
			for (int texelX = 0; texelX < width; ++texelX) {
				int lineAY = texelX + interceptA;
				int lineBY = texelX + interceptB;
				int lineCY = texelX + interceptC;
									
				Texture.TexelRGBA stripeTexel;
				if (texelY > lineAY) {
					stripeTexel = stripeATexel;
				}
				else if (texelY > lineBY) {
					stripeTexel = stripeBTexel;
				}
				else if (texelY > lineCY) {
					stripeTexel = stripeATexel;
				}
				else {
					stripeTexel = stripeBTexel;
				}
									
				Texture.TexelRGBA texel = new Texture.TexelRGBA();
				texel.r = stripeTexel.r;
				texel.g = stripeTexel.g;
				texel.b = stripeTexel.b;
				texel.a = stripeTexel.a;
				
				int texelIndex = texelY * width + texelX;
				texelArray[texelIndex] = texel;
			}
		}
		
		texture.width = width;
		texture.height = height;
		texture.texelArray = texelArray;
		
		return texture;
	}
	
	public static Texture createCheckerTexture(int width, int height, Texture.TexelRGBA checkerATexel, Texture.TexelRGBA checkerBTexel) {
		Texture texture = new Texture();					
		
		int texelCount = width * height;
		Texture.TexelRGBA[] texelArray = new Texture.TexelRGBA[texelCount];
		
		for (int texelY = 0; texelY < height; ++texelY) {
			for (int texelX = 0; texelX < width; ++texelX) {
				Texture.TexelRGBA checkerTexel;
				boolean xOdd = (texelX & 0x01) == 0x01;
				boolean yOdd = (texelY & 0x01) == 0x01;
				if (xOdd == yOdd) {
					checkerTexel = checkerATexel;
				}
				else {
					checkerTexel = checkerBTexel;
				}
									
				Texture.TexelRGBA texel = new Texture.TexelRGBA();
				texel.r = checkerTexel.r;
				texel.g = checkerTexel.g;
				texel.b = checkerTexel.b;
				texel.a = checkerTexel.a;
				
				int texelIndex = texelY * width + texelX;
				texelArray[texelIndex] = texel;
			}
		}
		
		texture.width = width;
		texture.height = height;
		texture.texelArray = texelArray;
		
		return texture;
	}
	
	public static Texture createCircleTexture(int width, int height, Texture.TexelRGBA circleATexel, Texture.TexelRGBA circleBTexel, float circleARadius, float circleBRadius) {
		Texture.TexelRGBA outsideTexel = new Texture.TexelRGBA();
		outsideTexel.r = 0;
		outsideTexel.g = 0;
		outsideTexel.b = 0;
		outsideTexel.a = 0;
		
		Texture texture = new Texture();
		
		int texelCount = width * height;
		Texture.TexelRGBA[] texelArray = new Texture.TexelRGBA[texelCount];
		
		float radiusASquared = circleARadius * circleARadius;
		float radiusBSquared = circleBRadius * circleBRadius;
		float centerX = (float)width / 2.0f;
		float centerY = (float)height / 2.0f;
		float pointY = 0.5f;
		for (int texelY = 0; texelY < height; ++texelY) {
			float pointX = 0.5f;
			for (int texelX = 0; texelX < width; ++texelX) {
				Texture.TexelRGBA sourceTexel;
				float deltaX = pointX - centerX;
				float deltaY = pointY - centerY;
				float distanceSquared = deltaX * deltaX + deltaY * deltaY;
				if (distanceSquared <= radiusASquared) {
					sourceTexel = circleATexel;
				}
				else if (distanceSquared <= radiusBSquared) {
					sourceTexel = circleBTexel;
				}
				else {
					sourceTexel = outsideTexel;
				}
				
				Texture.TexelRGBA texel = new Texture.TexelRGBA();
				texel.r = sourceTexel.r;
				texel.g = sourceTexel.g;
				texel.b = sourceTexel.b;
				texel.a = sourceTexel.a;
				
				int texelIndex = texelY * width + texelX;
				texelArray[texelIndex] = texel;
				
				pointX += 1.0f;
			}
			pointY += 1.0f;
		}		
		
		texture.width = width;
		texture.height = height;
		texture.texelArray = texelArray;
		
		return texture;
	}
	
	public static Texture createHermiteTestTexture() {
		Texture texture = new Texture();
		
		Texture.TexelRGBA blankTexel = new Texture.TexelRGBA();
		blankTexel.r = 255;
		blankTexel.g = 255;
		blankTexel.b = 255;
		blankTexel.a = 255;
		
		Texture.TexelRGBA pointTexel = new Texture.TexelRGBA();
		pointTexel.r = 0;
		pointTexel.g = 255;
		pointTexel.b = 0;
		pointTexel.a = 255;
		
		int width = 120;
		int height = 100;
		
		int texelCount = width * height;
		Texture.TexelRGBA[] texelArray = new Texture.TexelRGBA[texelCount];
		
		for (int texelY = 0; texelY < height; ++texelY) {
			for (int texelX = 0; texelX < width; ++texelX) {
				Texture.TexelRGBA texel = new Texture.TexelRGBA();
				texel.r = blankTexel.r;
				texel.g = blankTexel.g;
				texel.b = blankTexel.b;
				texel.a = blankTexel.a;
				
				int texelIndex = texelY * width + texelX;
				texelArray[texelIndex] = texel;
			}
		}
		
		float currKeyFrame = 0.0f;
		float currKeyValue = 0.0f;
		//float currKeySlope = 200.0f;
		float currKeySlope = 0.0f;
		float nextKeyFrame = 30.0f;
		float nextKeyValue = 64.0f;
		float nextKeySlope = 0.0f;
		for (float frame = 0.0f; frame < 30.0f; ++frame) {
			float frameDelta = frame - currKeyFrame; 
			float frameSpan = nextKeyFrame - currKeyFrame;
			float t = frameDelta / frameSpan;
			float tSqu = t * t;
			float tCub = t * t * t;
			float basis1 = (2.0f * tCub) - (3.0f * tSqu) + 1.0f;
			float basis2 = -(2.0f * tCub) + (3.0f * tSqu);
			float basis3 = tCub - (2.0f * tSqu) + t;
			float basis4 = tCub - tSqu;
			float value = (basis1 * currKeyValue) + (basis2 * nextKeyValue) + (basis3 * currKeySlope) + (basis4 * nextKeySlope);
			
			Texture.TexelRGBA texel = new Texture.TexelRGBA();
			texel.r = pointTexel.r;
			texel.g = pointTexel.g;
			texel.b = pointTexel.b;
			texel.a = pointTexel.a;
			
			int texelY = (int)value;
			int texelX = (int)frame;
			int texelIndex = texelY * width + texelX;
			if (texelIndex < texelCount)
			{
				texelArray[texelIndex] = texel;
			}
		}
		
		texture.width = width;
		texture.height = height;
		texture.texelArray = texelArray;
		
		return texture;
	}
	
	public static Manifest createManifest(String directoryPath) {
		Manifest manifest = new Manifest();
		manifest.name = MANIFEST_NAME;
		{
			manifest.assetEntryVector = new Vector<Manifest.AssetEntry>();
			{
				{
					String fileName = FIGURE_NAME + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = Constants.GROUP_CODE;
					assetEntry.typeCode = Constants.FIGURE_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);					
					manifest.assetEntryVector.add(assetEntry);
				}

				{
					String fileName = ANIMATION_1_NAME + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = Constants.GROUP_CODE;
					assetEntry.typeCode = Constants.ANIMATION_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);					
					manifest.assetEntryVector.add(assetEntry);
				}
				{
					String fileName = ANIMATION_2_NAME + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = Constants.GROUP_CODE;
					assetEntry.typeCode = Constants.ANIMATION_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);
					manifest.assetEntryVector.add(assetEntry);
				}
				{
					String fileName = ANIMATION_3_NAME + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = Constants.GROUP_CODE;
					assetEntry.typeCode = Constants.ANIMATION_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);
					manifest.assetEntryVector.add(assetEntry);
				}

				{
					String fileName = TEXTURE_STRIP_A_NAME + ".tga";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = lct.imag.Constants.GROUP_CODE;
					assetEntry.typeCode = lct.imag.Constants.TEXTURE_TABLE_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);
					manifest.assetEntryVector.add(assetEntry);
				}
				{
					String fileName = TEXTURE_STRIP_B_NAME + ".tga";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = lct.imag.Constants.GROUP_CODE;
					assetEntry.typeCode = lct.imag.Constants.TEXTURE_TABLE_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);
					manifest.assetEntryVector.add(assetEntry);
				}
				{
					String fileName = TEXTURE_STRIP_C_NAME + ".tga";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = lct.imag.Constants.GROUP_CODE;
					assetEntry.typeCode = lct.imag.Constants.TEXTURE_TABLE_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);
					manifest.assetEntryVector.add(assetEntry);
				}
			}
		}
		return manifest;
	}
	
}

