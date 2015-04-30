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

	virtual void* Alloc(u32 size, u32 align);

private:
	u32 m_beginAddr;
	u32 m_endAddr;
	u32 m_currAddr;
};

//namespace foun
}
//namespace lct
}

#endif//FOUN_LINEAR_ALLOCATOR_H