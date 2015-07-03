package lct.pack;

import lct.util.PathUtility;
import lct.util.XMLUtility;

import java.util.Vector;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

public class XMLConverter {
	public XMLConverter() {		
	}
	
	public void storeManifest(Manifest manifest, String filePath) {
		try {
			String directoryPath = PathUtility.getDirectoryFull(filePath);
			
			Document document = XMLUtility.createDocument();
			
			Element manifestElement = document.createElement("pack:manifest");
			{
				Element assetEntriesElement = document.createElement("assetEntries");
				for (Manifest.AssetEntry assetEntry : manifest.assetEntryVector) {
					Element assetEntryElement = document.createElement("assetEntry");
					assetEntryElement.setAttribute("group", assetEntry.groupCode);
					assetEntryElement.setAttribute("type", assetEntry.typeCode);
					{
						String assetRelativePath = PathUtility.makeRelative(directoryPath, assetEntry.filePath);
						XMLUtility.writeStringElement(document, "filePath", assetRelativePath, assetEntryElement);
					}
					assetEntriesElement.appendChild(assetEntryElement);
				}
				manifestElement.appendChild(assetEntriesElement);
			}
			document.appendChild(manifestElement);
			
			XMLUtility.storeDocument(document, filePath);
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			System.out.println(e.getStackTrace());
		}
	}
	
	public Manifest loadManifest(String filePath) {
		try {
			String directoryPath = PathUtility.getDirectoryFull(filePath);
			
			Document document = XMLUtility.loadDocument(filePath);
			
			Element manifestElement = (Element)document.getElementsByTagName("pack:manifest").item(0);
			Manifest manifest = new Manifest();
			manifest.name = PathUtility.getFileNameNoExtension(filePath);
			{
				Element assetEntriesElement = (Element)manifestElement.getElementsByTagName("assetEntries").item(0);
				NodeList assetEntryNodeList = assetEntriesElement.getElementsByTagName("assetEntry");
				manifest.assetEntryVector = new Vector<Manifest.AssetEntry>();
				for (int assetEntryIndex = 0; assetEntryIndex < assetEntryNodeList.getLength(); ++assetEntryIndex) {
					Element assetEntryElement = (Element)assetEntryNodeList.item(assetEntryIndex);					
					Manifest.AssetEntry assetEntry = new Manifest.AssetEntry();
					assetEntry.groupCode = assetEntryElement.getAttribute("group");
					assetEntry.typeCode = assetEntryElement.getAttribute("type");
					{						
						String assetRelativeFilePath = XMLUtility.readStringElement(assetEntryElement, "filePath", 0);
						assetEntry.filePath = PathUtility.combineFull(directoryPath, assetRelativeFilePath);
					}
					manifest.assetEntryVector.add(assetEntry);
				}
			}
			return manifest;
		}
		catch (Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
			
			return null;
		}
	}
}