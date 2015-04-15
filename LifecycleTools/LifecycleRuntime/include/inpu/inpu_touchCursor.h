#ifndef INPU_TOUCH_CURSOR_H
#define INPU_TOUCH_CURSOR_H

#include <inpu/inpu_cursor.h>

namespace lct
{
namespace inpu
{

class TouchReader;

class TouchCursor : public Cursor
{
public:
	TouchCursor();
	virtual ~TouchCursor();

	void SetReader(TouchReader* pReader);

	virtual s16 GetX();
	virtual s16 GetY();
	virtual bool IsDown();
	virtual bool IsPress();
	virtual bool IsRelease();

private:
	TouchReader* m_pReader;
};

//namespace inpu
}
//namespace lct
}

#endif//INPU_TOUCH_CURSOR_H
