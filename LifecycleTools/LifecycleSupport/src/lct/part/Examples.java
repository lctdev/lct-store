package lct.part;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RadialGradientPaint;
import java.awt.RenderingHints;
import java.awt.Stroke;
import java.awt.geom.Ellipse2D;
import java.awt.geom.GeneralPath;
import java.awt.geom.Point2D;
import java.awt.image.BufferedImage;
import java.util.Vector;

import lct.imag.Texture;
import lct.imag.TextureAtlas;
import lct.imag.TextureAtlas.Region;
import lct.imag.TextureUtility;
import lct.util.PathUtility;
import lct.pack.Manifest;

public class Examples {
	
	static final String FIELD_SPRAY_NAME = "example_field_spray";
	static final String FIELD_ORBIT_NAME = "example_field_orbit";
	static final String FIELD_PULSE_NAME = "example_field_pulse";
	static final String TEXTURE_DROPLET_NAME = "example_texture_droplet";
	static final String TEXTURE_STAR_NAME = "example_texture_star";
	static final String TEXTURE_ORB_NAME = "example_texture_orb";
	static final String TEXTURE_ATLAS_NAME = "example_particle_texture_atlas";
	static final String MANIFEST_NAME = "example_manifest";
	
	static final int TEXTURE_WIDTH = 32;
	static final int TEXTURE_HEIGHT = 32;
	static final int TEXTURE_ATLAS_WIDTH = 64;
	static final int TEXTURE_ATLAS_HEIGHT = 64;
	
