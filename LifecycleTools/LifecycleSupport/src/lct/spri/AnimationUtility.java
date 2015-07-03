package lct.spri;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.image.BufferedImage;

import lct.util.DataException;

public class AnimationUtility {
	public static void validateKeys(Animation animation) throws DataException {
		for (Animation.Track track : animation.trackVector) {	
			if (track.keyVector.size() < 2) {
				throw new DataException("Track: " + track.targetName + " has less than 2 keys");
			}
			
			for (int keyIndex = 1; keyIndex < track.keyVector.size(); ++keyIndex) {
				Animation.Track.Key prevKey = track.keyVector.get(keyIndex - 1);
				Animation.Track.Key currKey = track.keyVector.get(keyIndex);				
				if (!(currKey.frame > prevKey.frame)) {
					throw new DataException("Track: " + track.targetName + " Key: " + keyIndex + " frame is not greater than previous Key frame");
				}
			}
		}
	}
	
	public static void calculateSlopes(Animation animation) {
		for (Animation.Track track : animation.trackVector) {						
			for (int keyIndex = 0; keyIndex < track.keyVector.size(); ++keyIndex) {
				Animation.Track.Key currKey = track.keyVector.get(keyIndex);
				if (currKey.slopeCalc == Animation.Track.Key.SlopeCalc.AUTO) {
					if (currKey.curveType == Animation.Track.Key.CurveType.FLAT) {
						currKey.slope = 0.0f;
					}
					else {
						if (keyIndex < (track.keyVector.size() - 1)) {
							Animation.Track.Key nextKey = track.keyVector.get(keyIndex + 1);
							float valueDelta = nextKey.value - currKey.value;
							float frameDelta = nextKey.frame - currKey.frame;
							currKey.slope = valueDelta / frameDelta;
						}
						else {
							currKey.slope = 0.0f;
						}
					}
				}
			}
		}
	}
	
	public static BufferedImage plotToBufferedImage(Animation.Track track) {
		float valueMax;
		switch (track.targetType) {
		case PART:
		{
			valueMax = 1.0f;
			break;
		}
		case JOINT:
		{
			Figure.Joint.PropertyType propertyType = (Figure.Joint.PropertyType)track.targetPropertyType;
			switch (propertyType) {
			case SCALE_X:
			case SCALE_Y:
			{
				valueMax = 4.0f;
				break;
			}
			case ROTATE:
			{
				valueMax = 1.0f;
				break;
			}
			case TRANSLATE_X:
			case TRANSLATE_Y:
			{
				valueMax = 128.0f;
				break;
			}
			default:
			{
				valueMax = 1.0f;
				break;
			}
			}
			break;
		}
		case PATTERN:
		{
			valueMax = 10.0f;
			break;
		}
		default:
		{
			valueMax = 1.0f;
			break;
		}
		}
		
		float frameDuration = track.keyVector.lastElement().frame;
		int imageWidth = (int)frameDuration;
		int imageHeight = 128;
		int yRange = imageHeight / 2;
		float valueScale = (float)yRange / valueMax;
		
		BufferedImage bufferedImage = new BufferedImage(imageWidth, imageHeight, BufferedImage.TYPE_INT_ARGB);
		Graphics graphics = bufferedImage.getGraphics();
		graphics.setColor(Color.GRAY);
		graphics.fillRect(0, 0, imageWidth, imageHeight);
		
		graphics.setColor(Color.BLACK);
		graphics.drawLine(0, (imageHeight / 2), (imageWidth - 1), (imageHeight / 2));
		
		graphics.setColor(Color.DARK_GRAY);
		for (int i = 0; i < imageWidth; i += 60) {
			graphics.drawLine(i, 0, i, (imageHeight - 1));
		}
		
		float prevFrame = 0.0f;
		float prevValue = 0.0f;
		float currFrame;
		float currValue;
		
		for (int keyIndex = 0; keyIndex < track.keyVector.size(); ++keyIndex) {
			Animation.Track.Key currKey = track.keyVector.get(keyIndex);
			Animation.Track.Key nextKey;
			if (keyIndex < (track.keyVector.size() - 1)) {
				nextKey = track.keyVector.get(keyIndex + 1);
			}
			else {
				nextKey = null;
			}
			
			if (nextKey != null) {
				for (float frame = currKey.frame; frame < nextKey.frame; frame += 1.0f) {
					currFrame = frame;
					switch (currKey.curveType)
					{
					case FLAT:
					{
						currValue = currKey.value;
					}
					break;
					case LINEAR:
					{
						float frameDelta = frame - currKey.frame;
						float valueDelta = frameDelta * currKey.slope;
						currValue = currKey.value + valueDelta;
					}
					break;
					case HERMITE:
					{
						float frameDelta = frame - currKey.frame;
						float frameSpan = nextKey.frame - currKey.frame;
						float t = frameDelta / frameSpan;
						float tSqu = t * t;
						float tCub = t * t * t;
						float basis1 = (2.0f * tCub) - (3.0f * tSqu) + 1.0f;
						float basis2 = -(2.0f * tCub) + (3.0f * tSqu);
						float basis3 = tCub - (2.0f * tSqu) + t;
						float basis4 = tCub - tSqu;
						currValue = (basis1 * currKey.value) + (basis2 * nextKey.value) + (basis3 * currKey.slope) + (basis4 * nextKey.slope);
					}
					break;
					default:
					{
						currValue = 0.0f;
					}
					}
					
					drawPlotLine(graphics, prevFrame, prevValue, currFrame, currValue, valueScale, yRange, Color.GREEN);
					
					prevFrame = currFrame;
					prevValue = currValue;
				}
			}
			
			drawPlotPoint(graphics, currKey.frame, currKey.value, valueScale, yRange, Color.BLUE);
		}
		
		return bufferedImage;
	}
	
	public static void drawPlotPoint(Graphics graphics, float frame, float value, float valueScale, int yOffset, Color color) {
		int imageX = (int)frame;
		int imageY = yOffset - (int)(value * valueScale);
		int width = 3;
		int height = 3;
		int left = imageX - (width / 2);
		int top = imageY - (height / 2);
		graphics.setColor(color);
		graphics.drawOval(left, top, width, height);
		graphics.fillOval(left, top, width, height);
	}
	
	public static void drawPlotLine(Graphics graphics, float frameA, float valueA, float frameB, float valueB, float valueScale, int yOffset, Color color) {
		int imageXA = (int)frameA;
		int imageYA = yOffset - (int)(valueA * valueScale);
		int imageXB = (int)frameB;
		int imageYB = yOffset - (int)(valueB * valueScale);
		graphics.setColor(color);
		graphics.drawLine(imageXA, imageYA, imageXB, imageYB);
	}
}
