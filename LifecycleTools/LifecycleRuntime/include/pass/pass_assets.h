#ifndef PASS_ASSETS_H
#define PASS_ASSETS_H

#include <foun/foun_primitives.h>

#include <pack/pack_assets.h>
#include <pack/pack_assetHeader.h>

namespace lct
{
namespace pass
{

struct StringData;

struct StringAsset : public pack::Asset
{
	static const pack::AssetCode GROUP_CODE;
	static const pack::AssetCode TYPE_CODE;

	StringData* pStringData;
	char* pString;
};

//namespace pass
}
//namespace lct
}

#endif//PASS_ASSETS_H
