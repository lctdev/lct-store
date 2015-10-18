package lct.soun;

import lct.util.DataException;

public class AssetProcessor implements lct.pack.AssetProcessor {
	SoundConverter m_soundConverter = new SoundConverter();
	XMLConverter m_xmlConverter = new XMLConverter();
	BinaryConverter m_binaryConverter = new BinaryConverter();
	
	public AssetProcessor() {
	}
	
	public String getGroupCode() {
		return Constants.GROUP_CODE;
	}
	
	public void processAsset(String typeCode, String inputFilePath, String outputFilePath, boolean bigEndian) throws DataException {
		switch (typeCode) {
		case Constants.WAVE_TYPE_CODE:
		{
			Wave wave = m_soundConverter.loadWaveWAV(inputFilePath);
			if (wave == null) {
				throw new DataException("Error loading wave file");
			}
			
			m_binaryConverter.storeWave(wave, outputFilePath, bigEndian);
		}
		break;
		
		case Constants.CLIP_TYPE_CODE:
		{
			Clip clip = m_xmlConverter.loadClip(inputFilePath);
			if (clip == null) {
				throw new DataException("Error loading clip file");
			}
			
			m_binaryConverter.storeClip(clip, outputFilePath, bigEndian);
		}
		break;
		
		case Constants.RAMP_TYPE_CODE:
		{
			Ramp ramp = m_xmlConverter.loadRamp(inputFilePath);
			if (ramp == null) {
				throw new DataException("Error loading ramp file");
			}
			
			m_binaryConverter.storeRamp(ramp, outputFilePath, bigEndian);
		}
		break;
		
		case Constants.SEQUENCE_TYPE_CODE:
		{
			Sequence sequence = m_xmlConverter.loadSequence(inputFilePath);
			if (sequence == null) {
				throw new DataException("Error loading sequence file");
			}
			
			m_binaryConverter.storeSequence(sequence, outputFilePath, bigEndian);
		}
		break;
		
		default:
		{
			throw new DataException("Unhandled type code: " + typeCode);
		}
		}
	}
}