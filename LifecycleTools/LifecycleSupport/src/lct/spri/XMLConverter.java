package lct.spri;

import lct.util.PathUtility;
import lct.util.XMLUtility;

import java.util.Vector;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

public class XMLConverter {
	public XMLConverter() {		
	}
	
	public void storeFigure(Figure figure, String filePath) {
		try {
			Document document = XMLUtility.createDocument();
			
			Element figureElement = document.createElement("spri:figure");
			{
				Element partsElement = document.createElement("parts");
				for (Figure.Part part : figure.partVector) {
					Element partElement = document.createElement("part");
					partElement.setAttribute("name", part.name);
					{
						Figure.Part.Placement placement = part.placement;
						Element placementElement = document.createElement("placement");
						{
							XMLUtility.writeFloatElement(document, "width", placement.width, placementElement);
							XMLUtility.writeFloatElement(document, "height", placement.height, placementElement);
							XMLUtility.writeFloatElement(document, "x", placement.x, placementElement);
							XMLUtility.writeFloatElement(document, "y", placement.y, placementElement);
						}
						partElement.appendChild(placementElement);
						
						Element propertiesElement = document.createElement("properties");
						for (int partPropertyIndex = 0; partPropertyIndex < Figure.Part.PropertyType.values().length; ++partPropertyIndex) {
							float partProperty = part.propertyArray[partPropertyIndex];
							Figure.Part.PropertyType partPropertyType = Figure.Part.PropertyType.values()[partPropertyIndex];
							Element propertyElement = XMLUtility.writeFloatElement(document, "property", partProperty, propertiesElement);
							propertyElement.setAttribute("type", partPropertyType.toString());
							propertiesElement.appendChild(propertyElement);
						}
						partElement.appendChild(propertiesElement);
						
						XMLUtility.writeStringElement(document, "jointName", part.jointName, partElement);
						XMLUtility.writeStringElement(document, "patternName", part.patternName, partElement);
					}
					partsElement.appendChild(partElement);
				}
				figureElement.appendChild(partsElement);
				
				Element jointsElement = document.createElement("joints");
				for (Figure.Joint joint : figure.jointVector) {
					Element jointElement = document.createElement("joint");
					jointElement.setAttribute("name", joint.name);
					{
						Element propertiesElement = document.createElement("properties");
						for (int jointPropertyIndex = 0; jointPropertyIndex < Figure.Joint.PropertyType.values().length; ++jointPropertyIndex) {
							float jointProperty = joint.propertyArray[jointPropertyIndex];
							Figure.Joint.PropertyType jointPropertyType = Figure.Joint.PropertyType.values()[jointPropertyIndex];
							Element propertyElement = XMLUtility.writeFloatElement(document, "property", jointProperty, propertiesElement);
							propertyElement.setAttribute("type", jointPropertyType.toString());
							propertiesElement.appendChild(propertyElement);
						}
						jointElement.appendChild(propertiesElement);
						
						Element flagsElement = document.createElement("flags");
						for (int jointFlagIndex = 0; jointFlagIndex < Figure.Joint.FlagType.values().length; ++jointFlagIndex) {
							boolean jointFlag = joint.flagArray[jointFlagIndex];
							Figure.Joint.FlagType jointFlagType = Figure.Joint.FlagType.values()[jointFlagIndex];
							Element flagElement = XMLUtility.writeBooleanElement(document, "flag", jointFlag, flagsElement);
							flagElement.setAttribute("type", jointFlagType.toString());
							flagsElement.appendChild(flagElement);
						}
						jointElement.appendChild(flagsElement);
						
						XMLUtility.writeStringElement(document, "parentName", joint.parentName, jointElement);
					}
					jointsElement.appendChild(jointElement);
				}
				figureElement.appendChild(jointsElement);
				
				Element patternsElement = document.createElement("patterns");
				for (Figure.Pattern pattern : figure.patternVector) {
					Element patternElement = document.createElement("pattern");
					patternElement.setAttribute("name", pattern.name);
					{
						XMLUtility.writeStringElement(document, "textureTableName", pattern.textureTableName, patternElement);
						
						Element propertiesElement = document.createElement("properties");
						for (int patternPropertyIndex = 0; patternPropertyIndex < Figure.Pattern.PropertyType.values().length; ++patternPropertyIndex) {
							float patternProperty = pattern.propertyArray[patternPropertyIndex];
							Figure.Pattern.PropertyType patternPropertyType = Figure.Pattern.PropertyType.values()[patternPropertyIndex];
							Element propertyElement = XMLUtility.writeFloatElement(document, "property", patternProperty, propertiesElement);
							propertyElement.setAttribute("type", patternPropertyType.toString());
							propertiesElement.appendChild(propertyElement);
						}
						patternElement.appendChild(propertiesElement);
					}
					patternsElement.appendChild(patternElement);
				}
				figureElement.appendChild(patternsElement);
			}
			document.appendChild(figureElement);
			
			XMLUtility.storeDocument(document, filePath);
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			System.out.println(e.getStackTrace());
		}
	}
	
