package assetcompiler;

import lct.pack.*;

public class Program {
	
	private static final String MANIFEST_FILE_PATH_OPTION = "-manifest=";
	private static final String TEMP_DIRECTORY_PATH_OPTION = "-temp=";
	private static final String PACKAGE_FILE_PATH_OPTION = "-package=";
	
	private static String s_manifestFilePath;
	private static String s_tempDirectoryPath;
	private static String s_packageFilePath;
	private static boolean s_bigEndian = false;
	
	private static void parseArguments(String[] args) {
		for (int index = 0; index < args.length; ++index) {
			String arg = args[index];
			if (arg.startsWith(MANIFEST_FILE_PATH_OPTION)) {
				s_manifestFilePath = arg.substring(MANIFEST_FILE_PATH_OPTION.length());
			}
			else if (arg.startsWith(TEMP_DIRECTORY_PATH_OPTION)) {
				s_tempDirectoryPath = arg.substring(TEMP_DIRECTORY_PATH_OPTION.length());
			}
			else if (arg.startsWith(PACKAGE_FILE_PATH_OPTION)) {
				s_packageFilePath = arg.substring(PACKAGE_FILE_PATH_OPTION.length());
			}
		}
	}
	
	private static void buildPackage() {
		if ((s_manifestFilePath == null) || (s_tempDirectoryPath == null) || (s_packageFilePath == null)) {
			System.err.println("Invalid arguments for package task.");
			System.exit(-1);
		}
		
		PackageBuilder packageBuilder = new PackageBuilder();
		packageBuilder.addAssetProcessor(new lct.imag.AssetProcessor());
		packageBuilder.addAssetProcessor(new lct.spri.AssetProcessor());		
		packageBuilder.addAssetProcessor(new lct.font.AssetProcessor());
		packageBuilder.addAssetProcessor(new lct.soun.AssetProcessor());
		packageBuilder.addAssetProcessor(new lct.pass.AssetProcessor());
		
		try {
			packageBuilder.buildPackage(s_manifestFilePath, s_packageFilePath, s_tempDirectoryPath, s_bigEndian);
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
			System.exit(-1);
		}
	}

	public static void main(String[] args) {
		parseArguments(args);
		 
		buildPackage();
	}

}
