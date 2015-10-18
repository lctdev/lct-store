#ifndef SOUN_ASSETS_H
#define SOUN_ASSETS_H

#include <foun/foun_primitives.h>

#include <pack/pack_assets.h>
#include <pack/pack_assetHeader.h>

namespace lct
{
namespace audi
{
struct WaveResource;
}

namespace soun
{

struct WaveData;
struct ClipData;
struct SegmentData;
struct RampData;
struct TrackData;
struct KeyData;
struct SequenceData;
struct ClipReferenceData;
struct RampReferenceData;
struct TimelineData;
struct ActionData;

struct WaveAsset : public pack::Asset
{
	static const pack::AssetCode GROUP_CODE;
	static const pack::AssetCode TYPE_CODE;

	WaveData* pWaveData;
	void* pWaveBinary;

	audi::WaveResource* pWaveResource;
};

struct ClipAsset : public pack::Asset
{
	static const pack::AssetCode GROUP_CODE;
	static const pack::AssetCode TYPE_CODE;

	ClipData* pClipData;

	struct Segment
	{
		SegmentData* pSegmentData;

		WaveAsset* pWaveAsset;
	};
	Segment* pSegmentArray;
};

struct RampAsset : public pack::Asset
{
	static const pack::AssetCode GROUP_CODE;
	static const pack::AssetCode TYPE_CODE;

	RampData* pRampData;

	struct Track
	{
		TrackData* pTrackData;
		KeyData* pKeyDataArray;
	};
	Track* pTrackArray;
};

struct SequenceAsset : public pack::Asset
{
	static const pack::AssetCode GROUP_CODE;
	static const pack::AssetCode TYPE_CODE;

	SequenceData* pSequenceData;

	struct ClipReference
	{
		ClipReferenceData* pClipReferenceData;

		ClipAsset* pClipAsset;
	};
	ClipReference* pClipReferenceArray;

	struct RampReference
	{
		RampReferenceData* pRampReferenceData;

		RampAsset* pRampAsset;
	};
	RampReference* pRampReferenceArray;

	struct Action
	{
		ActionData* pActionData;
	};
	struct Timeline
	{
		TimelineData* pTimelineData;
		
		Action* pActionArray;
	};
	Timeline* pTimelineArray;
};

//namespace soun
}
//namespace lct
}

#endif//SOUN_ASSETS_H
