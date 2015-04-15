#ifndef INPU_CURSOR_H
#define INPU_CURSOR_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace inpu
{

class Cursor
{
public:
	Cursor();
	virtual ~Cursor();

	virtual s16 GetX() = 0;
	virtual s16 GetY() = 0;
	virtual bool IsDown() = 0;
	virtual bool IsPress() = 0;
	virtual bool IsRelease() = 0;
};

//namespace inpu
}
//namespace lct
}

#endif//INPU_CURSOR_H
