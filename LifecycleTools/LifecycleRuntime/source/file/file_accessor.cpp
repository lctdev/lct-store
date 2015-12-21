#include <file/file_accessor.h>

namespace lct
{
namespace file
{

/*
 * Public Instance
 */
Accessor::Accessor()
: m_pAllocator(NULL)
{
}

Accessor::~Accessor()
{
}

void Accessor::SetAllocator(lct::foun::Allocator* pAllocator)
{
	m_pAllocator = pAllocator;
}

s32 Accessor::GetFileSize(const char* pPath)
{
	return 0;
}

void Accessor::ReadFile(const char* pPath, void* pMemory, ssiz size)
{
}

void* Accessor::LoadFile(const char* pPath, ssiz* pSize)
{
	*pSize = 0;
	return NULL;
}

char* Accessor::LoadFileString(const char* pPath, ssiz* pSize)
{
	*pSize = 0;
	return NULL;
}

void Accessor::GetCurrentDirectoryPath(char* pPath, ssiz maxSize)
{
	pPath[0] = '\0';
}

//namespace file
}
//namespace lct
}
