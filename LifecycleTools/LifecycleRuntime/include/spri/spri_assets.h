#ifndef SPRI_ASSETS_H
#define SPRI_ASSETS_H

#include <foun/foun_primitives.h>

#include <pack/pack_assets.h>
#include <pack/pack_assetHeader.h>

namespace lct
{
namespace util
{
class StringIndexMap;
}
namespace imag
{
struct TextureTableAsset;
}

namespace spri
{

struct PartData;
struct JointData;
struct PatternData;
struct FigureData;
struct SkinData;
struct StripData;
struct AnimationData;
struct TrackData;
struct KeyData;

struct FigureAsset : public pack::Asset
{
	static const pack::AssetCode GROUP_CODE;
	static const pack::AssetCode TYPE_CODE;

	FigureData* pFigureData;

	struct Part
	{
		PartData* pPartData;
	};
	Part* pPartArray;
	struct Joint
	{
		JointData* pJointData;
	};
	Joint* pJointArray;
	struct Pattern
	{
		PatternData* pPatternData;

		imag::TextureTableAsset* pTextureTableAsset;
	};
	Pattern* pPatternArray;

	util::StringIndexMap* pPartIndexMap;
	util::StringIndexMap* pJointIndexMap;
	util::StringIndexMap* pPatternIndexMap;
};

struct SkinAsset : public pack::Asset
{
	static const pack::AssetCode GROUP_CODE;
	static const pack::AssetCode TYPE_CODE;

	SkinData* pSkinData;

	struct Strip
	{
		StripData* pStripData;

		imag::TextureTableAsset* pTextureTableAsset;
	};
	Strip* pStripArray;
};

struct AnimationAsset : public pack::Asset
{
	static const pack::AssetCode GROUP_CODE;
	static const pack::AssetCode TYPE_CODE;

	AnimationData* pAnimationData;

	struct Track
	{
		TrackData* pTrackData;
		KeyData* pKeyDataArray;
	};
	Track* pTrackArray;
};

//namespace spri
}
//namespace lct
}

#endif//SPRI_ASSETS_H
