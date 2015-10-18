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
	
	static final String NOTE_WAVE_PREFIX = "example_wave_note_";
	static final String LOOP_WAVE_NAME = "example_wave_loop";
	static final String LOOP_SLIDE_WAVE_NAME = "example_wave_loop_slide";
	
	static final String NOTE_CLIP_PREFIX = "example_clip_note_";	
	static final String BASIC_LOOP_CLIP_NAME = "example_clip_basic_loop";
	static final String SEGMENT_LOOP_CLIP_NAME = "example_clip_segment_loop";
	
	static final String NOTE_RAMP_NAME = "example_ramp_note"; 
	static final String BASIC_LOOP_INTRO_RAMP_NAME = "example_ramp_basic_loop_intro";
	static final String BASIC_LOOP_OUTRO_RAMP_NAME = "example_ramp_basic_loop_outro";
	static final String WOBBLE_LOOP_INTRO_RAMP_NAME = "example_ramp_wobble_loop_intro";
	
	static final String NOTE_SEQUENCE_PREFIX = "example_sequence_note_";
	static final String SCALE_SEQUENCE_NAME = "example_sequence_scale";
	static final String BASIC_LOOP_SEQUENCE_NAME = "example_sequence_basic_loop";
	static final String WOBBLE_LOOP_SEQUENCE_NAME = "example_sequence_wobble_loop";
	static final String SEGMENT_LOOP_SEQUENCE_NAME = "example_sequence_segment_loop";
	
	static final String MANIFEST_NAME = "example_manifest";
	
	static final int MAX_SAMPLE_AMPLITUDIE_S16 = 32760;
	static final int TONE_AMPLITUDE = MAX_SAMPLE_AMPLITUDIE_S16 / 2;
	static final float TONE_SECONDS = 1.0f;
	static final float SLIDE_SECONDS = 1.0f;
	static final float SLIDE_STEP_SECONDS = 0.01f;
	static final int TONE_SAMPLE_RATE = 22050;
	
	public static Wave createNoteWave(Tone tone) {
		float frequency = TONE_FREQUENCIES[tone.ordinal()];
		Wave.SampleS16[] sampleArray = createToneSamplesS16(frequency, TONE_AMPLITUDE, TONE_SECONDS, TONE_SAMPLE_RATE);
		
		Wave wave = new Wave();
		wave.name = NOTE_WAVE_PREFIX + tone.toString();
		wave.channelCount = 1;
		wave.sampleSize = 2;
		wave.sampleLength = sampleArray.length;
		wave.sampleRate = TONE_SAMPLE_RATE;
		wave.sampleArray = sampleArray;
		
		return wave;
	}
	
	public static Wave createLoopWave() {
		float frequency = TONE_FREQUENCIES[Tone.C_5.ordinal()];
		float periodCount = frequency * TONE_SECONDS;
		periodCount = (float)Math.ceil((double)periodCount);
		float loopSeconds = periodCount / frequency;
		Wave.SampleS16[] sampleArray = createToneSamplesS16(frequency, TONE_AMPLITUDE, loopSeconds, TONE_SAMPLE_RATE);
		
		Wave wave = new Wave();
		wave.name = LOOP_WAVE_NAME;
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
	
	public static Wave createLoopSlideWave() {
		float frequencyA = TONE_FREQUENCIES[Tone.A_4.ordinal()];
		float frequencyB = TONE_FREQUENCIES[Tone.C_5.ordinal()];
		Wave.SampleS16[] sampleArray = createSlideSamplesS16(frequencyA, frequencyB, TONE_AMPLITUDE, SLIDE_SECONDS, TONE_SAMPLE_RATE, SLIDE_STEP_SECONDS);
		
		Wave wave = new Wave();
		wave.name = LOOP_SLIDE_WAVE_NAME;
		wave.channelCount = 1;
		wave.sampleSize = 2;
		wave.sampleLength = sampleArray.length;
		wave.sampleRate = TONE_SAMPLE_RATE;
		wave.sampleArray = sampleArray;
		
		return wave;
	}
	
	static class SlideStep {
		public float frequency;
		public int sampleCount;
	}
	
	public static Wave.SampleS16[] createSlideSamplesS16(float frequencyA, float frequencyB, int amplitude, float seconds, int sampleRate, float stepSeconds) {
		int stepCount = (int)(seconds / stepSeconds);
		SlideStep[] slideStepArray = new SlideStep[stepCount];
		float frequencyStep = (frequencyB - frequencyA) / (float)stepCount;
		int sampleCount = 0;
		for (int stepIndex = 0; stepIndex < stepCount; ++stepIndex) {
			SlideStep step = new SlideStep();
			
			if (stepIndex < (stepCount - 1)) {
				step.frequency = frequencyA + (stepIndex * frequencyStep);
			}
			else {
				step.frequency = frequencyB;
			}
			
			float periodCount = step.frequency * stepSeconds;
			periodCount = (float)Math.ceil((double)periodCount);
			float currStepSeconds = periodCount / step.frequency;
			step.sampleCount = (int)(currStepSeconds * (float)sampleRate);
			
			slideStepArray[stepIndex] = step;
			
			sampleCount += step.sampleCount;
		}
		
		Wave.SampleS16[] sampleArray = new Wave.SampleS16[sampleCount];
		int sampleIndex = 0;
		for (int stepIndex = 0; stepIndex < stepCount; ++stepIndex) {
			SlideStep step = slideStepArray[stepIndex];
			for (int stepSampleIndex = 0; stepSampleIndex < step.sampleCount; ++stepSampleIndex) {
				Wave.SampleS16 sample = new Wave.SampleS16();
				sample.v = (int)(Math.sin((2.0f * Math.PI * step.frequency) / (float)sampleRate * (float)stepSampleIndex) * (float)amplitude);
				sampleArray[sampleIndex] = sample;
				
				++sampleIndex;
			}
		}
		
		return sampleArray;
	}
	
	public static Clip createNoteClip(Tone tone) {
		Clip clip = new Clip();
		clip.name = NOTE_CLIP_PREFIX + tone.toString();
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
				segment.waveName = NOTE_WAVE_PREFIX + tone.toString();
				clip.segmentVector.add(segment);
			}
		}
		
		return clip;
	}
	
	public static Clip createBasicLoopClip() {
		Clip clip = new Clip();
		clip.name = BASIC_LOOP_CLIP_NAME;
		{
			clip.propertyArray = new float[Clip.PropertyType.values().length];
			{
				clip.propertyArray[Clip.PropertyType.VOLUME.ordinal()] = 1.0f;
				clip.propertyArray[Clip.PropertyType.PITCH.ordinal()] = 1.0f;
			}
			clip.loop = true;
			
			// segments
			clip.segmentVector = new Vector<Clip.Segment>();
			{
				Clip.Segment segment = new Clip.Segment();
				segment.waveName = LOOP_WAVE_NAME;
				clip.segmentVector.add(segment);
			}
		}
		
		return clip;
	}
	
	public static Clip createSegmentLoopClip() {
		Clip clip = new Clip();
		clip.name = SEGMENT_LOOP_CLIP_NAME;
		{
			clip.propertyArray = new float[Clip.PropertyType.values().length];
			{
				clip.propertyArray[Clip.PropertyType.VOLUME.ordinal()] = 1.0f;
				clip.propertyArray[Clip.PropertyType.PITCH.ordinal()] = 1.0f;
			}
			clip.loop = true;
			
			// segments
			clip.segmentVector = new Vector<Clip.Segment>();
			{
				Clip.Segment segment = new Clip.Segment();
				segment.waveName = LOOP_SLIDE_WAVE_NAME;
				clip.segmentVector.add(segment);
			}
			{
				Clip.Segment segment = new Clip.Segment();
				segment.waveName = LOOP_WAVE_NAME;
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
					key.value = 0.5f;
					key.curveType = Ramp.Track.Key.CurveType.LINEAR;
					track.keyVector.add(key);
				}
				{
					Ramp.Track.Key key = new Ramp.Track.Key();
					key.second = 0.2f;
					key.value = 1.0f;
					key.curveType = Ramp.Track.Key.CurveType.LINEAR;
					track.keyVector.add(key);
				}
				{
					Ramp.Track.Key key = new Ramp.Track.Key();
					key.second = 0.4f;
					key.value = 0.75f;
					key.curveType = Ramp.Track.Key.CurveType.LINEAR;
					track.keyVector.add(key);
				}
				{
					Ramp.Track.Key key = new Ramp.Track.Key();
					key.second = 0.6f;
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
	
	public static Ramp createBasicLoopIntroRamp() {
		Ramp ramp = new Ramp();
		ramp.name = BASIC_LOOP_INTRO_RAMP_NAME;
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
					key.second = 0.5f;
					key.value = 1.0f;
					key.curveType = Ramp.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				ramp.trackVector.add(track);
			}
		}
		
		RampUtility.calculateSlopes(ramp);
		
		return ramp;
	}
	
	public static Ramp createBasicLoopOutroRamp() {
		Ramp ramp = new Ramp();
		ramp.name = BASIC_LOOP_OUTRO_RAMP_NAME;
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
					key.value = 1.0f;
					key.curveType = Ramp.Track.Key.CurveType.LINEAR;
					track.keyVector.add(key);
				}
				{
					Ramp.Track.Key key = new Ramp.Track.Key();
					key.second = 0.5f;
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
	
	public static Ramp createWobbleLoopIntroRamp() {
		Ramp ramp = new Ramp();
		ramp.name = WOBBLE_LOOP_INTRO_RAMP_NAME;
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
					key.second = 0.5f;
					key.value = 1.0f;
					key.curveType = Ramp.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				ramp.trackVector.add(track);
			}
			{
				Ramp.Track track = new Ramp.Track();
				track.targetPropertyType = Clip.PropertyType.PITCH;
				track.finishType = Ramp.Track.FinishType.LOOP;
				track.loopSecond = 0.5f;
				track.keyVector = new Vector<Ramp.Track.Key>();
				{
					Ramp.Track.Key key = new Ramp.Track.Key();
					key.second = 0.0f;
					key.value = 1.0f;
					key.curveType = Ramp.Track.Key.CurveType.LINEAR;
					track.keyVector.add(key);
				}
				{
					Ramp.Track.Key key = new Ramp.Track.Key();
					key.second = 0.5f;
					key.value = 1.0f;
					key.curveType = Ramp.Track.Key.CurveType.LINEAR;
					track.keyVector.add(key);
				}
				{
					Ramp.Track.Key key = new Ramp.Track.Key();
					key.second = 1.0f;
					key.value = 1.5f;
					key.curveType = Ramp.Track.Key.CurveType.LINEAR;
					track.keyVector.add(key);
				}
				{
					Ramp.Track.Key key = new Ramp.Track.Key();
					key.second = 1.5f;
					key.value = 1.0f;
					key.curveType = Ramp.Track.Key.CurveType.LINEAR;
					track.keyVector.add(key);
				}
				{
					Ramp.Track.Key key = new Ramp.Track.Key();
					key.second = 2.0f;
					key.value = 0.5f;
					key.curveType = Ramp.Track.Key.CurveType.LINEAR;
					track.keyVector.add(key);
				}
				{
					Ramp.Track.Key key = new Ramp.Track.Key();
					key.second = 2.5f;
					key.value = 1.0f;
					key.curveType = Ramp.Track.Key.CurveType.FLAT;
					track.keyVector.add(key);
				}
				ramp.trackVector.add(track);
			}
		}
		
		RampUtility.calculateSlopes(ramp);
		
		return ramp;
	}
	
	public static Sequence createNoteSequence(Tone tone) {
		Sequence sequence = new Sequence();
		sequence.name = NOTE_SEQUENCE_PREFIX + tone.toString();
		{
			sequence.clipHandleCount = 1;
			
			sequence.clipReferenceVector = new Vector<Sequence.ClipReference>();
			{
				Sequence.ClipReference clipReference = new Sequence.ClipReference();
				clipReference.clipName = NOTE_CLIP_PREFIX + tone.toString();
				sequence.clipReferenceVector.add(clipReference);
			}
			
			sequence.rampReferenceVector = new Vector<Sequence.RampReference>();
			{
				Sequence.RampReference rampReference = new Sequence.RampReference();
				rampReference.rampName = NOTE_RAMP_NAME;
				sequence.rampReferenceVector.add(rampReference);
			}
			
			sequence.timelineVector = new Vector<Sequence.Timeline>();
			{
				Sequence.Timeline timeline =  new Sequence.Timeline();
				timeline.finishType = Sequence.Timeline.FinishType.HOLD;
				timeline.loopSecond = 0.0f;
				timeline.actionVector = new Vector<Sequence.Timeline.Action>();
				{
					Sequence.Timeline.BeginClipAction beginClipAction = new Sequence.Timeline.BeginClipAction();
					beginClipAction.second = 0.0f;
					beginClipAction.type = Sequence.Timeline.Action.Type.BEGIN_CLIP;
					beginClipAction.handleIndex = 0;
					beginClipAction.clipIndex = 0;
					timeline.actionVector.add(beginClipAction);
				}
				{
					Sequence.Timeline.SetRampAction setRampAction = new Sequence.Timeline.SetRampAction();
					setRampAction.second = 0.0f;
					setRampAction.type = Sequence.Timeline.Action.Type.SET_RAMP;
					setRampAction.handleIndex = 0;
					setRampAction.rampIndex = 0;
					timeline.actionVector.add(setRampAction);
				}
				sequence.timelineVector.add(timeline);
			}
		}
		
		return sequence;
	}
	
	public static Sequence createScaleSequence() {
		Sequence sequence = new Sequence();
		sequence.name = SCALE_SEQUENCE_NAME;
		{
			sequence.clipHandleCount = 1;
			
			sequence.clipReferenceVector = new Vector<Sequence.ClipReference>();
			{
				Sequence.ClipReference clipReference = new Sequence.ClipReference();
				clipReference.clipName = NOTE_CLIP_PREFIX + Tone.A_4.toString();
				sequence.clipReferenceVector.add(clipReference);
			}
			{
				Sequence.ClipReference clipReference = new Sequence.ClipReference();
				clipReference.clipName = NOTE_CLIP_PREFIX + Tone.B_4.toString();
				sequence.clipReferenceVector.add(clipReference);
			}
			{
				Sequence.ClipReference clipReference = new Sequence.ClipReference();
				clipReference.clipName = NOTE_CLIP_PREFIX + Tone.C_5.toString();
				sequence.clipReferenceVector.add(clipReference);
			}
			{
				Sequence.ClipReference clipReference = new Sequence.ClipReference();
				clipReference.clipName = NOTE_CLIP_PREFIX + Tone.D_5.toString();
				sequence.clipReferenceVector.add(clipReference);
			}
			{
				Sequence.ClipReference clipReference = new Sequence.ClipReference();
				clipReference.clipName = NOTE_CLIP_PREFIX + Tone.E_5.toString();
				sequence.clipReferenceVector.add(clipReference);
			}
			{
				Sequence.ClipReference clipReference = new Sequence.ClipReference();
				clipReference.clipName = NOTE_CLIP_PREFIX + Tone.F_5.toString();
				sequence.clipReferenceVector.add(clipReference);
			}
			{
				Sequence.ClipReference clipReference = new Sequence.ClipReference();
				clipReference.clipName = NOTE_CLIP_PREFIX + Tone.G_5.toString();
				sequence.clipReferenceVector.add(clipReference);
			}
			{
				Sequence.ClipReference clipReference = new Sequence.ClipReference();
				clipReference.clipName = NOTE_CLIP_PREFIX + Tone.A_5.toString();
				sequence.clipReferenceVector.add(clipReference);
			}
			
			sequence.rampReferenceVector = new Vector<Sequence.RampReference>();
			{
				Sequence.RampReference rampReference = new Sequence.RampReference();
				rampReference.rampName = NOTE_RAMP_NAME;
				sequence.rampReferenceVector.add(rampReference);
			}
						
			sequence.timelineVector = new Vector<Sequence.Timeline>();
			{
				Sequence.Timeline timeline =  new Sequence.Timeline();
				timeline.finishType = Sequence.Timeline.FinishType.LOOP;
				timeline.loopSecond = 0.0f;
				timeline.actionVector = new Vector<Sequence.Timeline.Action>();
				{
					Sequence.Timeline.BeginClipAction beginClipAction = new Sequence.Timeline.BeginClipAction();
					beginClipAction.second = 0.0f;
					beginClipAction.type = Sequence.Timeline.Action.Type.BEGIN_CLIP;
					beginClipAction.handleIndex = 0;
					beginClipAction.clipIndex = 0;
					timeline.actionVector.add(beginClipAction);
				}
				{
					Sequence.Timeline.SetRampAction setRampAction = new Sequence.Timeline.SetRampAction();
					setRampAction.second = 0.0f;
					setRampAction.type = Sequence.Timeline.Action.Type.SET_RAMP;
					setRampAction.handleIndex = 0;
					setRampAction.rampIndex = 0;
					timeline.actionVector.add(setRampAction);
				}
				
				{
					Sequence.Timeline.BeginClipAction beginClipAction = new Sequence.Timeline.BeginClipAction();
					beginClipAction.second = 0.25f;
					beginClipAction.type = Sequence.Timeline.Action.Type.BEGIN_CLIP;
					beginClipAction.handleIndex = 0;
					beginClipAction.clipIndex = 1;
					timeline.actionVector.add(beginClipAction);
				}
				{
					Sequence.Timeline.SetRampAction setRampAction = new Sequence.Timeline.SetRampAction();
					setRampAction.second = 0.25f;
					setRampAction.type = Sequence.Timeline.Action.Type.SET_RAMP;
					setRampAction.handleIndex = 0;
					setRampAction.rampIndex = 0;
					timeline.actionVector.add(setRampAction);
				}
				
				{
					Sequence.Timeline.BeginClipAction beginClipAction = new Sequence.Timeline.BeginClipAction();
					beginClipAction.second = 0.5f;
					beginClipAction.type = Sequence.Timeline.Action.Type.BEGIN_CLIP;
					beginClipAction.handleIndex = 0;
					beginClipAction.clipIndex = 2;
					timeline.actionVector.add(beginClipAction);
				}
				{
					Sequence.Timeline.SetRampAction setRampAction = new Sequence.Timeline.SetRampAction();
					setRampAction.second = 0.5f;
					setRampAction.type = Sequence.Timeline.Action.Type.SET_RAMP;
					setRampAction.handleIndex = 0;
					setRampAction.rampIndex = 0;
					timeline.actionVector.add(setRampAction);
				}
				
				{
					Sequence.Timeline.BeginClipAction beginClipAction = new Sequence.Timeline.BeginClipAction();
					beginClipAction.second = 0.75f;
					beginClipAction.type = Sequence.Timeline.Action.Type.BEGIN_CLIP;
					beginClipAction.handleIndex = 0;
					beginClipAction.clipIndex = 3;
					timeline.actionVector.add(beginClipAction);
				}
				{
					Sequence.Timeline.SetRampAction setRampAction = new Sequence.Timeline.SetRampAction();
					setRampAction.second = 0.75f;
					setRampAction.type = Sequence.Timeline.Action.Type.SET_RAMP;
					setRampAction.handleIndex = 0;
					setRampAction.rampIndex = 0;
					timeline.actionVector.add(setRampAction);
				}
				
				{
					Sequence.Timeline.BeginClipAction beginClipAction = new Sequence.Timeline.BeginClipAction();
					beginClipAction.second = 1.0f;
					beginClipAction.type = Sequence.Timeline.Action.Type.BEGIN_CLIP;
					beginClipAction.handleIndex = 0;
					beginClipAction.clipIndex = 4;
					timeline.actionVector.add(beginClipAction);
				}
				{
					Sequence.Timeline.SetRampAction setRampAction = new Sequence.Timeline.SetRampAction();
					setRampAction.second = 1.0f;
					setRampAction.type = Sequence.Timeline.Action.Type.SET_RAMP;
					setRampAction.handleIndex = 0;
					setRampAction.rampIndex = 0;
					timeline.actionVector.add(setRampAction);
				}
				
				{
					Sequence.Timeline.BeginClipAction beginClipAction = new Sequence.Timeline.BeginClipAction();
					beginClipAction.second = 1.25f;
					beginClipAction.type = Sequence.Timeline.Action.Type.BEGIN_CLIP;
					beginClipAction.handleIndex = 0;
					beginClipAction.clipIndex = 5;
					timeline.actionVector.add(beginClipAction);
				}
				{
					Sequence.Timeline.SetRampAction setRampAction = new Sequence.Timeline.SetRampAction();
					setRampAction.second = 1.25f;
					setRampAction.type = Sequence.Timeline.Action.Type.SET_RAMP;
					setRampAction.handleIndex = 0;
					setRampAction.rampIndex = 0;
					timeline.actionVector.add(setRampAction);
				}
				
				{
					Sequence.Timeline.BeginClipAction beginClipAction = new Sequence.Timeline.BeginClipAction();
					beginClipAction.second = 1.5f;
					beginClipAction.type = Sequence.Timeline.Action.Type.BEGIN_CLIP;
					beginClipAction.handleIndex = 0;
					beginClipAction.clipIndex = 6;
					timeline.actionVector.add(beginClipAction);
				}
				{
					Sequence.Timeline.SetRampAction setRampAction = new Sequence.Timeline.SetRampAction();
					setRampAction.second = 1.5f;
					setRampAction.type = Sequence.Timeline.Action.Type.SET_RAMP;
					setRampAction.handleIndex = 0;
					setRampAction.rampIndex = 0;
					timeline.actionVector.add(setRampAction);
				}
				
				{
					Sequence.Timeline.BeginClipAction beginClipAction = new Sequence.Timeline.BeginClipAction();
					beginClipAction.second = 1.75f;
					beginClipAction.type = Sequence.Timeline.Action.Type.BEGIN_CLIP;
					beginClipAction.handleIndex = 0;
					beginClipAction.clipIndex = 7;
					timeline.actionVector.add(beginClipAction);
				}
				{
					Sequence.Timeline.SetRampAction setRampAction = new Sequence.Timeline.SetRampAction();
					setRampAction.second = 1.75f;
					setRampAction.type = Sequence.Timeline.Action.Type.SET_RAMP;
					setRampAction.handleIndex = 0;
					setRampAction.rampIndex = 0;
					timeline.actionVector.add(setRampAction);
				}
				
				{
					Sequence.Timeline.Action action = new Sequence.Timeline.Action();
					action.second = 2.0f;
					action.type = Sequence.Timeline.Action.Type.NONE;
					timeline.actionVector.add(action);
				}
				sequence.timelineVector.add(timeline);
			}
		}
		
		return sequence;
	}
	
	public static Sequence createBasicLoopSequence() {
		Sequence sequence = new Sequence();
		sequence.name = BASIC_LOOP_SEQUENCE_NAME;
		{
			sequence.clipHandleCount = 1;
			
			sequence.clipReferenceVector = new Vector<Sequence.ClipReference>();
			{
				Sequence.ClipReference clipReference = new Sequence.ClipReference();
				clipReference.clipName = BASIC_LOOP_CLIP_NAME;
				sequence.clipReferenceVector.add(clipReference);
			}
			
			sequence.rampReferenceVector = new Vector<Sequence.RampReference>();
			{
				Sequence.RampReference rampReference = new Sequence.RampReference();
				rampReference.rampName = BASIC_LOOP_INTRO_RAMP_NAME;
				sequence.rampReferenceVector.add(rampReference);
			}
			{
				Sequence.RampReference rampReference = new Sequence.RampReference();
				rampReference.rampName = BASIC_LOOP_OUTRO_RAMP_NAME;
				sequence.rampReferenceVector.add(rampReference);
			}
			
			sequence.timelineVector = new Vector<Sequence.Timeline>();
			{
				Sequence.Timeline timeline =  new Sequence.Timeline();
				timeline.finishType = Sequence.Timeline.FinishType.HOLD;
				timeline.loopSecond = 0.0f;
				timeline.actionVector = new Vector<Sequence.Timeline.Action>();
				{
					Sequence.Timeline.BeginClipAction beginClipAction = new Sequence.Timeline.BeginClipAction();
					beginClipAction.second = 0.0f;
					beginClipAction.type = Sequence.Timeline.Action.Type.BEGIN_CLIP;
					beginClipAction.handleIndex = 0;
					beginClipAction.clipIndex = 0;
					timeline.actionVector.add(beginClipAction);
				}
				{
					Sequence.Timeline.SetRampAction setRampAction = new Sequence.Timeline.SetRampAction();
					setRampAction.second = 0.0f;
					setRampAction.type = Sequence.Timeline.Action.Type.SET_RAMP;
					setRampAction.handleIndex = 0;
					setRampAction.rampIndex = 0;
					timeline.actionVector.add(setRampAction);
				}
				sequence.timelineVector.add(timeline);
			}
			{
				Sequence.Timeline timeline =  new Sequence.Timeline();
				timeline.finishType = Sequence.Timeline.FinishType.HOLD;
				timeline.loopSecond = 0.0f;
				timeline.actionVector = new Vector<Sequence.Timeline.Action>();
				{
					Sequence.Timeline.EndClipAction endClipAction = new Sequence.Timeline.EndClipAction();
					endClipAction.second = 0.0f;
					endClipAction.type = Sequence.Timeline.Action.Type.END_CLIP;
					endClipAction.handleIndex = 0;
					timeline.actionVector.add(endClipAction);
				}
				{
					Sequence.Timeline.SetRampAction setRampAction = new Sequence.Timeline.SetRampAction();
					setRampAction.second = 0.0f;
					setRampAction.type = Sequence.Timeline.Action.Type.SET_RAMP;
					setRampAction.handleIndex = 0;
					setRampAction.rampIndex = 1;
					timeline.actionVector.add(setRampAction);
				}
				sequence.timelineVector.add(timeline);
			}
		}
		
		return sequence;
	}
	
	public static Sequence createWobbleLoopSequence() {
		Sequence sequence = new Sequence();
		sequence.name = WOBBLE_LOOP_SEQUENCE_NAME;
		{
			sequence.clipHandleCount = 1;
			
			sequence.clipReferenceVector = new Vector<Sequence.ClipReference>();
			{
				Sequence.ClipReference clipReference = new Sequence.ClipReference();
				clipReference.clipName = BASIC_LOOP_CLIP_NAME;
				sequence.clipReferenceVector.add(clipReference);
			}
			
			sequence.rampReferenceVector = new Vector<Sequence.RampReference>();
			{
				Sequence.RampReference rampReference = new Sequence.RampReference();
				rampReference.rampName = WOBBLE_LOOP_INTRO_RAMP_NAME;
				sequence.rampReferenceVector.add(rampReference);
			}
			{
				Sequence.RampReference rampReference = new Sequence.RampReference();
				rampReference.rampName = BASIC_LOOP_OUTRO_RAMP_NAME;
				sequence.rampReferenceVector.add(rampReference);
			}
			
			sequence.timelineVector = new Vector<Sequence.Timeline>();
			{
				Sequence.Timeline timeline =  new Sequence.Timeline();
				timeline.finishType = Sequence.Timeline.FinishType.HOLD;
				timeline.loopSecond = 0.0f;
				timeline.actionVector = new Vector<Sequence.Timeline.Action>();
				{
					Sequence.Timeline.BeginClipAction beginClipAction = new Sequence.Timeline.BeginClipAction();
					beginClipAction.second = 0.0f;
					beginClipAction.type = Sequence.Timeline.Action.Type.BEGIN_CLIP;
					beginClipAction.handleIndex = 0;
					beginClipAction.clipIndex = 0;
					timeline.actionVector.add(beginClipAction);
				}
				{
					Sequence.Timeline.SetRampAction setRampAction = new Sequence.Timeline.SetRampAction();
					setRampAction.second = 0.0f;
					setRampAction.type = Sequence.Timeline.Action.Type.SET_RAMP;
					setRampAction.handleIndex = 0;
					setRampAction.rampIndex = 0;
					timeline.actionVector.add(setRampAction);
				}
				sequence.timelineVector.add(timeline);
			}
			{
				Sequence.Timeline timeline =  new Sequence.Timeline();
				timeline.finishType = Sequence.Timeline.FinishType.HOLD;
				timeline.loopSecond = 0.0f;
				timeline.actionVector = new Vector<Sequence.Timeline.Action>();
				{
					Sequence.Timeline.EndClipAction endClipAction = new Sequence.Timeline.EndClipAction();
					endClipAction.second = 0.0f;
					endClipAction.type = Sequence.Timeline.Action.Type.END_CLIP;
					endClipAction.handleIndex = 0;
					timeline.actionVector.add(endClipAction);
				}
				{
					Sequence.Timeline.SetRampAction setRampAction = new Sequence.Timeline.SetRampAction();
					setRampAction.second = 0.0f;
					setRampAction.type = Sequence.Timeline.Action.Type.SET_RAMP;
					setRampAction.handleIndex = 0;
					setRampAction.rampIndex = 1;
					timeline.actionVector.add(setRampAction);
				}
				sequence.timelineVector.add(timeline);
			}
		}
		
		return sequence;
	}
	
	public static Sequence createSegmentLoopSequence() {
		Sequence sequence = new Sequence();
		sequence.name = SEGMENT_LOOP_SEQUENCE_NAME;
		{
			sequence.clipHandleCount = 1;
			
			sequence.clipReferenceVector = new Vector<Sequence.ClipReference>();
			{
				Sequence.ClipReference clipReference = new Sequence.ClipReference();
				clipReference.clipName = SEGMENT_LOOP_CLIP_NAME;
				sequence.clipReferenceVector.add(clipReference);
			}
			
			sequence.rampReferenceVector = new Vector<Sequence.RampReference>();
			
			sequence.timelineVector = new Vector<Sequence.Timeline>();
			{
				Sequence.Timeline timeline =  new Sequence.Timeline();
				timeline.finishType = Sequence.Timeline.FinishType.HOLD;
				timeline.loopSecond = 0.0f;
				timeline.actionVector = new Vector<Sequence.Timeline.Action>();
				{
					Sequence.Timeline.BeginClipAction beginClipAction = new Sequence.Timeline.BeginClipAction();
					beginClipAction.second = 0.0f;
					beginClipAction.type = Sequence.Timeline.Action.Type.BEGIN_CLIP;
					beginClipAction.handleIndex = 0;
					beginClipAction.clipIndex = 0;
					timeline.actionVector.add(beginClipAction);
				}
				sequence.timelineVector.add(timeline);
			}
			{
				Sequence.Timeline timeline =  new Sequence.Timeline();
				timeline.finishType = Sequence.Timeline.FinishType.HOLD;
				timeline.loopSecond = 0.0f;
				timeline.actionVector = new Vector<Sequence.Timeline.Action>();
				{
					Sequence.Timeline.EndClipAction endClipAction = new Sequence.Timeline.EndClipAction();
					endClipAction.second = 0.0f;
					endClipAction.type = Sequence.Timeline.Action.Type.END_CLIP;
					endClipAction.handleIndex = 0;
					timeline.actionVector.add(endClipAction);
				}
				sequence.timelineVector.add(timeline);
			}
		}
		
		return sequence;
	}
	
	public static Manifest createManifest(String directoryPath) {
		Manifest manifest = new Manifest();
		manifest.name = MANIFEST_NAME;
		{
			manifest.assetEntryVector = new Vector<Manifest.AssetEntry>();
			{
				for (Tone tone : Tone.values())
				{
					String fileName = NOTE_WAVE_PREFIX + tone.toString() + ".wav";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = lct.soun.Constants.GROUP_CODE;
					assetEntry.typeCode = lct.soun.Constants.WAVE_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);
					manifest.assetEntryVector.add(assetEntry);
				}
				
				{
					String fileName = LOOP_WAVE_NAME + ".wav";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = lct.soun.Constants.GROUP_CODE;
					assetEntry.typeCode = lct.soun.Constants.WAVE_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);
					manifest.assetEntryVector.add(assetEntry);
				}
				
				{
					String fileName = LOOP_SLIDE_WAVE_NAME + ".wav";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = lct.soun.Constants.GROUP_CODE;
					assetEntry.typeCode = lct.soun.Constants.WAVE_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);
					manifest.assetEntryVector.add(assetEntry);
				}
				
				for (Tone tone : Tone.values())
				{
					String fileName = NOTE_CLIP_PREFIX + tone.toString() + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = Constants.GROUP_CODE;
					assetEntry.typeCode = Constants.CLIP_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);					
					manifest.assetEntryVector.add(assetEntry);
				}
				
				{
					String fileName = BASIC_LOOP_CLIP_NAME + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = Constants.GROUP_CODE;
					assetEntry.typeCode = Constants.CLIP_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);					
					manifest.assetEntryVector.add(assetEntry);
				}
				
				{
					String fileName = SEGMENT_LOOP_CLIP_NAME + ".xml";
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
					String fileName = BASIC_LOOP_INTRO_RAMP_NAME + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = Constants.GROUP_CODE;
					assetEntry.typeCode = Constants.RAMP_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);					
					manifest.assetEntryVector.add(assetEntry);
				}
				
				{
					String fileName = BASIC_LOOP_OUTRO_RAMP_NAME + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = Constants.GROUP_CODE;
					assetEntry.typeCode = Constants.RAMP_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);					
					manifest.assetEntryVector.add(assetEntry);
				}
				
				{
					String fileName = WOBBLE_LOOP_INTRO_RAMP_NAME + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = Constants.GROUP_CODE;
					assetEntry.typeCode = Constants.RAMP_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);					
					manifest.assetEntryVector.add(assetEntry);
				}
				
				{
					String fileName = NOTE_SEQUENCE_PREFIX + Tone.A_4.toString() + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = Constants.GROUP_CODE;
					assetEntry.typeCode = Constants.SEQUENCE_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);					
					manifest.assetEntryVector.add(assetEntry);
				}
				
				{
					String fileName = NOTE_SEQUENCE_PREFIX + Tone.C_5.toString() + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = Constants.GROUP_CODE;
					assetEntry.typeCode = Constants.SEQUENCE_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);					
					manifest.assetEntryVector.add(assetEntry);
				}
				
				{
					String fileName = NOTE_SEQUENCE_PREFIX + Tone.E_5.toString() + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = Constants.GROUP_CODE;
					assetEntry.typeCode = Constants.SEQUENCE_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);					
					manifest.assetEntryVector.add(assetEntry);
				}
				
				{
					String fileName = SCALE_SEQUENCE_NAME + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = Constants.GROUP_CODE;
					assetEntry.typeCode = Constants.SEQUENCE_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);					
					manifest.assetEntryVector.add(assetEntry);
				}
				
				{
					String fileName = BASIC_LOOP_SEQUENCE_NAME + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = Constants.GROUP_CODE;
					assetEntry.typeCode = Constants.SEQUENCE_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);					
					manifest.assetEntryVector.add(assetEntry);
				}
				
				{
					String fileName = WOBBLE_LOOP_SEQUENCE_NAME + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = Constants.GROUP_CODE;
					assetEntry.typeCode = Constants.SEQUENCE_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);					
					manifest.assetEntryVector.add(assetEntry);
				}
				
				{
					String fileName = SEGMENT_LOOP_SEQUENCE_NAME + ".xml";
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = Constants.GROUP_CODE;
					assetEntry.typeCode = Constants.SEQUENCE_TYPE_CODE;
					assetEntry.filePath = PathUtility.combineFull(directoryPath, fileName);					
					manifest.assetEntryVector.add(assetEntry);
				}
			}
		}
		return manifest;
	}
	
}

