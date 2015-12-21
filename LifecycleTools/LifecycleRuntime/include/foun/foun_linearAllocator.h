#ifndef FOUN_LINEAR_ALLOCATOR_H
#define FOUN_LINEAR_ALLOCATOR_H

#include <foun/foun_allocator.h>

namespace lct
{
namespace foun
{

class LinearAllocator : public Allocator
{
public:
	LinearAllocator();
	virtual ~LinearAllocator();

	void SetMemory(void* pMemory, u32 size);
	void Clear();

	virtual void* Alloc(ssiz size, u32 align);

private:
	uptr m_beginAddr;
	uptr m_endAddr;
	uptr m_currAddr;
};

//namespace foun
}
//namespace lct
}

#endif//FOUN_LINEAR_ALLOCATOR_H
