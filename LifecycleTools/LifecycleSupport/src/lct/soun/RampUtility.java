package lct.soun;

public class RampUtility {
	public static void calculateSlopes(Ramp ramp) {
		for (Ramp.Track track : ramp.trackVector) {						
			for (int keyIndex = 0; keyIndex < track.keyVector.size(); ++keyIndex) {
				Ramp.Track.Key currKey = track.keyVector.get(keyIndex);
				if (currKey.curveType == Ramp.Track.Key.CurveType.FLAT) {
					currKey.slope = 0.0f;
				}
				else {
					if (keyIndex < (track.keyVector.size() - 1)) {
						Ramp.Track.Key nextKey = track.keyVector.get(keyIndex + 1);
						float valueDelta = nextKey.value - currKey.value;
						float frameDelta = nextKey.second - currKey.second;
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
