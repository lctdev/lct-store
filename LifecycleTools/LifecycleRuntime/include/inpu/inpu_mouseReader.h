#ifndef INPU_MOUSE_READER_H
#define INPU_MOUSE_READER_H

#include <foun/foun_primitives.h>
#include <foun/foun_flags.h>

#include <inpu/inpu_constants.h>

namespace lct
{
namespace foun
{
struct PlatformMessage;
}

namespace inpu
{

class MouseReader
{
public:
	MouseReader();

	void PrepareValues();
	bool HandlePlatformMessage(const foun::PlatformMessage& platformMessage);

	s16 GetX() { return m_x; }
	s16 GetY() { return m_y; }
	bool IsButtonDown(MouseButton button);
	bool IsButtonPress(MouseButton button);
	bool IsButtonRelease(MouseButton button);

private:
	s16 m_x;
	s16 m_y;
	foun::Flags<u32> m_prevButtonDownFlags;
	foun::Flags<u32> m_currButtonDownFlags;
};

//namespace inpu
}
//namespace lct
}

#endif//INPU_MOUSE_READER_H
