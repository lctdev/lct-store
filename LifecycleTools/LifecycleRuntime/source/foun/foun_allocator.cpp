#include <foun/foun_allocator.h>

namespace lct
{
namespace foun
{

/*
 * Public Instance
 */
Allocator::Allocator()
{
}

Allocator::~Allocator()
{
}

void* Allocator::Alloc(ssiz size, u32 align)
{
	return NULL;
}

void Allocator::Free(void* pMemory)
{
}

//namespace lct
}
//namespace foun
}
