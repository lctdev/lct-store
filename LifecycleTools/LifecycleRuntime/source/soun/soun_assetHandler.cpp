#include <soun/soun_assetHandler.h>
#include <soun/soun_assets.h>
#include <soun/soun_data.h>

#include <audi/audi_device.h>
#include <audi/audi_resources.h>

#include <pack/pack_assetHeader.h>
#include <pack/pack_assetContainer.h>

#include <util/util_binaryReader.h>
#include <util/util_indexMap.h>

#include <string.h>

namespace lct
{
namespace soun
{

/*
* Internal Constants
*/

/*
* Public Instance
*/
AssetHandler::AssetHandler()
	: m_pAllocator(NULL)
	, m_pAudioDevice(NULL)
	, m_pAssetContainer(NULL)
{
}

AssetHandler::~AssetHandler()
{
}

void AssetHandler::SetAllocator(lct::foun::Allocator* pAllocator)
{
	m_pAllocator = pAllocator;
}

void AssetHandler::SetAudioDevice(audi::Device* pAudioDevice)
{
	m_pAudioDevice = pAudioDevice;
}

void AssetHandler::SetAssetContainer(pack::AssetContainer* pAssetContainer)
{
	m_pAssetContainer = pAssetContainer;
}

bool AssetHandler::LoadAsset(pack::AssetHeader& assetHeader, util::BinaryReader& binaryReader)
{
	bool assetLoaded = false;
	if ((assetHeader.groupCode.numeric == GROUP_CODE.numeric) && (assetHeader.version == VERSION))
	{
		if (assetHeader.typeCode.numeric == WAVE_TYPE_CODE.numeric)
		{
			WaveAsset* pWaveAsset = LoadWaveAsset(binaryReader);
			m_pAssetContainer->AddAsset(pWaveAsset, pWaveAsset->pWaveData->name);
			assetLoaded = true;
		}
		else if (assetHeader.typeCode.numeric == CLIP_TYPE_CODE.numeric)
		{
			ClipAsset* pClipAsset = LoadClipAsset(binaryReader);
			m_pAssetContainer->AddAsset(pClipAsset, pClipAsset->pClipData->name);
			assetLoaded = true;
		}
		else if (assetHeader.typeCode.numeric == RAMP_TYPE_CODE.numeric)
		{
			RampAsset* pRampAsset = LoadRampAsset(binaryReader);
			m_pAssetContainer->AddAsset(pRampAsset, pRampAsset->pRampData->name);
			assetLoaded = true;
		}
		else if (assetHeader.typeCode.numeric == SEQUENCE_TYPE_CODE.numeric)
		{
			SequenceAsset* pSequenceAsset = LoadSequenceAsset(binaryReader);
			m_pAssetContainer->AddAsset(pSequenceAsset, pSequenceAsset->pSequenceData->name);
			assetLoaded = true;
		}
	}
	return assetLoaded;
}

WaveAsset* AssetHandler::LoadWaveAsset(util::BinaryReader& binaryReader)
{
	WaveData* pWaveData = binaryReader.ReadType<WaveData>();

	void* pWaveBinary = binaryReader.Read(pWaveData->size);

	audi::WaveResource* pWaveResource = m_pAllocator->AllocType<audi::WaveResource>();
	memset(pWaveResource, 0, sizeof(audi::WaveResource));

	WaveAsset* pWaveAsset = m_pAllocator->AllocType<WaveAsset>();
	pWaveAsset->pWaveData = pWaveData;
	pWaveAsset->pWaveBinary = pWaveBinary;
	pWaveAsset->pWaveResource = pWaveResource;

	return pWaveAsset;
}

ClipAsset* AssetHandler::LoadClipAsset(util::BinaryReader& binaryReader)
{
	ClipData* pClipData = binaryReader.ReadType<ClipData>();

	ClipAsset::Segment* pSegmentArray = m_pAllocator->AllocTypeArray<ClipAsset::Segment>(pClipData->segmentCount);
	for (u32 segmentIndex = 0; segmentIndex < pClipData->segmentCount; ++segmentIndex)
	{
		SegmentData* pSegmentData = binaryReader.ReadType<SegmentData>();

		ClipAsset::Segment* pSegment = pSegmentArray + segmentIndex;
		pSegment->pSegmentData = pSegmentData;
	}

	ClipAsset* pClipAsset = m_pAllocator->AllocType<ClipAsset>();
	pClipAsset->pClipData = pClipData;
	pClipAsset->pSegmentArray = pSegmentArray;

	return pClipAsset;
}

RampAsset* AssetHandler::LoadRampAsset(util::BinaryReader& binaryReader)
{
	RampData* pRampData = binaryReader.ReadType<RampData>();

	RampAsset::Track* pTrackArray = m_pAllocator->AllocTypeArray<RampAsset::Track>(pRampData->trackCount);
	for (u32 trackIndex = 0; trackIndex < pRampData->trackCount; ++trackIndex)
	{
		TrackData* pTrackData = binaryReader.ReadType<TrackData>();
		KeyData* pKeyDataArray = binaryReader.ReadTypeArray<KeyData>(pTrackData->keyCount);

		RampAsset::Track* pTrack = pTrackArray + trackIndex;
		pTrack->pTrackData = pTrackData;
		pTrack->pKeyDataArray = pKeyDataArray;
	}

	RampAsset* pRampAsset = m_pAllocator->AllocType<RampAsset>();
	pRampAsset->pRampData = pRampData;
	pRampAsset->pTrackArray = pTrackArray;

	return pRampAsset;
}

SequenceAsset* AssetHandler::LoadSequenceAsset(util::BinaryReader& binaryReader)
{
	SequenceData* pSequenceData = binaryReader.ReadType<SequenceData>();

	SequenceAsset::ClipReference* pClipReferenceArray = m_pAllocator->AllocTypeArray<SequenceAsset::ClipReference>(pSequenceData->clipReferenceCount);
	for (u32 clipReferenceIndex = 0; clipReferenceIndex < pSequenceData->clipReferenceCount; ++clipReferenceIndex)
	{
		ClipReferenceData* pClipReferenceData = binaryReader.ReadType<ClipReferenceData>();

		SequenceAsset::ClipReference* pClipReference = pClipReferenceArray + clipReferenceIndex;
		pClipReference->pClipReferenceData = pClipReferenceData;
	}

	SequenceAsset::RampReference* pRampReferenceArray = m_pAllocator->AllocTypeArray<SequenceAsset::RampReference>(pSequenceData->rampReferenceCount);
	for (u32 rampReferenceIndex = 0; rampReferenceIndex < pSequenceData->rampReferenceCount; ++rampReferenceIndex)
	{
		RampReferenceData* pRampReferenceData = binaryReader.ReadType<RampReferenceData>();

		SequenceAsset::RampReference* pRampReference = pRampReferenceArray + rampReferenceIndex;
		pRampReference->pRampReferenceData = pRampReferenceData;
	}

	SequenceAsset::Timeline* pTimelineArray = m_pAllocator->AllocTypeArray<SequenceAsset::Timeline>(pSequenceData->timelineCount);
	for (u32 timelineIndex = 0; timelineIndex < pSequenceData->timelineCount; ++timelineIndex)
	{
		TimelineData* pTimelineData = binaryReader.ReadType<TimelineData>();

		SequenceAsset::Action* pActionArray = m_pAllocator->AllocTypeArray<SequenceAsset::Action>(pTimelineData->actionCount);
		for (u32 actionIndex = 0; actionIndex < pTimelineData->actionCount; ++actionIndex)
		{
			ActionData* pActionData = binaryReader.ReadType<ActionData>();
			u32 typeSize = sizeof(ActionData);
			switch (pActionData->type)
			{
			case ACTION_TYPE_BEGIN_CLIP:
			{
				typeSize = sizeof(BeginClipActionData);
			}
			break;
			case ACTION_TYPE_END_CLIP:
			{
				typeSize = sizeof(EndClipActionData);
			}
			break;
			case ACTION_TYPE_SET_RAMP:
			{
				typeSize = sizeof(SetRampActionData);
			}
			}
			u32 remainingSize = typeSize - sizeof(ActionData);
			binaryReader.Read(remainingSize);

			SequenceAsset::Action* pAction = pActionArray + actionIndex;
			pAction->pActionData = pActionData;
		}

		SequenceAsset::Timeline* pTimeline = pTimelineArray + timelineIndex;
		pTimeline->pTimelineData = pTimelineData;
		pTimeline->pActionArray = pActionArray;
	}

	SequenceAsset* pSequenceAsset = m_pAllocator->AllocType<SequenceAsset>();
	pSequenceAsset->pSequenceData = pSequenceData;
	pSequenceAsset->pClipReferenceArray = pClipReferenceArray;
	pSequenceAsset->pRampReferenceArray = pRampReferenceArray;
	pSequenceAsset->pTimelineArray = pTimelineArray;

	return pSequenceAsset;
}

void AssetHandler::FixupAllAssets()
{
	for (pack::AssetIterator<ClipAsset> clipIterator = m_pAssetContainer->GetIterator<ClipAsset>(); !clipIterator.IsEnd(); clipIterator.Next())
	{
		ClipAsset* pClipAsset = clipIterator.GetAsset();
		ClipData* pClipData = pClipAsset->pClipData;
		for (u32 segmentIndex = 0; segmentIndex < pClipData->segmentCount; ++segmentIndex)
		{
			ClipAsset::Segment* pSegment = pClipAsset->pSegmentArray + segmentIndex;
			SegmentData* pSegmentData = pSegment->pSegmentData;
			WaveAsset* pWaveAsset = m_pAssetContainer->FindAsset<WaveAsset>(pSegmentData->waveName);
			pSegment->pWaveAsset = pWaveAsset;
		}
	}

	for (pack::AssetIterator<SequenceAsset> sequenceIterator = m_pAssetContainer->GetIterator<SequenceAsset>(); !sequenceIterator.IsEnd(); sequenceIterator.Next())
	{
		SequenceAsset* pSequenceAsset = sequenceIterator.GetAsset();
		SequenceData* pSequenceData = pSequenceAsset->pSequenceData;
		for (u32 clipReferenceIndex = 0; clipReferenceIndex < pSequenceData->clipReferenceCount; ++clipReferenceIndex)
		{
			SequenceAsset::ClipReference* pClipReference = pSequenceAsset->pClipReferenceArray + clipReferenceIndex;
			ClipReferenceData* pClipReferenceData = pClipReference->pClipReferenceData;
			ClipAsset* pClipAsset = m_pAssetContainer->FindAsset<ClipAsset>(pClipReferenceData->clipName);
			pClipReference->pClipAsset = pClipAsset;
		}

		for (u32 rampReferenceIndex = 0; rampReferenceIndex < pSequenceData->rampReferenceCount; ++rampReferenceIndex)
		{
			SequenceAsset::RampReference* pRampReference = pSequenceAsset->pRampReferenceArray + rampReferenceIndex;
			RampReferenceData* pRampReferenceData = pRampReference->pRampReferenceData;
			RampAsset* pRampAsset = m_pAssetContainer->FindAsset<RampAsset>(pRampReferenceData->rampName);
			pRampReference->pRampAsset = pRampAsset;
		}
	}
}

void AssetHandler::AcquireAllAssetResources()
{
	for (pack::AssetIterator<WaveAsset> waveIterator = m_pAssetContainer->GetIterator<WaveAsset>(); !waveIterator.IsEnd(); waveIterator.Next())
	{
		WaveAsset* pWaveAsset = waveIterator.GetAsset();

		WaveData* pWaveData = pWaveAsset->pWaveData;
		void* pWaveBinary = pWaveAsset->pWaveBinary;
		audi::WaveResource* pWaveResource = pWaveAsset->pWaveResource;

		audi::WaveSetupParameters waveSetupParameters;
		waveSetupParameters.pWaveResource = pWaveResource;
		waveSetupParameters.pWaveBinary = pWaveBinary;
		waveSetupParameters.size = pWaveData->size;
		waveSetupParameters.channelCount = pWaveData->channelCount;
		waveSetupParameters.sampleSize = pWaveData->sampleSize;
		waveSetupParameters.sampleRate = pWaveData->sampleRate;
		m_pAudioDevice->AcquireWaveResource(waveSetupParameters);
	}
}

void AssetHandler::ReleaseAllAssetResources()
{
	for (pack::AssetIterator<WaveAsset> waveIterator = m_pAssetContainer->GetIterator<WaveAsset>(); !waveIterator.IsEnd(); waveIterator.Next())
	{
		WaveAsset* pWaveAsset = waveIterator.GetAsset();

		audi::WaveResource* pWaveResource = pWaveAsset->pWaveResource;

		audi::WaveSetupParameters waveSetupParameters;
		waveSetupParameters.pWaveResource = pWaveResource;
		m_pAudioDevice->ReleaseWaveResource(waveSetupParameters);
	}
}

//namespace soun
}
//namespace lct
}
