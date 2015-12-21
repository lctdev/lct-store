#include <foun/foun_linearAllocator.h>

namespace lct
{
namespace foun
{

/*
 * Public Instance
 */
LinearAllocator::LinearAllocator()
: Allocator()
, m_beginAddr(0)
, m_endAddr(0)
, m_currAddr(0)
{
}

LinearAllocator::~LinearAllocator()
{
}

void LinearAllocator::SetMemory(void* pMemory, u32 size)
{
	m_beginAddr = (uptr)pMemory;
	m_endAddr = m_beginAddr + size;
	m_currAddr = m_beginAddr;
}

void LinearAllocator::Clear()
{
	m_currAddr = m_beginAddr;
}

void* LinearAllocator::Alloc(ssiz size, u32 align)
{
	if (size > 0)
	{
		ssiz round = size - 1;
		uptr nextAddr = (m_currAddr + round) & ~(round);
		void* pMemory = (void*)nextAddr;
		nextAddr += size;
		if (nextAddr <= m_endAddr)
		{
			m_currAddr = nextAddr;
			return pMemory;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}

//namespace lct
}
//namespace foun
}