	public Figure loadFigure(String filePath) {
		try {
			Document document = XMLUtility.loadDocument(filePath);
			
			Element figureElement = (Element)document.getElementsByTagName("spri:figure").item(0);
			Figure figure = new Figure();
			figure.name = PathUtility.getFileNameNoExtension(filePath);
			{
				Element partsElement = (Element)figureElement.getElementsByTagName("parts").item(0);
				NodeList partNodeList = partsElement.getElementsByTagName("part");
				figure.partVector = new Vector<Figure.Part>();
				for (int partIndex = 0; partIndex < partNodeList.getLength(); ++partIndex) {
					Element partElement = (Element)partNodeList.item(partIndex);
					Figure.Part part = new Figure.Part();
					part.name = partElement.getAttribute("name");
					{
						Element placementElement = (Element)partElement.getElementsByTagName("placement").item(0);
						Figure.Part.Placement placement = new Figure.Part.Placement();
						placement.width = XMLUtility.readFloatElement(placementElement, "width", 0);
						placement.height = XMLUtility.readFloatElement(placementElement, "height", 0);
						placement.x = XMLUtility.readFloatElement(placementElement, "x", 0);
						placement.y = XMLUtility.readFloatElement(placementElement, "y", 0);
						part.placement = placement;
						
						Element propertiesElement = (Element)partElement.getElementsByTagName("properties").item(0);
						NodeList propertyNodeList = propertiesElement.getElementsByTagName("property");
						part.propertyArray = new float[Figure.Part.PropertyType.values().length];
						for (int partPropertyIndex = 0; partPropertyIndex < propertyNodeList.getLength(); ++partPropertyIndex) {
							float partProperty = XMLUtility.readFloatElement(propertiesElement, "property", partPropertyIndex);
							part.propertyArray[partPropertyIndex] = partProperty;
						}
						
						part.jointName = XMLUtility.readStringElement(partElement, "jointName", 0);
						part.patternName = XMLUtility.readStringElement(partElement, "patternName", 0);
					}
					figure.partVector.add(part);
				}
				
				Element jointsElement = (Element)figureElement.getElementsByTagName("joints").item(0);
				NodeList jointNodeList = jointsElement.getElementsByTagName("joint");
				figure.jointVector = new Vector<Figure.Joint>();
				for (int jointIndex = 0; jointIndex < jointNodeList.getLength(); ++jointIndex) {
					Element jointElement = (Element)jointNodeList.item(jointIndex);
					Figure.Joint joint = new Figure.Joint();
					joint.name = jointElement.getAttribute("name");
					{
						Element propertiesElement = (Element)jointElement.getElementsByTagName("properties").item(0);
						NodeList propertyNodeList = propertiesElement.getElementsByTagName("property");
						joint.propertyArray = new float[Figure.Joint.PropertyType.values().length];
						for (int jointPropertyIndex = 0; jointPropertyIndex < propertyNodeList.getLength(); ++jointPropertyIndex) {
							float jointProperty = XMLUtility.readFloatElement(propertiesElement, "property", jointPropertyIndex);
							joint.propertyArray[jointPropertyIndex] = jointProperty;
						}
						
						Element flagsElement = (Element)jointElement.getElementsByTagName("flags").item(0);
						NodeList flagNodeList = flagsElement.getElementsByTagName("flag");
						joint.flagArray = new boolean[Figure.Joint.FlagType.values().length];
						for (int jointFlagIndex = 0; jointFlagIndex < flagNodeList.getLength(); ++jointFlagIndex) {
							boolean jointFlag = XMLUtility.readBooleanElement(flagsElement, "flag", jointFlagIndex);
							joint.flagArray[jointFlagIndex] = jointFlag;
						}
						
						joint.parentName = XMLUtility.readStringElement(jointElement, "parentName", 0);
					}
					figure.jointVector.add(joint);
				}
				
				Element patternsElement = (Element)figureElement.getElementsByTagName("patterns").item(0);
				NodeList patternNodeList = patternsElement.getElementsByTagName("pattern");
				figure.patternVector = new Vector<Figure.Pattern>();
				for (int patternIndex = 0; patternIndex < patternNodeList.getLength(); ++patternIndex) {
					Element patternElement = (Element)patternNodeList.item(patternIndex);
					Figure.Pattern pattern = new Figure.Pattern();
					pattern.name = patternElement.getAttribute("name");
					{
						pattern.textureTableName = XMLUtility.readStringElement(patternElement, "textureTableName", 0);
						
						Element propertiesElement = (Element)patternElement.getElementsByTagName("properties").item(0);
						NodeList propertyNodeList = propertiesElement.getElementsByTagName("property");
						pattern.propertyArray = new float[Figure.Pattern.PropertyType.values().length];
						for (int patternPropertyIndex = 0; patternPropertyIndex < propertyNodeList.getLength(); ++patternPropertyIndex) {
							float patternProperty = XMLUtility.readFloatElement(propertiesElement, "property", patternPropertyIndex);
							pattern.propertyArray[patternPropertyIndex] = patternProperty;
						}
					}
					figure.patternVector.add(pattern);
				}
			}
			return figure;
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			System.out.println(e.getStackTrace());
			
			return null;
		}
	}
	
