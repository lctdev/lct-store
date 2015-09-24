package lct.soun;

import java.util.Vector;

import lct.util.PathUtility;
import lct.pack.Manifest;

public class Examples {
	
	public enum Tone {
		A_4,
		B_FLAT_4,
		B_4,
		C_5,
		C_SHARP_5,
		D_5,
		E_FLAT_5,
		E_5,
		F_5,
		F_SHARP_5,
		G_5,
		G_SHARP_5,
		A_5
	}
	
	static final float[] TONE_FREQUENCIES = {
		440.0f,
		466.16f,
		493.88f,
		523.25f,
		554.37f,
		587.33f,
		622.25f,
		659.25f,
		698.46f,
		739.99f,
		783.99f,
		830.61f,
		880.00f
	};
	
	static final String TONE_CLIP_PREFIX = "example_clip_tone_";
	static final String TONE_WAVE_PREFIX = "example_wave_tone_";
	static final String NOTE_RAMP_NAME = "example_ramp_note";
	static final String MANIFEST_NAME = "example_manifest";
	
	static final int MAX_SAMPLE_AMPLITUDIE_S16 = 32760;
	static final int TONE_AMPLITUDE = MAX_SAMPLE_AMPLITUDIE_S16 / 2;
	static final float TONE_SECONDS = 2.0f;
	static final int TONE_SAMPLE_RATE = 22050;
	
	
	public static Clip createToneClip(Tone tone) {
		Clip clip = new Clip();
		clip.name = TONE_CLIP_PREFIX + tone.toString();
		{
			clip.propertyArray = new float[Clip.PropertyType.values().length];
			{
				clip.propertyArray[Clip.PropertyType.VOLUME.ordinal()] = 1.0f;
				clip.propertyArray[Clip.PropertyType.PITCH.ordinal()] = 1.0f;
			}
			clip.loop = false;
			
			// segments
			clip.segmentVector = new Vector<Clip.Segment>();
			{
				Clip.Segment segment = new Clip.Segment();
				segment.waveName = TONE_WAVE_PREFIX + tone.toString();
				clip.segmentVector.add(segment);
			}
		}
		
		return clip;
	}
	
	public static Ramp createNoteRamp() {
		Ramp ramp = new Ramp();
		ramp.name = NOTE_RAMP_NAME;
		{
			ramp.trackVector = new Vector<Ramp.Track>();
			{
				Ramp.Track track = new Ramp.Track();
				track.targetPropertyType = Clip.PropertyType.VOLUME;
				track.finishType = Ramp.Track.FinishType.HOLD;
				track.keyVector = new Vector<Ramp.Track.Key>();
				{
					Ramp.Track.Key key = new Ramp.Track.Key();
					key.second = 0.0f;
					key.value = 0.0f;
					key.curveType = Ramp.Track.Key.CurveType.LINEAR;
					track.keyVector.add(key);
				}
				{
					Ramp.Track.Key key = new Ramp.Track.Key();
					key.second = 0.25f;
					key.value = 1.0f;
					key.curveType = Ramp.Track.Key.CurveType.LINEAR;
					track.keyVector.add(key);
				}
				{
					Ramp.Track.Key key = new Ramp.Track.Key();
					key.second = 0.5f;
					key.value = 0.75f;
					key.curveType = Ramp.Track.Key.CurveType.LINEAR;
					track.keyVector.add(key);
				}
				{
					Ramp.Track.Key key = new Ramp.Track.Key();
					key.second = 0.75f;
					key.value = 0.75f;
					key.curveType = Ramp.Track.Key.CurveType.LINEAR;
					track.keyVector.add(key);
				}
				{
					Ramp.Track.Key key = new Ramp.Track.Key();
					key.second = 1.0f;
					key.value = 0.0f;
					key.curveType = Ramp.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				ramp.trackVector.add(track);
			}
		}
		
		RampUtility.calculateSlopes(ramp);
		
		return ramp;
	}
	
	public static Wave createToneWave(Tone tone) {
		float frequency = TONE_FREQUENCIES[tone.ordinal()];
		Wave.SampleS16[] sampleArray = createToneSamplesS16(frequency, TONE_AMPLITUDE, TONE_SECONDS, TONE_SAMPLE_RATE);
		
		Wave wave = new Wave();
		wave.name = TONE_WAVE_PREFIX + tone.toString();
		wave.channelCount = 1;
		wave.sampleSize = 2;
		wave.sampleLength = sampleArray.length;
		wave.sampleRate = TONE_SAMPLE_RATE;
		wave.sampleArray = sampleArray;
		
		return wave;
	}
	
	public static Wave.SampleS16[] createToneSamplesS16(float frequency, int amplitude, float seconds, int sampleRate) {
		int sampleCount = (int)(seconds * (float)sampleRate);
		
		Wave.SampleS16[] sampleArray = new Wave.SampleS16[sampleCount];
		for (int sampleIndex = 0; sampleIndex < sampleCount; ++sampleIndex) {
			Wave.SampleS16 sample = new Wave.SampleS16();
			sample.v = (int)(Math.sin((2.0f * Math.PI * frequency) / (float)sampleRate * (float)sampleIndex) * (float)amplitude);
			sampleArray[sampleIndex] = sample;
		}
		
		return sampleArray;
	}
	
	public static Manifest createManifest(String directoryPath) {
		Manifest manifest = new Manifest();
		manifest.name = MANIFEST_NAME;
		{
			manifest.assetEntryVector = new Vector<Manifest.AssetEntry>();
			{
				
				{
					String fileName = TONE_CLIP_PREFIX + Tone.A_4.toString() + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = Constants.GROUP_CODE;
					assetEntry.typeCode = Constants.CLIP_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);					
					manifest.assetEntryVector.add(assetEntry);
				}
				
				{
					String fileName = NOTE_RAMP_NAME + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = Constants.GROUP_CODE;
					assetEntry.typeCode = Constants.RAMP_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);					
					manifest.assetEntryVector.add(assetEntry);
				}
		
				{
					String fileName = TONE_WAVE_PREFIX + Tone.A_4.toString() + ".wav";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = lct.soun.Constants.GROUP_CODE;
					assetEntry.typeCode = lct.soun.Constants.WAVE_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);
					manifest.assetEntryVector.add(assetEntry);
				}
			}
		}
		return manifest;
	}
	
}

