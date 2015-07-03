package lct.spri;

public class FigureUtility {
	public static Figure.Joint findJoint(Figure figure, String jointName) {
		for (Figure.Joint joint : figure.jointVector) {
			if (joint.name.equals(jointName)) {
				return joint;
			}
		}
		return null;
	}
	
	public static int findJointIndex(Figure figure, String jointName) {
		Figure.Joint joint = findJoint(figure, jointName);
		int index = figure.jointVector.indexOf(joint);
		return index;
	}
	
	public static Figure.Pattern findPattern(Figure figure, String patternName) {
		for (Figure.Pattern pattern : figure.patternVector) {
			if (pattern.name.equals(patternName)) {
				return pattern;
			}
		}
		return null;
	}
	
	public static int findPatternIndex(Figure figure, String patternName) {
		Figure.Pattern pattern = findPattern(figure, patternName);
		int index = figure.patternVector.indexOf(pattern);
		return index;
	}
}
