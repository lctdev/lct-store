package lct.soun;

public class Wave {
	public static class SampleS16 {
		public SampleS16() {
		}
		
		public SampleS16(int v) {
			this.v = v;
		}
		
		public int v;
	}
	
	public String name;
	public int channelCount;
	public int sampleSize;
	public int sampleLength;
	public int sampleRate;
	public Object[] sampleArray;
}