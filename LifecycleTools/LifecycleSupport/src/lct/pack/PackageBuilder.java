package lct.pack;

import java.util.HashMap;
import java.util.Vector;

import lct.util.*;

public class PackageBuilder {
	
	private XMLConverter m_xmlConverter = new XMLConverter();
	
	private HashMap<String, AssetProcessor> m_assetProcessorMap = new HashMap<String, AssetProcessor>();
	
	private static class ProcessItem {
		public String groupCode;
		public String typeCode;
		public String name;
		public String inputFilePath;
		public String outputFilePath;
	}
	private Vector<ProcessItem> m_processItemVector = new Vector<ProcessItem>();
	
	private String createAssetReadout(ProcessItem processItem) {
		String assetReadout = "[ " + processItem.groupCode + " - " + processItem.typeCode + " - " + processItem.name + " ]";
		return assetReadout;
	}
	
	public PackageBuilder() {
	}
	
	public void addAssetProcessor(AssetProcessor assetProcessor) {
		m_assetProcessorMap.put(assetProcessor.getGroupCode(), assetProcessor);
	}
	
	public void buildPackage(String manifestFilePath, String packageFilePath, String tempDirectoryPath, boolean bigEndian) throws DataException {
		// load the manifest file
		System.out.println("Building package: " + PathUtility.getFileNameNoExtension(manifestFilePath));
		Manifest manifest = m_xmlConverter.loadManifest(manifestFilePath);
		if (manifest == null) {
			throw new DataException("Error Loading manifest file");
		}
		
		// store information for each asset entry
		Vector<String> outputFilePathVector = new Vector<String>();
		for (Manifest.AssetEntry assetEntry : manifest.assetEntryVector) {
			ProcessItem processItem = new ProcessItem();
			
			processItem.groupCode = assetEntry.groupCode;
			processItem.typeCode = assetEntry.typeCode;
			processItem.name = PathUtility.getFileNameNoExtension(assetEntry.filePath);
			
			processItem.inputFilePath = assetEntry.filePath;
			
			String assetTempDirectory = PathUtility.combineFull(tempDirectoryPath, assetEntry.groupCode);
			assetTempDirectory = PathUtility.combineFull(assetTempDirectory, assetEntry.typeCode);
			String outputFileName = PathUtility.getFileNameNoExtension(assetEntry.filePath) + ".bin";
			String outputFilePath = PathUtility.combineFull(assetTempDirectory, outputFileName);
			processItem.outputFilePath = outputFilePath;
			
			m_processItemVector.add(processItem);
			
			outputFilePathVector.add(outputFilePath);
		}
		
		// consider each item
		boolean anyAssetProcessed = false;		
		for (ProcessItem processItem : m_processItemVector) {
			AssetProcessor assetProcessor = m_assetProcessorMap.get(processItem.groupCode);
			if (assetProcessor == null) {
				throw new DataException("No processor registered for group code: " + processItem.groupCode);
			}
			
			if (!FileUtility.exists(processItem.inputFilePath)) {
				throw new DataException("Referenced asset file does not exist: " + processItem.inputFilePath);
			}
			
			boolean processAsset = false;
			if (!FileUtility.exists(processItem.outputFilePath)) {
				processAsset = true;
			}			
			else {
				Vector<String> checkFileVector = assetProcessor.getDependencies(processItem.typeCode, processItem.inputFilePath);
				checkFileVector.add(processItem.inputFilePath);
				for (String checkFilePath : checkFileVector) {
					if (FileUtility.isOutdated(processItem.outputFilePath, checkFilePath)) {
						processAsset = true;
						break;
					}
				}				
			}
			// TODO: check version?
			
			if (processAsset) {
				String assetTempDirectory = PathUtility.getDirectoryFull(processItem.outputFilePath);
				if (!FileUtility.exists(assetTempDirectory)) {
					FileUtility.createDirectory(assetTempDirectory);
				}
				
				System.out.println("Processing asset: " + createAssetReadout(processItem));
				assetProcessor.processAsset(processItem.typeCode, processItem.inputFilePath, processItem.outputFilePath, bigEndian);
				anyAssetProcessed = true;
			}
			else {
				System.out.println("Asset up to date: "+ createAssetReadout(processItem));
			}
		}
		
		boolean conslidatePackage = anyAssetProcessed || !FileUtility.exists(packageFilePath);
		
		if (conslidatePackage) {
			System.out.println("Consolidating package files");
			FileUtility.consolidateFiles(outputFilePathVector, packageFilePath);
		}
		else {
			System.out.println("Package up to date");
		}
	}
	
}
