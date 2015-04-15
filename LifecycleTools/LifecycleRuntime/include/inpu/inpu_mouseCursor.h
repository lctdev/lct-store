#ifndef INPU_MOUSE_CURSOR_H
#define INPU_MOUSE_CURSOR_H

#include <inpu/inpu_cursor.h>

namespace lct
{
namespace inpu
{

class MouseReader;

class MouseCursor : public Cursor
{
public:
	MouseCursor();
	virtual ~MouseCursor();

	void SetReader(MouseReader* pReader);

	virtual s16 GetX();
	virtual s16 GetY();
	virtual bool IsDown();
	virtual bool IsPress();
	virtual bool IsRelease();

private:
	MouseReader* m_pReader;
};

//namespace inpu
}
//namespace lct
}

#endif//INPU_MOUSE_CURSOR_H
