package lct;

public class NativeLibrary {
	public static void Load() {
		try {
			System.loadLibrary("LifecycleSupport");
		}
		catch (Exception e) {
			System.out.println("Failure to load native library");
		}
	}
}
