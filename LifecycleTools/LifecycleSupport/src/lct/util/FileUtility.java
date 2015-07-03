package lct.util;

import java.io.File;
import java.io.FileOutputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Vector;

public class FileUtility {
	public static boolean exists(String filePath) {
		File file = new File(filePath);
		boolean exists = file.exists();
		return exists;
	}
	
	public static boolean isOutdated(String destinationFilePath, String sourceFilePath) {
		File destinationFile = new File(destinationFilePath);
		File sourceFile = new File(sourceFilePath);
		boolean isOutdated = destinationFile.lastModified() < sourceFile.lastModified();
		return isOutdated;
	}
	
	public static boolean createDirectory(String directoryPath) {
		File directory = new File(directoryPath);
		boolean success = directory.mkdirs();
		return success;
	}
	
	public static void consolidateFiles(Vector<String> filePathVector, String consolidatedFilePath) {
		try {
			FileOutputStream packageOutputStream = new FileOutputStream(consolidatedFilePath);
			
			for (String filePath: filePathVector) {
				Path path = Paths.get(filePath);
				byte[] byteArray = Files.readAllBytes(path);
				packageOutputStream.write(byteArray);
			}
			
			packageOutputStream.close();
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			System.out.println(e.getStackTrace());
		}
	}
}