	public void storeAnimation(Animation animation, String filePath) {
		try {
			Document document = XMLUtility.createDocument();
			
			Element animationElement = document.createElement("spri:animation");
			{
				Element tracksElement = document.createElement("tracks");
				for (Animation.Track track : animation.trackVector) {
					Element trackElement = document.createElement("track");
					{						
						XMLUtility.writeStringElement(document, "targetName", track.targetName, trackElement);
						XMLUtility.writeStringElement(document, "targetType", track.targetType.toString(), trackElement);
						XMLUtility.writeStringElement(document, "targetPropertyType", track.targetPropertyType.toString(), trackElement);
						XMLUtility.writeStringElement(document, "finishType", track.finishType.toString(), trackElement);
						
						Element keysElement = document.createElement("keys");
						for (Animation.Track.Key key : track.keyVector) {
							Element keyElement = document.createElement("key");
							{
								XMLUtility.writeFloatElement(document, "frame", key.frame, keyElement);
								XMLUtility.writeFloatElement(document, "value", key.value, keyElement);
								XMLUtility.writeFloatElement(document, "slope", key.slope, keyElement);
								XMLUtility.writeStringElement(document, "slopeCalc", key.slopeCalc.toString(), keyElement);
								XMLUtility.writeStringElement(document, "curveType", key.curveType.toString(), keyElement);
							}
							keysElement.appendChild(keyElement);
						}
						trackElement.appendChild(keysElement);
					}
					tracksElement.appendChild(trackElement);
				}
				animationElement.appendChild(tracksElement);
			}
			document.appendChild(animationElement);
			
			XMLUtility.storeDocument(document, filePath);
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			System.out.println(e.getStackTrace());
		}
	}
	
	public Animation loadAnimation(String filePath) {
		try {
			Document document = XMLUtility.loadDocument(filePath);
			
			Element animationElement = (Element)document.getElementsByTagName("spri:animation").item(0);
			Animation animation = new Animation();
			animation.name = PathUtility.getFileNameNoExtension(filePath);
			{
				Element tracksElement = (Element)animationElement.getElementsByTagName("tracks").item(0);
				NodeList trackNodeList = tracksElement.getElementsByTagName("track");
				animation.trackVector = new Vector<Animation.Track>();
				for (int trackIndex = 0; trackIndex < trackNodeList.getLength(); ++trackIndex) {
					Element trackElement = (Element)trackNodeList.item(trackIndex);
					Animation.Track track = new Animation.Track();
					{
						track.targetName = XMLUtility.readStringElement(trackElement, "targetName", 0);
						String targetTypeString = XMLUtility.readStringElement(trackElement, "targetType", 0);
						track.targetType = Animation.Track.TargetType.valueOf(targetTypeString);
						String targetPropertyTypeString = XMLUtility.readStringElement(trackElement, "targetPropertyType", 0);
						switch (track.targetType) {
						case PART:
							track.targetPropertyType = Figure.Part.PropertyType.valueOf(targetPropertyTypeString);
							break;
						case JOINT:
							track.targetPropertyType = Figure.Joint.PropertyType.valueOf(targetPropertyTypeString);
							break;
						case PATTERN:
							track.targetPropertyType = Figure.Pattern.PropertyType.valueOf(targetPropertyTypeString);
							break;
						}
						String finishTypeString = XMLUtility.readStringElement(trackElement, "finishType", 0);
						track.finishType = Animation.Track.FinishType.valueOf(finishTypeString);
						
						Element keysElement = (Element)trackElement.getElementsByTagName("keys").item(0);
						NodeList keyNodeList = keysElement.getElementsByTagName("key");
						track.keyVector = new Vector<Animation.Track.Key>();
						for (int keyIndex = 0; keyIndex < keyNodeList.getLength(); ++keyIndex) {
							Element keyElement = (Element)keyNodeList.item(keyIndex);
							Animation.Track.Key key = new Animation.Track.Key();
							{
								key.frame = XMLUtility.readFloatElement(keyElement, "frame", 0);
								key.value = XMLUtility.readFloatElement(keyElement, "value", 0);
								key.slope = XMLUtility.readFloatElement(keyElement, "slope", 0);
								String slopeCalcString = XMLUtility.readStringElement(keyElement, "slopeCalc", 0);
								key.slopeCalc = Animation.Track.Key.SlopeCalc.valueOf(slopeCalcString);
								String curveTypeString = XMLUtility.readStringElement(keyElement, "curveType", 0);
								key.curveType = Animation.Track.Key.CurveType.valueOf(curveTypeString);
							}
							track.keyVector.add(key);
						}						
					}
					animation.trackVector.add(track);
				}
			}
			
			AnimationUtility.validateKeys(animation);
			AnimationUtility.calculateSlopes(animation);
			
			return animation;
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			System.out.println(e.getStackTrace());
			
			return null;
		}
	}
}
