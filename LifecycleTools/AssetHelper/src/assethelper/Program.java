package assethelper;

import java.awt.Font;
import java.awt.image.BufferedImage;

import lct.util.FileUtility;

public class Program {
	
	private static final String GROUP_OPTION = "-group=";
	private static final String TASK_OPTION = "-task=";
	private static final String TASK_EXAMPLE = "example";
	private static final String TASK_PLOT = "plot";
	private static final String INPUT_FILE_PATH_OPTION = "-input=";
	private static final String OUTPUT_DIRECTORY_PATH_OPTION = "-output=";
	
	private static String s_groupCode;
	private static String s_task;
	private static String s_inputFilePath;
	private static String s_outputDirectoryPath;

	private static void parseArguments(String[] args) {
		for (int index = 0; index < args.length; ++index) {
			String arg = args[index];
			if (arg.startsWith(GROUP_OPTION)) {
				s_groupCode = arg.substring(GROUP_OPTION.length());
			}
			else if (arg.startsWith(TASK_OPTION)) {
				s_task = arg.substring(TASK_OPTION.length());
			}
			else if (arg.startsWith(INPUT_FILE_PATH_OPTION)) {
				s_inputFilePath = arg.substring(INPUT_FILE_PATH_OPTION.length());
			}
			else if (arg.startsWith(OUTPUT_DIRECTORY_PATH_OPTION)) {
				s_outputDirectoryPath = arg.substring(OUTPUT_DIRECTORY_PATH_OPTION.length());
			}
		}
	}
	
	private static void generateSpriteExamples(String directoryPath) {
		System.out.println("Generating sprite examples");
		
		lct.spri.XMLConverter xmlConverter = new lct.spri.XMLConverter();
		lct.imag.ImageConverter imageConverter = new lct.imag.ImageConverter();
		lct.pack.XMLConverter packageXMLConverter = new lct.pack.XMLConverter();		
		
		if (!FileUtility.exists(directoryPath)) {
			FileUtility.createDirectory(directoryPath);
		}
		
		{
			lct.spri.Figure figure = lct.spri.Examples.createFigure();
			String fileName = figure.name + ".xml";
			String filePath = lct.util.PathUtility.combineFull(directoryPath, fileName);
			xmlConverter.storeFigure(figure, filePath);
		}
		
		{
			lct.spri.Animation animation = lct.spri.Examples.createAnimation1();
			String fileName = animation.name + ".xml";
			String filePath = lct.util.PathUtility.combineFull(directoryPath, fileName);
			xmlConverter.storeAnimation(animation, filePath);
		}
		{
			lct.spri.Animation animation = lct.spri.Examples.createAnimation2();
			String fileName = animation.name + ".xml";
			String filePath = lct.util.PathUtility.combineFull(directoryPath, fileName);
			xmlConverter.storeAnimation(animation, filePath);
		}
		{
			lct.spri.Animation animation = lct.spri.Examples.createAnimation3();
			String fileName = animation.name + ".xml";
			String filePath = lct.util.PathUtility.combineFull(directoryPath, fileName);
			xmlConverter.storeAnimation(animation, filePath);
		}
		
		{
			lct.imag.Texture textureStrip = lct.spri.Examples.createTextureStripA();
			String fileName = textureStrip.name + ".tga";
			String filePath = lct.util.PathUtility.combineFull(directoryPath, fileName);
			imageConverter.storeTextureTGA(textureStrip, filePath);
		}
		{
			lct.imag.Texture textureStrip = lct.spri.Examples.createTextureStripB();
			String fileName = textureStrip.name + ".tga";
			String filePath = lct.util.PathUtility.combineFull(directoryPath, fileName);
			imageConverter.storeTextureTGA(textureStrip, filePath);
		}
		{
			lct.imag.Texture textureStrip = lct.spri.Examples.createTextureStripC();
			String fileName = textureStrip.name + ".tga";
			String filePath = lct.util.PathUtility.combineFull(directoryPath, fileName);
			imageConverter.storeTextureTGA(textureStrip, filePath);
		}
		
		{
			lct.pack.Manifest manifest = lct.spri.Examples.createManifest(directoryPath);
			String fileName = manifest.name + ".xml";
			String filePath = lct.util.PathUtility.combineFull(directoryPath, fileName);
			packageXMLConverter.storeManifest(manifest, filePath);
		}
	}
	
	private static void plotSpriteAnimation(String animationFilePath, String imageDirectoryPath) {
		if (animationFilePath == null) {
			System.err.println("Invalid arguments for plot task.");
			System.exit(-1);
		}
		
		System.out.println("Plotting sprite animation");
		
		lct.spri.XMLConverter xmlConverter = new lct.spri.XMLConverter();
		lct.imag.ImageConverter imageConverter = new lct.imag.ImageConverter();
		
		if (!FileUtility.exists(imageDirectoryPath)) {
			FileUtility.createDirectory(imageDirectoryPath);
		}
		
		lct.spri.Animation animation = xmlConverter.loadAnimation(animationFilePath);
		lct.spri.AnimationUtility.calculateSlopes(animation);
		
		for (lct.spri.Animation.Track track: animation.trackVector) {
			String tgaFileName = lct.util.PathUtility.getFileNameNoExtension(animationFilePath) + "_" + track.targetName + "_" + track.targetPropertyType.toString() + ".tga";
			String tgaFilePath = lct.util.PathUtility.combineFull(imageDirectoryPath, tgaFileName);
			BufferedImage bufferedImage = lct.spri.AnimationUtility.plotToBufferedImage(track);
			lct.imag.Texture texture = lct.imag.TextureUtility.fromBufferedImage(bufferedImage);
			imageConverter.storeTextureTGA(texture, tgaFilePath);
		}
	}
	
