#ifndef INPU_TOUCH_READER_H
#define INPU_TOUCH_READER_H

#include <foun/foun_primitives.h>

#include <inpu/inpu_constants.h>

namespace lct
{
namespace foun
{
struct PlatformMessage;
}

namespace inpu
{

class TouchReader
{
public:
	TouchReader();

	void PrepareValues();
	bool HandlePlatformMessage(const foun::PlatformMessage& platformMessage);

	s16 GetX() { return m_x; }
	s16 GetY() { return m_y; }
	bool IsDown();
	bool IsPress();
	bool IsRelease();

private:
	s16 m_x;
	s16 m_y;
	bool m_prevDown;
	bool m_currDown;
};

//namespace inpu
}
//namespace lct
}

#endif//INPU_TOUCH_READER_H
