#ifndef FONT_DATA_H
#define FONT_DATA_H

#include <foun/foun_primitives.h>

#include <font/font_constants.h>

namespace lct
{
namespace font
{

struct VertexData
{
	f32 x;
	f32 y;
	f32 z;
	f32 s;
	f32 t;
	f32 r;
	f32 g;
	f32 b;
	f32 a;
};

typedef u16 IndexData;

struct GlyphData
{
	u16 characterCode;
	u16 padding;
	f32 cellX;
	f32 cellY;
	f32 width;
	f32 leftBearing;
	f32 rightBearing;
	f32 advance;
};

struct SheetData
{
	char name[MAX_NAME_SIZE];
	char textureName[MAX_NAME_SIZE];
	f32 cellWidth;
	f32 cellHeight;
	f32 cellBaselineX;
	f32 cellBaselineY;
	f32 ascent;
	f32 descent;
	f32 lineHeight;
	u32 horizontalCellCount;
	u32 verticalCellCount;
	u32 glyphCount;
};

//namespace font
}
//namespace lct
}

#endif//FONT_DATA_H
