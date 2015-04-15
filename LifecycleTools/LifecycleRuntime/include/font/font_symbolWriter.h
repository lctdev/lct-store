#ifndef FONT_TEXT_WRITER_H
#define FONT_TEXT_WRITER_H

#include <foun/foun_primitives.h>
#include <foun/foun_vector.h>
#include <foun/foun_color.h>

#include <font/font_constants.h>

namespace lct
{

namespace font
{

class SymbolBuffer;

class SymbolWriter
{
public:
	SymbolWriter();

	void SetBuffer(SymbolBuffer* pBuffer);

	void SetCursor(const foun::Vector2& cursor);
	void SetCursorX(f32 cursorX);
	void SetCursorY(f32 cursorY);
	void SetHorizontalAlign(HorizontalAlign horizontalAlign);
	void SetVerticalAlign(VerticalAlign verticalAlign);
	void SetColor(const foun::FloatColor& color);

	f32 MeasureStringWidth(const char* pString);

	void WriteString(const char* pString);

protected:
	SymbolBuffer* m_pBuffer;

	foun::Vector2 m_cursor;
	HorizontalAlign m_horizontalAlign;
	VerticalAlign m_verticalAlign;
	foun::FloatColor m_color;
};

//namespace font
}
//namespace lct
}

#endif//FONT_TEXT_WRITER_H
