package lct.util;

import java.nio.file.Path;
import java.nio.file.Paths;

public class PathUtility {
	public static String combineFull(String directoryPath, String relativePath) {
		Path path = Paths.get(directoryPath, relativePath);
		path = path.toAbsolutePath();
		path = path.normalize();
		String filePath = path.toString();
		filePath = filePath.replace("\\", "/");
		return filePath;
	}
	
	public static String getDirectoryFull(String filePath) {
		Path path = Paths.get(filePath);		
		path = path.toAbsolutePath();
		path = path.normalize();
		path = path.getParent();
		String directoryPath = path.toString();
		directoryPath = directoryPath.replace("\\", "/");
		return directoryPath;
	}
	
	public static String getFileNameNoExtension(String filePath) {
		Path path = Paths.get(filePath);
		path = path.getFileName();
		String fileName = path.toString();
		fileName = fileName.substring(0, fileName.lastIndexOf("."));
		return fileName;
	}
	
	public static String makeRelative(String directoryPath, String filePath) {
		Path path = Paths.get(directoryPath);
		path = path.relativize(Paths.get(filePath));
		String relativePath = path.toString();
		relativePath.replace("\\", "/");
		return relativePath;
	}
}
