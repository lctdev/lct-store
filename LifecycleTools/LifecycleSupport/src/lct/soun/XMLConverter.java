package lct.soun;

import lct.util.PathUtility;
import lct.util.XMLUtility;

import java.util.Vector;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

public class XMLConverter {
	public XMLConverter() {		
	}
	
	public void storeClip(Clip clip, String filePath) {
		try {
			Document document = XMLUtility.createDocument();
			
			Element clipElement = document.createElement("soun:clip");
			{
				Element propertiesElement = document.createElement("properties");
				for (int propertyIndex = 0; propertyIndex < Clip.PropertyType.values().length; ++propertyIndex) {
					float property = clip.propertyArray[propertyIndex];
					Clip.PropertyType propertyType = Clip.PropertyType.values()[propertyIndex];
					Element propertyElement = XMLUtility.writeFloatElement(document, "property", property, propertiesElement);
					propertyElement.setAttribute("type", propertyType.toString());
					propertiesElement.appendChild(propertyElement);
				}
				clipElement.appendChild(propertiesElement);
				
				XMLUtility.writeBooleanElement(document, "loop", clip.loop, clipElement);
				
				Element segmentsElement = document.createElement("segments");
				for (Clip.Segment segment : clip.segmentVector) {
					Element segmentElement = document.createElement("segment");
					{
						XMLUtility.writeStringElement(document, "waveName", segment.waveName, segmentElement);
					}
					segmentsElement.appendChild(segmentElement);
				}
				clipElement.appendChild(segmentsElement);				
			}
			document.appendChild(clipElement); 
			
			XMLUtility.storeDocument(document, filePath);
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			System.out.println(e.getStackTrace());
		}
	}
	
	public Clip loadClip(String filePath) {
		try {
			Document document = XMLUtility.loadDocument(filePath);
			
			Element clipElement = (Element)document.getElementsByTagName("soun:clip").item(0);
			Clip clip = new Clip();
			clip.name = PathUtility.getFileNameNoExtension(filePath);
			{
				Element propertiesElement = (Element)clipElement.getElementsByTagName("properties").item(0);
				NodeList propertyNodeList = propertiesElement.getElementsByTagName("property");
				clip.propertyArray = new float[Clip.PropertyType.values().length];
				for (int propertyIndex = 0; propertyIndex < propertyNodeList.getLength(); ++propertyIndex) {
					float property = XMLUtility.readFloatElement(propertiesElement, "property", propertyIndex);
					clip.propertyArray[propertyIndex] = property;
				}
				
				clip.loop = XMLUtility.readBooleanElement(clipElement, "loop", 0);
				
				Element segmentsElement = (Element)clipElement.getElementsByTagName("segments").item(0);
				NodeList segmentNodeList = segmentsElement.getElementsByTagName("segment");
				clip.segmentVector = new Vector<Clip.Segment>();
				for (int segmentIndex = 0; segmentIndex < segmentNodeList.getLength(); ++segmentIndex) {
					Element segmentElement = (Element)segmentNodeList.item(segmentIndex);
					Clip.Segment segment = new Clip.Segment();
					{
						segment.waveName = XMLUtility.readStringElement(segmentElement, "waveName", 0);
					}
					clip.segmentVector.add(segment);
				}
			}
			return clip;
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			System.out.println(e.getStackTrace());
			
			return null;
		}
	}
	
	public void storeRamp(Ramp ramp, String filePath) {
		try {
			Document document = XMLUtility.createDocument();
			
			Element rampElement = document.createElement("soun:ramp");
			{
				Element tracksElement = document.createElement("tracks");
				for (Ramp.Track track : ramp.trackVector) {
					Element trackElement = document.createElement("track");
					{						
						XMLUtility.writeStringElement(document, "targetPropertyType", track.targetPropertyType.toString(), trackElement);
						XMLUtility.writeStringElement(document, "finishType", track.finishType.toString(), trackElement);
						
						Element keysElement = document.createElement("keys");
						for (Ramp.Track.Key key : track.keyVector) {
							Element keyElement = document.createElement("key");
							{
								XMLUtility.writeFloatElement(document, "second", key.second, keyElement);
								XMLUtility.writeFloatElement(document, "value", key.value, keyElement);
								XMLUtility.writeFloatElement(document, "slope", key.slope, keyElement);
								XMLUtility.writeStringElement(document, "curveType", key.curveType.toString(), keyElement);
							}
							keysElement.appendChild(keyElement);
						}
						trackElement.appendChild(keysElement);
					}
					tracksElement.appendChild(trackElement);
				}
				rampElement.appendChild(tracksElement);
			}
			document.appendChild(rampElement);
			
			XMLUtility.storeDocument(document, filePath);
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			System.out.println(e.getStackTrace());
		}
	}
	
	public Ramp loadRamp(String filePath) {
		try {
			Document document = XMLUtility.loadDocument(filePath);
			
			Element rampElement = (Element)document.getElementsByTagName("soun:ramp").item(0);
			Ramp ramp = new Ramp();
			ramp.name = PathUtility.getFileNameNoExtension(filePath);
			{
				Element tracksElement = (Element)rampElement.getElementsByTagName("tracks").item(0);
				NodeList trackNodeList = tracksElement.getElementsByTagName("track");
				ramp.trackVector = new Vector<Ramp.Track>();
				for (int trackIndex = 0; trackIndex < trackNodeList.getLength(); ++trackIndex) {
					Element trackElement = (Element)trackNodeList.item(trackIndex);
					Ramp.Track track = new Ramp.Track();
					{
						String targetPropertyTypeString = XMLUtility.readStringElement(trackElement, "targetPropertyType", 0);
						track.targetPropertyType = Clip.PropertyType.valueOf(targetPropertyTypeString);
						String finishTypeString = XMLUtility.readStringElement(trackElement, "finishType", 0);
						track.finishType = Ramp.Track.FinishType.valueOf(finishTypeString);
						
						Element keysElement = (Element)trackElement.getElementsByTagName("keys").item(0);
						NodeList keyNodeList = keysElement.getElementsByTagName("key");
						track.keyVector = new Vector<Ramp.Track.Key>();
						for (int keyIndex = 0; keyIndex < keyNodeList.getLength(); ++keyIndex) {
							Element keyElement = (Element)keyNodeList.item(keyIndex);
							Ramp.Track.Key key = new Ramp.Track.Key();
							{
								key.second = XMLUtility.readFloatElement(keyElement, "second", 0);
								key.value = XMLUtility.readFloatElement(keyElement, "value", 0);
								key.slope = XMLUtility.readFloatElement(keyElement, "slope", 0);
								String curveTypeString = XMLUtility.readStringElement(keyElement, "curveType", 0);
								key.curveType = Ramp.Track.Key.CurveType.valueOf(curveTypeString);
							}
							track.keyVector.add(key);
						}						
					}
					ramp.trackVector.add(track);
				}
			}
			
			//RampUtility.validateKeys(ramp);
			RampUtility.calculateSlopes(ramp);
			
			return ramp;
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			System.out.println(e.getStackTrace());
			
			return null;
		}
	}
}
