package lct.part;

import lct.spri.Animation;
import lct.util.PathUtility;
import lct.util.XMLUtility;

import java.util.Vector;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

public class XMLConverter {
	public XMLConverter() {		
	}
	
	public void storeField(Field field, String filePath) {
		try {
			Document document = XMLUtility.createDocument();
			
			Element fieldElement = document.createElement("part:field");
			{
				XMLUtility.writeStringElement(document, "textureAtlasName", field.textureAtlasName, fieldElement);
				
				Element emittersElement = document.createElement("emitters");
				for (Field.Emitter emitter : field.emitterVector) {
					Element emitterElement = document.createElement("emitter");
					emitterElement.setAttribute("name", emitter.name);
					{
						XMLUtility.writeStringElement(document, "textureRegionName", emitter.textureRegionName, emitterElement);
						XMLUtility.writeFloatElement(document, "x", emitter.x, emitterElement);
						XMLUtility.writeFloatElement(document, "y", emitter.y, emitterElement);
						XMLUtility.writeFloatElement(document, "shapeParameterA", emitter.shapeParameterA, emitterElement);
						XMLUtility.writeFloatElement(document, "shapeParameterB", emitter.shapeParameterB, emitterElement);
						XMLUtility.writeFloatElement(document, "globalAngle", emitter.globalAngle, emitterElement);
						
						{
							Field.Range range = emitter.expelAngleRange;
							Element rangeElement = document.createElement("expelAngleRange");
							XMLUtility.writeFloatElement(document, "min", range.min, rangeElement);
							XMLUtility.writeFloatElement(document, "max", range.max, rangeElement);
							emitterElement.appendChild(rangeElement);
						}
						{
							Field.Range range = emitter.expelMultiplierRange;
							Element rangeElement = document.createElement("expelMultiplierRange");
							XMLUtility.writeFloatElement(document, "min", range.min, rangeElement);
							XMLUtility.writeFloatElement(document, "max", range.max, rangeElement);
							emitterElement.appendChild(rangeElement);
						}
						{
							Field.Range range = emitter.scaleMultiplierRange;
							Element rangeElement = document.createElement("scaleMultiplierRange");
							XMLUtility.writeFloatElement(document, "min", range.min, rangeElement);
							XMLUtility.writeFloatElement(document, "max", range.max, rangeElement);
							emitterElement.appendChild(rangeElement);
						}
						{
							Field.Range range = emitter.rotationMultiplierRange;
							Element rangeElement = document.createElement("rotationMultiplierRange");
							XMLUtility.writeFloatElement(document, "min", range.min, rangeElement);
							XMLUtility.writeFloatElement(document, "max", range.max, rangeElement);
							emitterElement.appendChild(rangeElement);
						}
						{
							Field.Range range = emitter.scaleFluxRange;
							Element rangeElement = document.createElement("scaleFluxRange");
							XMLUtility.writeFloatElement(document, "min", range.min, rangeElement);
							XMLUtility.writeFloatElement(document, "max", range.max, rangeElement);
							emitterElement.appendChild(rangeElement);
						}
						
						{
							Field.Color color = emitter.color0;
							Element colorElement = document.createElement("color0");
							XMLUtility.writeFloatElement(document, "r", color.r, colorElement);
							XMLUtility.writeFloatElement(document, "g", color.g, colorElement);
							XMLUtility.writeFloatElement(document, "b", color.b, colorElement);
							emitterElement.appendChild(colorElement);
						}
						{
							Field.Color color = emitter.color1;
							Element colorElement = document.createElement("color1");
							XMLUtility.writeFloatElement(document, "r", color.r, colorElement);
							XMLUtility.writeFloatElement(document, "g", color.g, colorElement);
							XMLUtility.writeFloatElement(document, "b", color.b, colorElement);
							emitterElement.appendChild(colorElement);
						}
						
						XMLUtility.writeFloatElement(document, "delayFrames", emitter.delayFrames, emitterElement);
						XMLUtility.writeFloatElement(document, "frameDuration", emitter.frameDuration, emitterElement);
						XMLUtility.writeFloatElement(document, "particleFrameLifetime", emitter.particleFrameLifetime, emitterElement);
						XMLUtility.writeIntegerElement(document, "particleCount", emitter.particleCount, emitterElement);
						XMLUtility.writeIntegerElement(document, "emitCount", emitter.emitCount, emitterElement);
						XMLUtility.writeStringElement(document, "shapeType", emitter.shapeType.toString(), emitterElement);
						XMLUtility.writeStringElement(document, "arrangeType", emitter.arrangeType.toString(), emitterElement);
						
						Element flagTypesElement = document.createElement("flagTypes");
						for (Field.Emitter.FlagType flagType : emitter.flagTypeVector) {
							XMLUtility.writeStringElement(document, "flagType", flagType.toString(), flagTypesElement);
						}
						emitterElement.appendChild(flagTypesElement);
						
						Element particleControlsElement = document.createElement("particleControls");
						for (Field.ParticleControl particleControl : emitter.particleControlVector) {
							Element particleControlElement = document.createElement("particleControl");
							{
								XMLUtility.writeStringElement(document, "propertyType", particleControl.propertyType.toString(), particleControlElement);
								XMLUtility.writeFloatElement(document, "initial", particleControl.initial, particleControlElement);
								XMLUtility.writeFloatElement(document, "velocity", particleControl.velocity, particleControlElement);
								XMLUtility.writeFloatElement(document, "acceleration", particleControl.acceleration, particleControlElement);
								
								{
									Field.Range range = particleControl.velocityFrameRange;
									Element rangeElement = document.createElement("velocityFrameRange");
									XMLUtility.writeFloatElement(document, "min", range.min, rangeElement);
									XMLUtility.writeFloatElement(document, "max", range.max, rangeElement);
									particleControlElement.appendChild(rangeElement);
								}
								{
									Field.Range range = particleControl.accelerationFrameRange;
									Element rangeElement = document.createElement("accelerationFrameRange");
									XMLUtility.writeFloatElement(document, "min", range.min, rangeElement);
									XMLUtility.writeFloatElement(document, "max", range.max, rangeElement);
									particleControlElement.appendChild(rangeElement);
								}
							}
							particleControlsElement.appendChild(particleControlElement);
						}
						emitterElement.appendChild(particleControlsElement);												
					}
					emittersElement.appendChild(emitterElement);
				}
				fieldElement.appendChild(emittersElement);
			}
			document.appendChild(fieldElement);
			
			XMLUtility.storeDocument(document, filePath);
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			System.out.println(e.getStackTrace());
		}
	}
	
