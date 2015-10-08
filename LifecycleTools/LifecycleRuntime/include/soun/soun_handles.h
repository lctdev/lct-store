#ifndef SOUN_HANDLES_H
#define SOUN_HANDLES_H

#include <foun/foun_primitives.h>

#include <soun/soun_constants.h>

namespace lct
{
namespace soun
{

class ClipHandle
{
public:
	ClipHandle() : m_slotIndex(-1) {}

private:
	s32 m_slotIndex;

	friend class ClipCoordinator;
};

//namespace soun
}
//namespace lct
}

#endif//SOUN_HANDLES_H