	public static Field createFieldSpray() {
		Field field = new Field();
		field.name = FIELD_SPRAY_NAME;
		field.textureAtlasName = TEXTURE_ATLAS_NAME;
		{
			// emitters
			field.emitterVector = new Vector<Field.Emitter>();			
			{
				Field.Emitter emitter = new Field.Emitter();
				emitter.name = "example_emitter_A";
				emitter.textureRegionName = TEXTURE_DROPLET_NAME;
				emitter.x = 0.0f;
				emitter.y = 0.0f;
				emitter.shapeParameterA = 0.0f;
				emitter.shapeParameterB = 0.0f;
				emitter.globalAngle = 0.75f;
				{
					Field.Range range = new Field.Range();
					range.min = 0.2f;
					range.max = 0.3f;
					emitter.expelAngleRange = range;
				}
				{
					Field.Range range = new Field.Range();
					range.min = 1.0f;
					range.max = 1.5f;
					emitter.expelMultiplierRange = range;
				}
				{
					Field.Range range = new Field.Range();
					range.min = 12.8f;
					range.max = 19.2f;
					emitter.scaleMultiplierRange = range;
				}
				{
					Field.Range range = new Field.Range();
					range.min = 0.8f;
					range.max = 1.2f;
					emitter.rotationMultiplierRange = range;
				}
				{
					Field.Range range = new Field.Range();
					range.min = 0.0f;
					range.max = 1.0f;
					emitter.scaleFluxRange = range;
				}
				
				{
					Field.Color color = new Field.Color();
					color.r = 1.0f;
					color.g = 1.0f;
					color.b = 1.0f;
					emitter.color0 = color;
				}
				{
					Field.Color color = new Field.Color();
					color.r = 0.0f;
					color.g = 0.0f;
					color.b = 1.0f;
					emitter.color1 = color;
				}
				
				emitter.delayFrames = 4.0f;
				emitter.frameDuration = 0.0f;
				emitter.particleFrameLifetime = 90.0f;
				emitter.particleCount = 32;
				emitter.emitCount = 1;
				emitter.shapeType = Field.Emitter.ShapeType.POINT;
				emitter.arrangeType = Field.Emitter.ArrangeType.EVEN_EDGE;
				
				{
					Vector<Field.Emitter.FlagType> flagTypeVector = new Vector<Field.Emitter.FlagType>();
					emitter.flagTypeVector = flagTypeVector;
					emitter.flagTypeVector.add(Field.Emitter.FlagType.REVERSE_ORDER);
				}
				
				{
					emitter.particleControlVector = new Vector<Field.ParticleControl>();
					{
						Field.ParticleControl particleControl = new Field.ParticleControl();
						particleControl.propertyType = Field.ParticlePropertyType.GLOBAL_DISTANCE;
						particleControl.initial = 0.0f;
						particleControl.velocity = 0.0f;
						particleControl.acceleration = 0.2f;
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.velocityFrameRange = range;
						}
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.accelerationFrameRange = range;
						}
						emitter.particleControlVector.add(particleControl);
					}
					{
						Field.ParticleControl particleControl = new Field.ParticleControl();
						particleControl.propertyType = Field.ParticlePropertyType.EXPEL_DISTANCE;
						particleControl.initial = 0.0f;
						particleControl.velocity = 6.0f;
						particleControl.acceleration = 0.0f;
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.velocityFrameRange = range;
						}
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.accelerationFrameRange = range;
						}
						emitter.particleControlVector.add(particleControl);
					}
					{
						Field.ParticleControl particleControl = new Field.ParticleControl();
						particleControl.propertyType = Field.ParticlePropertyType.ROTATION;
						particleControl.initial = 0.0f;
						particleControl.velocity = 0.1f;
						particleControl.acceleration = 0.0f;
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.velocityFrameRange = range;
						}
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.accelerationFrameRange = range;
						}
						emitter.particleControlVector.add(particleControl);
					}
					{
						Field.ParticleControl particleControl = new Field.ParticleControl();
						particleControl.propertyType = Field.ParticlePropertyType.COLOR_RATIO;
						particleControl.initial = 0.0f;
						particleControl.velocity = 0.01f;
						particleControl.acceleration = 0.0f;
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.velocityFrameRange = range;
						}
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.accelerationFrameRange = range;
						}
						emitter.particleControlVector.add(particleControl);
					}
					{
						Field.ParticleControl particleControl = new Field.ParticleControl();
						particleControl.propertyType = Field.ParticlePropertyType.ALPHA;
						particleControl.initial = 1.0f;
						particleControl.velocity = -1.0f / 15.0f;
						particleControl.acceleration = 0.0f;
						{
							Field.Range range = new Field.Range();
							range.min = 75.0f;
							range.max = 90.0f;
							particleControl.velocityFrameRange = range;
						}
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.accelerationFrameRange = range;
						}
						emitter.particleControlVector.add(particleControl);
					}
				}
				
				field.emitterVector.add(emitter);
			}
		}
		
		return field;
	}
	
	public static Field createFieldOrbit() {
		Field field = new Field();
		field.name = FIELD_ORBIT_NAME;
		field.textureAtlasName = TEXTURE_ATLAS_NAME;
		{
			// emitters
			field.emitterVector = new Vector<Field.Emitter>();			
			{
				Field.Emitter emitter = new Field.Emitter();
				emitter.name = "example_emitter_A";
				emitter.textureRegionName = TEXTURE_STAR_NAME;
				emitter.x = 0.0f;
				emitter.y = 0.0f;
				emitter.shapeParameterA = 0.0f;
				emitter.shapeParameterB = 0.0f;
				emitter.globalAngle = 0.0f;
				{
					Field.Range range = new Field.Range();
					range.min = 0.0f;
					range.max = 0.0f;
					emitter.expelAngleRange = range;
				}
				{
					Field.Range range = new Field.Range();
					range.min = 1.0f;
					range.max = 1.0f;
					emitter.expelMultiplierRange = range;
				}
				{
					Field.Range range = new Field.Range();
					range.min = 16.0f;
					range.max = 16.0f;
					emitter.scaleMultiplierRange = range;
				}
				{
					Field.Range range = new Field.Range();
					range.min = 0.0f;
					range.max = 0.0f;
					emitter.rotationMultiplierRange = range;
				}
				{
					Field.Range range = new Field.Range();
					range.min = 0.0f;
					range.max = 1.0f;
					emitter.scaleFluxRange = range;
				}
				
				{
					Field.Color color = new Field.Color();
					color.r = 1.0f;
					color.g = 1.0f;
					color.b = 0.0f;
					emitter.color0 = color;
				}
				{
					Field.Color color = new Field.Color();
					color.r = 0.0f;
					color.g = 1.0f;
					color.b = 0.0f;
					emitter.color1 = color;
				}
				
				emitter.delayFrames = 30.0f;
				emitter.frameDuration = 90.0f;
				emitter.particleFrameLifetime = 90.0f;
				emitter.particleCount = 32;
				emitter.emitCount = 8;
				emitter.shapeType = Field.Emitter.ShapeType.CIRCLE;
				emitter.arrangeType = Field.Emitter.ArrangeType.EVEN_EDGE;
				
				{
					Vector<Field.Emitter.FlagType> flagTypeVector = new Vector<Field.Emitter.FlagType>();
					emitter.flagTypeVector = flagTypeVector;
					emitter.flagTypeVector.add(Field.Emitter.FlagType.REVERSE_ORDER);
					emitter.flagTypeVector.add(Field.Emitter.FlagType.EXPEL_OUTWARD);
				}
				
				{
					emitter.particleControlVector = new Vector<Field.ParticleControl>();
					{
						Field.ParticleControl particleControl = new Field.ParticleControl();
						particleControl.propertyType = Field.ParticlePropertyType.EXPEL_ANGLE;
						particleControl.initial = 0.0f;
						particleControl.velocity = 0.005f;
						particleControl.acceleration = 0.0002f;
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.velocityFrameRange = range;
						}
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.accelerationFrameRange = range;
						}
						emitter.particleControlVector.add(particleControl);
					}
					{
						Field.ParticleControl particleControl = new Field.ParticleControl();
						particleControl.propertyType = Field.ParticlePropertyType.EXPEL_DISTANCE;
						particleControl.initial = 0.0f;
						particleControl.velocity = 2.0f;
						particleControl.acceleration = 0.01f;
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.velocityFrameRange = range;
						}
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.accelerationFrameRange = range;
						}
						emitter.particleControlVector.add(particleControl);
					}
					{
						Field.ParticleControl particleControl = new Field.ParticleControl();
						particleControl.propertyType = Field.ParticlePropertyType.SIZE;
						particleControl.initial = 1.0f;
						particleControl.velocity = 0.03125f;
						particleControl.acceleration = 0.0f;
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.velocityFrameRange = range;
						}
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.accelerationFrameRange = range;
						}
						emitter.particleControlVector.add(particleControl);
					}
					{
						Field.ParticleControl particleControl = new Field.ParticleControl();
						particleControl.propertyType = Field.ParticlePropertyType.COLOR_RATIO;
						particleControl.initial = 0.0f;
						particleControl.velocity = 0.01f;
						particleControl.acceleration = 0.0f;
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.velocityFrameRange = range;
						}
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.accelerationFrameRange = range;
						}
						emitter.particleControlVector.add(particleControl);
					}
					{
						Field.ParticleControl particleControl = new Field.ParticleControl();
						particleControl.propertyType = Field.ParticlePropertyType.ALPHA;
						particleControl.initial = 1.0f;
						particleControl.velocity = -1.0f / 30.0f;
						particleControl.acceleration = 0.0f;
						{
							Field.Range range = new Field.Range();
							range.min = 60.0f;
							range.max = 90.0f;
							particleControl.velocityFrameRange = range;
						}
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.accelerationFrameRange = range;
						}
						emitter.particleControlVector.add(particleControl);
					}
				}
				
				field.emitterVector.add(emitter);
			}
		}
		
		return field;
	}
	
	public static Field createFieldPulse() {
		Field field = new Field();
		field.name = FIELD_PULSE_NAME;
		field.textureAtlasName = TEXTURE_ATLAS_NAME;
		{
			// emitters
			field.emitterVector = new Vector<Field.Emitter>();			
			{
				Field.Emitter emitter = new Field.Emitter();
				emitter.name = "example_emitter_A";
				emitter.textureRegionName = TEXTURE_ORB_NAME;
				emitter.x = 0.0f;
				emitter.y = 0.0f;
				emitter.shapeParameterA = 128.0f;
				emitter.shapeParameterB = 0.0f;
				emitter.globalAngle = 0.25f;
				{
					Field.Range range = new Field.Range();
					range.min = 0.25f;
					range.max = 0.25f;
					emitter.expelAngleRange = range;
				}
				{
					Field.Range range = new Field.Range();
					range.min = 1.0f;
					range.max = 1.0f;
					emitter.expelMultiplierRange = range;
				}
				{
					Field.Range range = new Field.Range();
					range.min = 16.0f;
					range.max = 16.0f;
					emitter.scaleMultiplierRange = range;
				}
				{
					Field.Range range = new Field.Range();
					range.min = 0.0f;
					range.max = 0.0f;
					emitter.rotationMultiplierRange = range;
				}
				{
					Field.Range range = new Field.Range();
					range.min = 0.5f;
					range.max = 1.0f;
					emitter.scaleFluxRange = range;
				}
				
				{
					Field.Color color = new Field.Color();
					color.r = 1.0f;
					color.g = 1.0f;
					color.b = 1.0f;
					emitter.color0 = color;
				}
				{
					Field.Color color = new Field.Color();
					color.r = 1.0f;
					color.g = 1.0f;
					color.b = 1.0f;
					emitter.color1 = color;
				}
				
				emitter.delayFrames = 30.0f;
				emitter.frameDuration = 0.0f;
				emitter.particleFrameLifetime = 600.0f;
				emitter.particleCount = 32;
				emitter.emitCount = 1;
				emitter.shapeType = Field.Emitter.ShapeType.CIRCLE;
				emitter.arrangeType = Field.Emitter.ArrangeType.RANDOM_FILL;
				
				{
					Vector<Field.Emitter.FlagType> flagTypeVector = new Vector<Field.Emitter.FlagType>();
					emitter.flagTypeVector = flagTypeVector;
					emitter.flagTypeVector.add(Field.Emitter.FlagType.REVERSE_ORDER);
					emitter.flagTypeVector.add(Field.Emitter.FlagType.ADDITIVE_BLEND);
				}
				
				{
					emitter.particleControlVector = new Vector<Field.ParticleControl>();
					{
						Field.ParticleControl particleControl = new Field.ParticleControl();
						particleControl.propertyType = Field.ParticlePropertyType.GLOBAL_DISTANCE;
						particleControl.initial = 0.0f;
						particleControl.velocity = 0.5f;
						particleControl.acceleration = 0.01f;
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.velocityFrameRange = range;
						}
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.accelerationFrameRange = range;
						}
						emitter.particleControlVector.add(particleControl);
					}
					{
						Field.ParticleControl particleControl = new Field.ParticleControl();
						particleControl.propertyType = Field.ParticlePropertyType.TANGENT_DISTANCE;
						particleControl.initial = 20.0f;
						particleControl.velocity = 0.0f;
						particleControl.acceleration = 0.0f;
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.velocityFrameRange = range;
						}
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.accelerationFrameRange = range;
						}
						emitter.particleControlVector.add(particleControl);
					}
					{
						Field.ParticleControl particleControl = new Field.ParticleControl();
						particleControl.propertyType = Field.ParticlePropertyType.TANGENT_FLUX;
						particleControl.initial = 0.25f;
						particleControl.velocity = 0.01f;
						particleControl.acceleration = 0.0f;
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.velocityFrameRange = range;
						}
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.accelerationFrameRange = range;
						}
						emitter.particleControlVector.add(particleControl);
					}
					{
						Field.ParticleControl particleControl = new Field.ParticleControl();
						particleControl.propertyType = Field.ParticlePropertyType.SCALE_X_FLUX;
						particleControl.initial = 0.25f;
						particleControl.velocity = 0.015f;
						particleControl.acceleration = 0.0f;
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.velocityFrameRange = range;
						}
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.accelerationFrameRange = range;
						}
						emitter.particleControlVector.add(particleControl);
					}
					{
						Field.ParticleControl particleControl = new Field.ParticleControl();
						particleControl.propertyType = Field.ParticlePropertyType.SCALE_Y_FLUX;
						particleControl.initial = 0.25f;
						particleControl.velocity = 0.015f;
						particleControl.acceleration = 0.0f;
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.velocityFrameRange = range;
						}
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.accelerationFrameRange = range;
						}
						emitter.particleControlVector.add(particleControl);
					}
					{
						Field.ParticleControl particleControl = new Field.ParticleControl();
						particleControl.propertyType = Field.ParticlePropertyType.ALPHA;
						particleControl.initial = 0.25f;
						particleControl.velocity = -0.25f / 15.0f;
						particleControl.acceleration = 0.0f;
						{
							Field.Range range = new Field.Range();
							range.min = 180.0f;
							range.max = 195.0f;
							particleControl.velocityFrameRange = range;
						}
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.accelerationFrameRange = range;
						}
						emitter.particleControlVector.add(particleControl);
					}
				}
				
				field.emitterVector.add(emitter);
			}
			{
				Field.Emitter emitter = new Field.Emitter();
				emitter.name = "example_emitter_B";
				emitter.textureRegionName = TEXTURE_ORB_NAME;
				emitter.x = -16.0f;
				emitter.y = 16.0f;
				emitter.shapeParameterA = 0.0f;
				emitter.shapeParameterB = 0.0f;
				emitter.globalAngle = 0.0f;
				{
					Field.Range range = new Field.Range();
					range.min = 0.0f;
					range.max = 0.0f;
					emitter.expelAngleRange = range;
				}
				{
					Field.Range range = new Field.Range();
					range.min = 1.0f;
					range.max = 1.0f;
					emitter.expelMultiplierRange = range;
				}
				{
					Field.Range range = new Field.Range();
					range.min = 128.0f;
					range.max = 128.0f;
					emitter.scaleMultiplierRange = range;
				}
				{
					Field.Range range = new Field.Range();
					range.min = 0.0f;
					range.max = 0.0f;
					emitter.rotationMultiplierRange = range;
				}
				{
					Field.Range range = new Field.Range();
					range.min = 0.75f;
					range.max = 1.0f;
					emitter.scaleFluxRange = range;
				}
				
				{
					Field.Color color = new Field.Color();
					color.r = 1.0f;
					color.g = 1.0f;
					color.b = 1.0f;
					emitter.color0 = color;
				}
				{
					Field.Color color = new Field.Color();
					color.r = 1.0f;
					color.g = 1.0f;
					color.b = 1.0f;
					emitter.color1 = color;
				}
				
				emitter.delayFrames = 120.0f;
				emitter.frameDuration = 0.0f;
				emitter.particleFrameLifetime = 120.0f;
				emitter.particleCount = 32;
				emitter.emitCount = 1;
				emitter.shapeType = Field.Emitter.ShapeType.POINT;
				emitter.arrangeType = Field.Emitter.ArrangeType.RANDOM_FILL;
				
				{
					Vector<Field.Emitter.FlagType> flagTypeVector = new Vector<Field.Emitter.FlagType>();
					emitter.flagTypeVector = flagTypeVector;
					emitter.flagTypeVector.add(Field.Emitter.FlagType.REVERSE_ORDER);
					emitter.flagTypeVector.add(Field.Emitter.FlagType.ADDITIVE_BLEND);
				}
				
				{
					emitter.particleControlVector = new Vector<Field.ParticleControl>();
					{
						Field.ParticleControl particleControl = new Field.ParticleControl();
						particleControl.propertyType = Field.ParticlePropertyType.SCALE_X_FLUX;
						particleControl.initial = 0.0f;
						particleControl.velocity = 1.0f / 120.0f;
						particleControl.acceleration = 0.0f;
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.velocityFrameRange = range;
						}
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.accelerationFrameRange = range;
						}
						emitter.particleControlVector.add(particleControl);
					}
					{
						Field.ParticleControl particleControl = new Field.ParticleControl();
						particleControl.propertyType = Field.ParticlePropertyType.SCALE_Y_FLUX;
						particleControl.initial = 0.25f;
						particleControl.velocity = 1.0f / 120.0f;
						particleControl.acceleration = 0.0f;
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.velocityFrameRange = range;
						}
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.accelerationFrameRange = range;
						}
						emitter.particleControlVector.add(particleControl);
					}
					{
						Field.ParticleControl particleControl = new Field.ParticleControl();
						particleControl.propertyType = Field.ParticlePropertyType.ALPHA;
						particleControl.initial = 0.15f;
						particleControl.velocity = 0.0f;
						particleControl.acceleration = 0.0f;
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 0.0f;
							particleControl.velocityFrameRange = range;
						}
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 0.0f;
							particleControl.accelerationFrameRange = range;
						}
						emitter.particleControlVector.add(particleControl);
					}
				}
				
				field.emitterVector.add(emitter);
			}
			{
				Field.Emitter emitter = new Field.Emitter();
				emitter.name = "example_emitter_C";
				emitter.textureRegionName = TEXTURE_ORB_NAME;
				emitter.x = 16.0f;
				emitter.y = -16.0f;
				emitter.shapeParameterA = 0.0f;
				emitter.shapeParameterB = 0.0f;
				emitter.globalAngle = 0.0f;
				{
					Field.Range range = new Field.Range();
					range.min = 0.0f;
					range.max = 0.0f;
					emitter.expelAngleRange = range;
				}
				{
					Field.Range range = new Field.Range();
					range.min = 1.0f;
					range.max = 1.0f;
					emitter.expelMultiplierRange = range;
				}
				{
					Field.Range range = new Field.Range();
					range.min = 128.0f;
					range.max = 128.0f;
					emitter.scaleMultiplierRange = range;
				}
				{
					Field.Range range = new Field.Range();
					range.min = 0.0f;
					range.max = 0.0f;
					emitter.rotationMultiplierRange = range;
				}
				{
					Field.Range range = new Field.Range();
					range.min = 0.75f;
					range.max = 1.0f;
					emitter.scaleFluxRange = range;
				}
				
				{
					Field.Color color = new Field.Color();
					color.r = 1.0f;
					color.g = 1.0f;
					color.b = 1.0f;
					emitter.color0 = color;
				}
				{
					Field.Color color = new Field.Color();
					color.r = 1.0f;
					color.g = 1.0f;
					color.b = 1.0f;
					emitter.color1 = color;
				}
				
				emitter.delayFrames = 120.0f;
				emitter.frameDuration = 0.0f;
				emitter.particleFrameLifetime = 120.0f;
				emitter.particleCount = 32;
				emitter.emitCount = 1;
				emitter.shapeType = Field.Emitter.ShapeType.POINT;
				emitter.arrangeType = Field.Emitter.ArrangeType.RANDOM_FILL;
				
				{
					Vector<Field.Emitter.FlagType> flagTypeVector = new Vector<Field.Emitter.FlagType>();
					emitter.flagTypeVector = flagTypeVector;
					emitter.flagTypeVector.add(Field.Emitter.FlagType.REVERSE_ORDER);
					emitter.flagTypeVector.add(Field.Emitter.FlagType.ADDITIVE_BLEND);
				}
				
				{
					emitter.particleControlVector = new Vector<Field.ParticleControl>();
					{
						Field.ParticleControl particleControl = new Field.ParticleControl();
						particleControl.propertyType = Field.ParticlePropertyType.SCALE_X_FLUX;
						particleControl.initial = 0.5f;
						particleControl.velocity = 1.0f / 120.0f;
						particleControl.acceleration = 0.0f;
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.velocityFrameRange = range;
						}
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.accelerationFrameRange = range;
						}
						emitter.particleControlVector.add(particleControl);
					}
					{
						Field.ParticleControl particleControl = new Field.ParticleControl();
						particleControl.propertyType = Field.ParticlePropertyType.SCALE_Y_FLUX;
						particleControl.initial = 0.75f;
						particleControl.velocity = 1.0f / 120.0f;
						particleControl.acceleration = 0.0f;
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.velocityFrameRange = range;
						}
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 600.0f;
							particleControl.accelerationFrameRange = range;
						}
						emitter.particleControlVector.add(particleControl);
					}
					{
						Field.ParticleControl particleControl = new Field.ParticleControl();
						particleControl.propertyType = Field.ParticlePropertyType.ALPHA;
						particleControl.initial = 0.15f;
						particleControl.velocity = 0.0f;
						particleControl.acceleration = 0.0f;
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 0.0f;
							particleControl.velocityFrameRange = range;
						}
						{
							Field.Range range = new Field.Range();
							range.min = 0.0f;
							range.max = 0.0f;
							particleControl.accelerationFrameRange = range;
						}
						emitter.particleControlVector.add(particleControl);
					}
				}
				
				field.emitterVector.add(emitter);
			}
		}
		
		return field;
	}
	
	public static TextureAtlas createTextureAtlas() {				
		Vector<Region> regionVector = new Vector<Region>();
		{
			Region region = new Region();
			region.texture = createDropletTexture();
			region.x = 0;
			region.y = 0;
			regionVector.add(region);
		}
		{
			Region region = new Region();
			region.texture = createStarTexture();
			region.x = 32;
			region.y = 0;
			regionVector.add(region);
		}
		{
			Region region = new Region();
			region.texture = createOrbTexture();
			region.x = 0;
			region.y = 32;
			regionVector.add(region);
		}
		
		TextureAtlas textureAtlas = new TextureAtlas();
		textureAtlas.name = TEXTURE_ATLAS_NAME;
		textureAtlas.width = TEXTURE_ATLAS_WIDTH;
		textureAtlas.height = TEXTURE_ATLAS_HEIGHT;
		textureAtlas.regionVector = regionVector;
		
		return textureAtlas;
	}
	
	public static Texture createDropletTexture() {
		BufferedImage bufferedImage = new BufferedImage(TEXTURE_WIDTH, TEXTURE_HEIGHT, BufferedImage.TYPE_INT_ARGB);
		Graphics graphics = bufferedImage.getGraphics();
		
		Graphics2D graphics2D = (Graphics2D)graphics;
		graphics2D.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		
		Point2D center = new Point2D.Float(16.0f, 16.0f);
		float radius = 20.0f;
		float[] distances = {0.0f, 1.0f};
		Color[] colors = {new Color(255, 255, 255, 255), new Color(0, 0, 0, 255)};
		RadialGradientPaint paint = new RadialGradientPaint(center, radius, distances, colors);
		
		Ellipse2D shape = new Ellipse2D.Float(4.0f, 4.0f, 24.0f, 24.0f);
		
		graphics2D.setPaint(paint);
		graphics2D.fill(shape);
		
		Texture texture = TextureUtility.fromBufferedImage(bufferedImage);
		texture.name = TEXTURE_DROPLET_NAME;
		
		return texture;
	}
	
	public static Texture createStarTexture() {
		BufferedImage bufferedImage = new BufferedImage(TEXTURE_WIDTH, TEXTURE_HEIGHT, BufferedImage.TYPE_INT_ARGB);
		Graphics graphics = bufferedImage.getGraphics();
		
		Graphics2D graphics2D = (Graphics2D)graphics;
		graphics2D.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		
		final double CENTER_X = 16.0;
		final double CENTER_Y = 16.0;
		final double OUTER_RADIUS = 14.0;
		final double INNER_RADIUS = 6.0;
		final int ARM_COUNT = 5;
		final double INITIAL_ANGLE = -Math.PI / 2.0;
		final double ARM_ANGLE_STEP = (Math.PI * 2.0) / ((double)ARM_COUNT);
		final int POINT_COUNT = ARM_COUNT * 2;		
		
		Point2D[] pointArray = new Point2D[POINT_COUNT];
		double angleOuter = INITIAL_ANGLE;
		double angleInner = INITIAL_ANGLE + (ARM_ANGLE_STEP / 2.0);		
		for (int armIndex = 0; armIndex < ARM_COUNT; ++armIndex) {		
			double outerX = Math.cos(angleOuter) * OUTER_RADIUS;
			double outerY = Math.sin(angleOuter) * OUTER_RADIUS;
			double innerX = Math.cos(angleInner) * INNER_RADIUS;
			double innerY = Math.sin(angleInner) * INNER_RADIUS;
			
			Point2D pointOuter = new Point2D.Double(outerX + CENTER_X, outerY + CENTER_Y);
			Point2D pointInner = new Point2D.Double(innerX + CENTER_X, innerY + CENTER_Y);
			int pointIndex = armIndex * 2;
			pointArray[pointIndex + 0] = pointOuter;
			pointArray[pointIndex + 1] = pointInner;
			
			angleInner += ARM_ANGLE_STEP;
			angleOuter += ARM_ANGLE_STEP;
		}
		
		GeneralPath path = new GeneralPath();
		Point2D point0 = pointArray[0];
		path.moveTo(point0.getX(), point0.getY());
		for (int pointIndex = 1; pointIndex < POINT_COUNT; ++pointIndex) {
			Point2D point = pointArray[pointIndex];
			path.lineTo(point.getX(), point.getY());
		}
		path.closePath();
		
		Stroke stroke = new BasicStroke(1.0f);
		Color strokePaint = Color.BLACK;
		Color fillPaint = Color.WHITE;
		
		graphics2D.setPaint(fillPaint);
		graphics2D.fill(path);
		
		graphics2D.setStroke(stroke);
		graphics2D.setPaint(strokePaint);
		graphics2D.draw(path);		
		
		Texture texture = TextureUtility.fromBufferedImage(bufferedImage);
		texture.name = TEXTURE_STAR_NAME;
		
		return texture;
	}
	
	public static Texture createOrbTexture() {
		BufferedImage bufferedImage = new BufferedImage(TEXTURE_WIDTH, TEXTURE_HEIGHT, BufferedImage.TYPE_INT_ARGB);
		Graphics graphics = bufferedImage.getGraphics();
		
		Graphics2D graphics2D = (Graphics2D)graphics;
		graphics2D.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		
		Point2D center = new Point2D.Float(16.0f, 16.0f);
		float radius = 20.0f;
		float[] distances = {0.0f, 1.0f};
		Color[] colors = {new Color(255, 255, 255, 0), new Color(255, 255, 255, 255)};
		RadialGradientPaint paint = new RadialGradientPaint(center, radius, distances, colors);
		
		Ellipse2D shape = new Ellipse2D.Float(4.0f, 4.0f, 24.0f, 24.0f);
		
		graphics2D.setPaint(paint);
		graphics2D.fill(shape);
		
		Texture texture = TextureUtility.fromBufferedImage(bufferedImage);
		texture.name = TEXTURE_ORB_NAME;
		
		return texture;
	}
	
	public static Manifest createManifest(String directoryPath) {
		Manifest manifest = new Manifest();
		manifest.name = MANIFEST_NAME;
		{
			manifest.assetEntryVector = new Vector<Manifest.AssetEntry>();
			{
				{
					String fileName = FIELD_SPRAY_NAME + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = Constants.GROUP_CODE;
					assetEntry.typeCode = Constants.FIELD_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);					
					manifest.assetEntryVector.add(assetEntry);
				}
				
				{
					String fileName = FIELD_ORBIT_NAME + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = Constants.GROUP_CODE;
					assetEntry.typeCode = Constants.FIELD_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);					
					manifest.assetEntryVector.add(assetEntry);
				}
				
				{
					String fileName = FIELD_PULSE_NAME + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = Constants.GROUP_CODE;
					assetEntry.typeCode = Constants.FIELD_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);					
					manifest.assetEntryVector.add(assetEntry);
				}

				{
					String fileName = TEXTURE_ATLAS_NAME + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = lct.imag.Constants.GROUP_CODE;
					assetEntry.typeCode = lct.imag.Constants.TEXTURE_ATLAS_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);
					manifest.assetEntryVector.add(assetEntry);
				}
			}
		}
		return manifest;
	}
	
}