	private static void generateFontExamples(String directoryPath) {
		System.out.println("Generating font examples");
		
		Font font = new Font("Arial", Font.BOLD, 16);
		final int CELL_WIDTH = 32;
		final int CELL_HEIGHT = 32;
		
		lct.font.XMLConverter xmlConverter = new lct.font.XMLConverter();
		lct.imag.ImageConverter imageConverter = new lct.imag.ImageConverter();
		lct.pack.XMLConverter packageXMLConverter = new lct.pack.XMLConverter();
		
		if (!FileUtility.exists(directoryPath)) {
			FileUtility.createDirectory(directoryPath);
		}
		
		{
			lct.font.Sheet sheet = lct.font.Examples.createSheet(CELL_WIDTH, CELL_HEIGHT, font);
			String fileName = sheet.name + ".xml";
			String filePath = lct.util.PathUtility.combineFull(directoryPath, fileName);
			xmlConverter.storeSheet(sheet, filePath);
		}
		
		{
			lct.imag.Texture sheetTexture = lct.font.Examples.createSheetTexture(CELL_WIDTH, CELL_HEIGHT, font);
			String fileName = sheetTexture.name + ".tga";
			String filePath = lct.util.PathUtility.combineFull(directoryPath, fileName);
			imageConverter.storeTextureTGA(sheetTexture, filePath);
		}
		
		{
			lct.imag.Texture gridTexture = lct.font.Examples.createGridTexture(CELL_WIDTH, CELL_HEIGHT);
			String fileName = gridTexture.name + ".tga";
			String filePath = lct.util.PathUtility.combineFull(directoryPath, fileName);
			imageConverter.storeTextureTGA(gridTexture, filePath);
		}
		
		{
			lct.pack.Manifest manifest = lct.font.Examples.createManifest(directoryPath);
			String fileName = manifest.name + ".xml";
			String filePath = lct.util.PathUtility.combineFull(directoryPath, fileName);
			packageXMLConverter.storeManifest(manifest, filePath);
		}
	}
	
	private static void generateSoundExamples(String directoryPath) {
		System.out.println("Generating sound examples");
		
		lct.soun.XMLConverter xmlConverter = new lct.soun.XMLConverter();
		lct.soun.SoundConverter soundConverter = new lct.soun.SoundConverter();
		lct.pack.XMLConverter packageXMLConverter = new lct.pack.XMLConverter();
		
		if (!FileUtility.exists(directoryPath)) {
			FileUtility.createDirectory(directoryPath);
		}
		
		{
			lct.soun.Clip clip = lct.soun.Examples.createToneClip(lct.soun.Examples.Tone.A_4);
			String fileName = clip.name + ".xml";
			String filePath = lct.util.PathUtility.combineFull(directoryPath, fileName);
			xmlConverter.storeClip(clip, filePath);
		}
		
		{
			lct.soun.Ramp ramp = lct.soun.Examples.createNoteRamp();
			String fileName = ramp.name + ".xml";
			String filePath = lct.util.PathUtility.combineFull(directoryPath, fileName);
			xmlConverter.storeRamp(ramp, filePath);
		}
		
		{
			lct.soun.Wave wave = lct.soun.Examples.createToneWave(lct.soun.Examples.Tone.A_4);
			String fileName = wave.name + ".wav";
			String filePath = lct.util.PathUtility.combineFull(directoryPath, fileName);
			soundConverter.storeWaveWAV(wave, filePath);
		}
		
		{
			lct.pack.Manifest manifest = lct.soun.Examples.createManifest(directoryPath);
			String fileName = manifest.name + ".xml";
			String filePath = lct.util.PathUtility.combineFull(directoryPath, fileName);
			packageXMLConverter.storeManifest(manifest, filePath);
		}
	}

	public static void main(String[] args) {
		parseArguments(args);
		
		if ((s_groupCode == null) || (s_task == null) || (s_outputDirectoryPath == null)) {
			System.err.println("Invalid arguments for asset helper");
			System.exit(-1);
		}
		
		String directoryPath = lct.util.PathUtility.combineFull(s_outputDirectoryPath, s_groupCode);
		directoryPath = lct.util.PathUtility.combineFull(directoryPath, s_task);
		 
		switch (s_groupCode) {
		case lct.spri.Constants.GROUP_CODE:
		{
			switch (s_task) {
			case TASK_EXAMPLE:
				generateSpriteExamples(directoryPath);
				break;
			case TASK_PLOT:
				plotSpriteAnimation(s_inputFilePath, directoryPath);
				break;
			default:
			{
				System.err.println("Unhandled task");
				System.exit(-1);
			}
			}
		}
		break;
		
		case lct.font.Constants.GROUP_CODE:
		{
			switch (s_task) {
			case TASK_EXAMPLE:
				generateFontExamples(directoryPath);
				break;
			default:
			{
				System.err.println("Unhandled task");
				System.exit(-1);
			}
			}
		}
		break;
		
		case lct.soun.Constants.GROUP_CODE:
		{
			switch (s_task) {
			case TASK_EXAMPLE:
				generateSoundExamples(directoryPath);
				break;
			default:
			{
				System.err.println("Unhandled task");
				System.exit(-1);
			}
			}
		}
		break;
		
		default:
		{
			System.err.println("Unhandled group code");
			System.exit(-1);
		}
		}
	}
	
}
