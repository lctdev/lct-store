#include <font/font_symbolWriter.h>
#include <font/font_symbolBuffer.h>
#include <font/font_assets.h>
#include <font/font_data.h>
#include <font/font_constants.h>

#include <util/util_indexMap.h>

#include <foun/foun_math.h>
#include <foun/foun_debug.h>

#include <string.h>

namespace lct
{
namespace font
{

/*
 * Internal Constants
 */
static const foun::FloatColor4 DEFAULT_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };

/*
 * Internal Functions
 */
inline GlyphData* FindGlyphData(SheetAsset* pSheetAsset, u16 characterCode)
{
	SheetData* pSheetData = pSheetAsset->pSheetData;
	util::IntegerIndexMap* pGlyphIndexMap = pSheetAsset->pGlyphIndexMap;

	s32 glyphIndex = pGlyphIndexMap->FindIndex(characterCode);
	GlyphData* pGlyphData = NULL;
	if (glyphIndex >= 0)
	{
		pGlyphData = pSheetAsset->pGlyphArray[glyphIndex].pGlyphData;
	}
	else if (pSheetData->glyphCount > 0)
	{
		pGlyphData = pSheetAsset->pGlyphArray[0].pGlyphData;
	}
	return pGlyphData;
}


/*
 * Public Instance
 */
SymbolWriter::SymbolWriter()
: m_pBuffer(NULL)
, m_cursor()
, m_horizontalAlign(HORIZONTAL_ALIGN_LEFT)
, m_verticalAlign(VERTICAL_ALIGN_NONE)
, m_color(DEFAULT_COLOR)
{
}

void SymbolWriter::SetBuffer(SymbolBuffer* pBuffer)
{
	m_pBuffer = pBuffer;
}

void SymbolWriter::SetCursor(const foun::Vector2& cursor)
{
	m_cursor = cursor;
}

void SymbolWriter::SetCursorX(f32 cursorX)
{
	m_cursor.x = cursorX;
}

void SymbolWriter::SetCursorY(f32 cursorY)
{
	m_cursor.y = cursorY;
}

void SymbolWriter::SetHorizontalAlign(HorizontalAlign horizontalAlign)
{
	m_horizontalAlign = horizontalAlign;
}

void SymbolWriter::SetVerticalAlign(VerticalAlign verticalAlign)
{
	m_verticalAlign = verticalAlign;
}

void SymbolWriter::SetColor(const foun::FloatColor4& color)
{
	m_color = color;
}

f32 SymbolWriter::MeasureStringWidth(const char* pString)
{
	u32 characterCount = static_cast<s32>(strlen(pString));
	f32 width = 0.0f;

	for (u32 characterIndex = 0; characterIndex < characterCount; ++characterIndex)
	{
		if (m_pBuffer->m_quadCount >= m_pBuffer->m_quadCapacity)
		{
			break;
		}

		char character = pString[characterIndex];
		u16 characterCode = static_cast<u16>(character);
		GlyphData* pGlyphData = FindGlyphData(m_pBuffer->m_pSheetAsset, characterCode);

		width += pGlyphData->advance;
	}
	return width;
}

void SymbolWriter::WriteString(const char* pString)
{
	SheetAsset* pSheetAsset = m_pBuffer->m_pSheetAsset;
	SheetData* pSheetData = pSheetAsset->pSheetData;

	u32 characterCount = static_cast<s32>(strlen(pString));
	f32 x = m_cursor.x;
	f32 y = m_cursor.y;
	f32 normalizedCellWidth = 1.0f / static_cast<f32>(pSheetData->horizontalCellCount);
	f32 normalizedCellHeight = 1.0f / static_cast<f32>(pSheetData->verticalCellCount);
	f32 horizontalOffset;
	f32 verticalOffset;

	switch (m_horizontalAlign)
	{
	case HORIZONTAL_ALIGN_MIDDLE:
	{
		f32 stringWidth = MeasureStringWidth(pString);

		horizontalOffset = -pSheetData->cellBaselineX - (stringWidth / 2.0f);
		horizontalOffset = foun::Floor(horizontalOffset);
	}
	break;

	default:
	{
		horizontalOffset = -pSheetData->cellBaselineX;
	}
	}

	switch (m_verticalAlign)
	{
	case VERTICAL_ALIGN_TOP:
	{
		verticalOffset = -pSheetData->cellBaselineY - pSheetData->ascent;
	}
	break;

	case VERTICAL_ALIGN_MIDDLE:
	{
		verticalOffset = -pSheetData->cellBaselineY - (((pSheetData->ascent + pSheetData->descent) / 2.0f) - pSheetData->descent);
		verticalOffset = foun::Floor(verticalOffset);
	}
	break;

	default:
	{
		verticalOffset = -pSheetData->cellBaselineY;
	}
	}

	for (u32 characterIndex = 0; characterIndex < characterCount; ++characterIndex)
	{
		if (m_pBuffer->m_quadCount >= m_pBuffer->m_quadCapacity)
		{
			LCT_TRACE("Insufficient quad capacity!\n");
			break;
		}

		char character = pString[characterIndex];
		u16 characterCode = static_cast<u16>(character);
		GlyphData* pGlyphData = FindGlyphData(m_pBuffer->m_pSheetAsset, characterCode);

		if (pGlyphData != NULL)
		{
			u32 vertexBaseIndex = m_pBuffer->m_quadCount * QUAD_VERTEX_COUNT;
			VertexData* pBaseVertex = m_pBuffer->m_pVertexDataArray + vertexBaseIndex;

			f32 leftX = x + horizontalOffset;
			f32 rightX = leftX + pSheetData->cellWidth;
			f32 bottomY = y + verticalOffset;
			f32 topY = bottomY + pSheetData->cellHeight;
			f32 leftS = pGlyphData->cellX * normalizedCellWidth;
			f32 rightS = leftS + normalizedCellWidth;
			f32 topT = 1.0f - (pGlyphData->cellY * normalizedCellHeight);
			f32 bottomT = topT - normalizedCellHeight;

			{
				VertexData* pVertex = pBaseVertex + 0;
				pVertex->x = leftX;
				pVertex->y = bottomY;
				pVertex->z = 0.0f;
				pVertex->s = leftS;
				pVertex->t = bottomT;
				pVertex->r = m_color.r;
				pVertex->g = m_color.g;
				pVertex->b = m_color.b;
				pVertex->a = m_color.a;
			}
			{
				VertexData* pVertex = pBaseVertex + 1;
				pVertex->x = rightX;
				pVertex->y = bottomY;
				pVertex->z = 0.0f;
				pVertex->s = rightS;
				pVertex->t = bottomT;
				pVertex->r = m_color.r;
				pVertex->g = m_color.g;
				pVertex->b = m_color.b;
				pVertex->a = m_color.a;
			}
			{
				VertexData* pVertex = pBaseVertex + 2;
				pVertex->x = rightX;
				pVertex->y = topY;
				pVertex->z = 0.0f;
				pVertex->s = rightS;
				pVertex->t = topT;
				pVertex->r = m_color.r;
				pVertex->g = m_color.g;
				pVertex->b = m_color.b;
				pVertex->a = m_color.a;
			}
			{
				VertexData* pVertex = pBaseVertex + 3;
				pVertex->x = leftX;
				pVertex->y = topY;
				pVertex->z = 0.0f;
				pVertex->s = leftS;
				pVertex->t = topT;
				pVertex->r = m_color.r;
				pVertex->g = m_color.g;
				pVertex->b = m_color.b;
				pVertex->a = m_color.a;
			}

			u32 indexBaseIndex = m_pBuffer->m_quadCount * QUAD_INDEX_COUNT;
			IndexData* pBaseIndex = m_pBuffer->m_pIndexDataArray + indexBaseIndex;

			pBaseIndex[0] = vertexBaseIndex + 0;
			pBaseIndex[1] = vertexBaseIndex + 1;
			pBaseIndex[2] = vertexBaseIndex + 2;
			pBaseIndex[3] = vertexBaseIndex + 2;
			pBaseIndex[4] = vertexBaseIndex + 3;
			pBaseIndex[5] = vertexBaseIndex + 0;

			++m_pBuffer->m_quadCount;

			x += pGlyphData->advance;
		}
	}
}

/*
 * Protected Instance
 */

//namespace spri
}
//namespace lct
}