	public Field loadField(String filePath) {
		try {
			
			Document document = XMLUtility.loadDocument(filePath);
			
			Element fieldElement = (Element)document.getElementsByTagName("part:field").item(0);
			Field field = new Field();
			field.name = PathUtility.getFileNameNoExtension(filePath);
			{
				field.textureAtlasName = XMLUtility.readStringElement(fieldElement, "textureAtlasName", 0);
				
				Element emittersElement = (Element)fieldElement.getElementsByTagName("emitters").item(0);
				NodeList emitterNodeList = emittersElement.getElementsByTagName("emitter");
				field.emitterVector = new Vector<Field.Emitter>();
				for (int emitterIndex = 0; emitterIndex < emitterNodeList.getLength(); ++emitterIndex) {
					Element emitterElement = (Element)emitterNodeList.item(emitterIndex);
					Field.Emitter emitter = new Field.Emitter();
					emitter.name = emitterElement.getAttribute("name");
					{												
						emitter.textureRegionName = XMLUtility.readStringElement(emitterElement, "textureRegionName", 0);
						emitter.x = XMLUtility.readFloatElement(emitterElement, "x", 0);
						emitter.y = XMLUtility.readFloatElement(emitterElement, "y", 0);
						emitter.shapeParameterA = XMLUtility.readFloatElement(emitterElement, "shapeParameterA", 0);
						emitter.shapeParameterB = XMLUtility.readFloatElement(emitterElement, "shapeParameterB", 0);
						emitter.globalAngle = XMLUtility.readFloatElement(emitterElement, "globalAngle", 0);
						
						{
							Element rangeElement = (Element)emitterElement.getElementsByTagName("expelAngleRange").item(0);
							Field.Range range = new Field.Range();
							range.min = XMLUtility.readFloatElement(rangeElement, "min", 0);
							range.max = XMLUtility.readFloatElement(rangeElement, "max", 0);
							emitter.expelAngleRange = range;
						}
						{
							Element rangeElement = (Element)emitterElement.getElementsByTagName("expelMultiplierRange").item(0);
							Field.Range range = new Field.Range();
							range.min = XMLUtility.readFloatElement(rangeElement, "min", 0);
							range.max = XMLUtility.readFloatElement(rangeElement, "max", 0);
							emitter.expelMultiplierRange = range;
						}
						{
							Element rangeElement = (Element)emitterElement.getElementsByTagName("scaleMultiplierRange").item(0);
							Field.Range range = new Field.Range();
							range.min = XMLUtility.readFloatElement(rangeElement, "min", 0);
							range.max = XMLUtility.readFloatElement(rangeElement, "max", 0);
							emitter.scaleMultiplierRange = range;
						}
						{
							Element rangeElement = (Element)emitterElement.getElementsByTagName("rotationMultiplierRange").item(0);
							Field.Range range = new Field.Range();
							range.min = XMLUtility.readFloatElement(rangeElement, "min", 0);
							range.max = XMLUtility.readFloatElement(rangeElement, "max", 0);
							emitter.rotationMultiplierRange = range;
						}
						{
							Element rangeElement = (Element)emitterElement.getElementsByTagName("scaleFluxRange").item(0);
							Field.Range range = new Field.Range();
							range.min = XMLUtility.readFloatElement(rangeElement, "min", 0);
							range.max = XMLUtility.readFloatElement(rangeElement, "max", 0);
							emitter.scaleFluxRange = range;
						}
						
						{
							Element colorElement = (Element)emitterElement.getElementsByTagName("color0").item(0);
							Field.Color color = new Field.Color();
							color.r = XMLUtility.readFloatElement(colorElement, "r", 0);
							color.g = XMLUtility.readFloatElement(colorElement, "g", 0);
							color.b = XMLUtility.readFloatElement(colorElement, "b", 0);
							emitter.color0 = color;
						}
						{
							Element colorElement = (Element)emitterElement.getElementsByTagName("color1").item(0);
							Field.Color color = new Field.Color();
							color.r = XMLUtility.readFloatElement(colorElement, "r", 0);
							color.g = XMLUtility.readFloatElement(colorElement, "g", 0);
							color.b = XMLUtility.readFloatElement(colorElement, "b", 0);
							emitter.color1 = color;
						}
						
						emitter.delayFrames = XMLUtility.readFloatElement(emitterElement, "delayFrames", 0);
						emitter.frameDuration = XMLUtility.readFloatElement(emitterElement, "frameDuration", 0);
						emitter.particleFrameLifetime = XMLUtility.readFloatElement(emitterElement, "particleFrameLifetime", 0);
						emitter.particleCount = XMLUtility.readIntegerElement(emitterElement, "particleCount", 0);
						emitter.emitCount = XMLUtility.readIntegerElement(emitterElement, "emitCount", 0);
						String shapeTypeString = XMLUtility.readStringElement(emitterElement, "shapeType", 0);
						emitter.shapeType = Field.Emitter.ShapeType.valueOf(shapeTypeString);
						String arrangeTypeString = XMLUtility.readStringElement(emitterElement, "arrangeType", 0);
						emitter.arrangeType = Field.Emitter.ArrangeType.valueOf(arrangeTypeString);
						
						Element flagTypesElement = (Element)fieldElement.getElementsByTagName("flagTypes").item(0);
						NodeList flagTypeNodeList = flagTypesElement.getElementsByTagName("flagType");
						emitter.flagTypeVector = new Vector<Field.Emitter.FlagType>();
						for (int flagTypeIndex = 0; flagTypeIndex < flagTypeNodeList.getLength(); ++flagTypeIndex) {
							String flagTypeString = XMLUtility.readStringElement(flagTypesElement, "flagType", flagTypeIndex);
							Field.Emitter.FlagType flagType = Field.Emitter.FlagType.valueOf(flagTypeString);
							emitter.flagTypeVector.add(flagType);
						}
						
						Element particleControlsElement = (Element)emitterElement.getElementsByTagName("particleControls").item(0);
						NodeList particleControlNodeList = particleControlsElement.getElementsByTagName("particleControl");
						emitter.particleControlVector = new Vector<Field.ParticleControl>();
						for (int particleControlIndex = 0; particleControlIndex < particleControlNodeList.getLength(); ++particleControlIndex) {
							Element particleControlElement = (Element)particleControlNodeList.item(particleControlIndex);
							Field.ParticleControl particleControl = new Field.ParticleControl();
							{
								String propertyTypeString = XMLUtility.readStringElement(particleControlElement, "propertyType", 0);
								particleControl.propertyType = Field.ParticlePropertyType.valueOf(propertyTypeString);
								particleControl.initial = XMLUtility.readFloatElement(particleControlElement, "initial", 0);
								particleControl.velocity = XMLUtility.readFloatElement(particleControlElement, "velocity", 0);
								particleControl.acceleration = XMLUtility.readFloatElement(particleControlElement, "acceleration", 0);
								{
									Element rangeElement = (Element)particleControlElement.getElementsByTagName("velocityFrameRange").item(0);
									Field.Range range = new Field.Range();
									range.min = XMLUtility.readFloatElement(rangeElement, "min", 0);
									range.max = XMLUtility.readFloatElement(rangeElement, "max", 0);
									particleControl.velocityFrameRange = range;
								}
								{
									Element rangeElement = (Element)particleControlElement.getElementsByTagName("accelerationFrameRange").item(0);
									Field.Range range = new Field.Range();
									range.min = XMLUtility.readFloatElement(rangeElement, "min", 0);
									range.max = XMLUtility.readFloatElement(rangeElement, "max", 0);
									particleControl.accelerationFrameRange = range;
								}
							}
							emitter.particleControlVector.add(particleControl);
						}
					}
					field.emitterVector.add(emitter);
				}
			}
			return field;
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			//System.out.println(e.getStackTrace());
			e.printStackTrace();
			
			return null;
		}
	}
}
