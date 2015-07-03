package lct.pass;

import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

import lct.util.PathUtility;

public class PassthroughConverter {
	public StringAsset loadString(String filePath) {
		try {
			Path path = Paths.get(filePath);
			byte[] byteArray = Files.readAllBytes(path);
			
			StringAsset stringAsset = new StringAsset();
			stringAsset.name = PathUtility.getFileNameNoExtension(filePath);
			stringAsset.string = new String(byteArray, StandardCharsets.US_ASCII);
			
			return stringAsset;
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
			return null;
		}
	}
}
