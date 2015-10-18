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
						XMLUtility.writeFloatElement(document, "loopSecond", track.loopSecond, trackElement);
						
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
						track.loopSecond = XMLUtility.readFloatElement(trackElement, "loopSecond", 0);
						
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
	
	public void storeSequence(Sequence sequence, String filePath) {
		try {
			Document document = XMLUtility.createDocument();
			
			Element sequenceElement = document.createElement("soun:sequence");
			{
				XMLUtility.writeIntegerElement(document, "clipHandleCount", sequence.clipHandleCount, sequenceElement);
				
				Element clipReferencesElement = document.createElement("clipReferences");
				for (Sequence.ClipReference clipReference : sequence.clipReferenceVector) {
					Element clipReferenceElement = document.createElement("clipReference");
					{
						XMLUtility.writeStringElement(document, "clipName", clipReference.clipName, clipReferenceElement);
					}
					clipReferencesElement.appendChild(clipReferenceElement);
				}
				sequenceElement.appendChild(clipReferencesElement);	
				
				Element rampReferencesElement = document.createElement("rampReferences");
				for (Sequence.RampReference rampReference : sequence.rampReferenceVector) {
					Element rampReferenceElement = document.createElement("rampReference");
					{
						XMLUtility.writeStringElement(document, "rampName", rampReference.rampName, rampReferenceElement);
					}
					rampReferencesElement.appendChild(rampReferenceElement);
				}
				sequenceElement.appendChild(rampReferencesElement);
				
				Element timelinesElement = document.createElement("timelines");
				for (Sequence.Timeline timeline : sequence.timelineVector) {
					Element timelineElement = document.createElement("timeline");
					{
						XMLUtility.writeStringElement(document, "finishType", timeline.finishType.toString(), timelineElement);
						XMLUtility.writeFloatElement(document, "loopSecond", timeline.loopSecond, timelineElement);
						
						Element actionsElement = document.createElement("actions");
						for (Sequence.Timeline.Action action : timeline.actionVector) {
							Element actionElement = document.createElement("action");
							{
								XMLUtility.writeFloatElement(document, "second", action.second, actionElement);
								XMLUtility.writeStringElement(document, "type", action.type.toString(), actionElement);
								switch (action.type) {
								case NONE:
									break;
								case BEGIN_CLIP:
									Sequence.Timeline.BeginClipAction beginClipAction = (Sequence.Timeline.BeginClipAction)action;
									XMLUtility.writeIntegerElement(document, "handleIndex", beginClipAction.handleIndex, actionElement);
									XMLUtility.writeIntegerElement(document, "clipIndex", beginClipAction.clipIndex, actionElement);
									break;
								case END_CLIP:
									Sequence.Timeline.EndClipAction endClipAction = (Sequence.Timeline.EndClipAction)action;
									XMLUtility.writeIntegerElement(document, "handleIndex", endClipAction.handleIndex, actionElement);
									break;
								case SET_RAMP:
									Sequence.Timeline.SetRampAction setRampAction = (Sequence.Timeline.SetRampAction)action;
									XMLUtility.writeIntegerElement(document, "handleIndex", setRampAction.handleIndex, actionElement);
									XMLUtility.writeIntegerElement(document, "rampIndex", setRampAction.rampIndex, actionElement);
									break;
								}
							}
							actionsElement.appendChild(actionElement);
						}
						timelineElement.appendChild(actionsElement);
					}
					timelinesElement.appendChild(timelineElement);
				}
				sequenceElement.appendChild(timelinesElement);
			}
			document.appendChild(sequenceElement);
			
			XMLUtility.storeDocument(document, filePath);
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			System.out.println(e.getStackTrace());
		}
	}
	
	public Sequence loadSequence(String filePath) {
		try {
			Document document = XMLUtility.loadDocument(filePath);
			
			Element sequenceElement = (Element)document.getElementsByTagName("soun:sequence").item(0);
			Sequence sequence = new Sequence();
			sequence.name = PathUtility.getFileNameNoExtension(filePath);
			{
				sequence.clipHandleCount = XMLUtility.readIntegerElement(sequenceElement, "clipHandleCount", 0);
				
				Element clipReferencesElement = (Element)sequenceElement.getElementsByTagName("clipReferences").item(0);
				NodeList clipReferenceNodeList = clipReferencesElement.getElementsByTagName("clipReference");
				sequence.clipReferenceVector = new Vector<Sequence.ClipReference>();
				for (int clipReferenceIndex = 0; clipReferenceIndex < clipReferenceNodeList.getLength(); ++clipReferenceIndex) {
					Element clipReferenceElement = (Element)clipReferenceNodeList.item(clipReferenceIndex);
					Sequence.ClipReference clipReference = new Sequence.ClipReference();
					{
						clipReference.clipName = XMLUtility.readStringElement(clipReferenceElement, "clipName", 0);
					}
					sequence.clipReferenceVector.add(clipReference);
				}
				
				Element rampReferencesElement = (Element)sequenceElement.getElementsByTagName("rampReferences").item(0);
				NodeList rampReferenceNodeList = rampReferencesElement.getElementsByTagName("rampReference");
				sequence.rampReferenceVector = new Vector<Sequence.RampReference>();
				for (int rampReferenceIndex = 0; rampReferenceIndex < rampReferenceNodeList.getLength(); ++rampReferenceIndex) {
					Element rampReferenceElement = (Element)rampReferenceNodeList.item(rampReferenceIndex);
					Sequence.RampReference rampReference = new Sequence.RampReference();
					{
						rampReference.rampName = XMLUtility.readStringElement(rampReferenceElement, "rampName", 0);
					}
					sequence.rampReferenceVector.add(rampReference);
				}
				
				Element timelinesElement = (Element)sequenceElement.getElementsByTagName("timelines").item(0);
				NodeList timelineNodeList = timelinesElement.getElementsByTagName("timeline");
				sequence.timelineVector = new Vector<Sequence.Timeline>();
				for (int timelineIndex = 0; timelineIndex < timelineNodeList.getLength(); ++timelineIndex) {
					Element timelineElement = (Element)timelineNodeList.item(timelineIndex);
					Sequence.Timeline timeline = new Sequence.Timeline();
					{
						String finishTypeString = XMLUtility.readStringElement(timelineElement, "finishType", 0);
						timeline.finishType = Sequence.Timeline.FinishType.valueOf(finishTypeString);
						timeline.loopSecond = XMLUtility.readFloatElement(timelineElement, "loopSecond", 0);
						
						Element actionsElement = (Element)timelineElement.getElementsByTagName("actions").item(0);
						NodeList actionNodeList = actionsElement.getElementsByTagName("action");
						timeline.actionVector = new Vector<Sequence.Timeline.Action>();
						for (int actionIndex = 0; actionIndex < actionNodeList.getLength(); ++actionIndex) {
							Element actionElement = (Element)actionNodeList.item(actionIndex);
							Sequence.Timeline.Action action = null;
							{
								float second = XMLUtility.readFloatElement(actionElement, "second", 0);
								String typeString = XMLUtility.readStringElement(actionElement, "type", 0);
								Sequence.Timeline.Action.Type type = Sequence.Timeline.Action.Type.valueOf(typeString);
								switch (type) {
								case NONE:
									action = new Sequence.Timeline.Action();
									break;
								case BEGIN_CLIP:
									Sequence.Timeline.BeginClipAction beginClipAction = new Sequence.Timeline.BeginClipAction();
									beginClipAction.handleIndex = XMLUtility.readIntegerElement(actionElement, "handleIndex", 0);
									beginClipAction.clipIndex = XMLUtility.readIntegerElement(actionElement, "clipIndex", 0);
									action = beginClipAction;
									break;
								case END_CLIP:
									Sequence.Timeline.EndClipAction endClipAction = new Sequence.Timeline.EndClipAction();
									endClipAction.handleIndex = XMLUtility.readIntegerElement(actionElement, "handleIndex", 0);
									action = endClipAction;
									break;
								case SET_RAMP:
									Sequence.Timeline.SetRampAction setRampAction = new Sequence.Timeline.SetRampAction();
									setRampAction.handleIndex = XMLUtility.readIntegerElement(actionElement, "handleIndex", 0);
									setRampAction.rampIndex = XMLUtility.readIntegerElement(actionElement, "rampIndex", 0);
									action = setRampAction;
									break;
								}
								action.second = second;
								action.type = type;
							}
							timeline.actionVector.add(action);
						}
					}
					sequence.timelineVector.add(timeline);
				}
			}
			
			return sequence;
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			System.out.println(e.getStackTrace());
			
			return null;
		}
	}
}
