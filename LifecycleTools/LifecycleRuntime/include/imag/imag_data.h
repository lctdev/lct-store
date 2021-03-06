#ifndef IMAG_DATA_H
#define IMAG_DATA_H

#include <foun/foun_primitives.h>

#include <imag/imag_constants.h>

namespace lct
{
namespace imag
{

struct TexelRGBAData
{
	u8 r;
	u8 g;
	u8 b;
	u8 a;
};

struct TextureData
{
	char name[MAX_NAME_SIZE];
	u32 width;
	u32 height;
	u32 size;
};

struct TextureTableData
{
	char name[MAX_NAME_SIZE];
	u32 width;
	u32 height;
	u32 size;
	u32 count;
};

struct TextureRegionData
{
	char name[MAX_NAME_SIZE];
	f32 translateS;
	f32 translateT;
	f32 scaleS;
	f32 scaleT;
};

struct TextureAtlasData
{
	char name[MAX_NAME_SIZE];
	u32 width;
	u32 height;
	u32 size;
	u32 regionCount;
};

//namespace imag
}
//namespace lct
}

#endif//IMAG_DATA_H
