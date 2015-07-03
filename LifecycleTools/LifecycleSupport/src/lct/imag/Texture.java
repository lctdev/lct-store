package lct.imag;

public class Texture {
	public static class TexelRGBA {
		public TexelRGBA() {
		}
		
		public TexelRGBA(int r, int g, int b, int a) {
			this.r = r;
			this.g = g;
			this.b = b;
			this.a = a;
		}
		
		public TexelRGBA(TexelRGBA other) {
			this.r = other.r;
			this.g = other.g;
			this.b = other.b;
			this.a = other.a;
		}
		
		public int r;
		public int g;
		public int b;
		public int a;
	}
	
	public String name;
	public int width;
	public int height;
	public Object[] texelArray;
}
