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

//namespace soun
}
//namespace lct
}

#endif//SOUN_ASSETS_H
