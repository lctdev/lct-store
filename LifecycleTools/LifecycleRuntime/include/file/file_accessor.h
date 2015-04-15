#ifndef FILE_ACCESSOR_H
#define FILE_ACCESSOR_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>

namespace lct
{
namespace file
{

class Accessor
{
public:
	Accessor();
	virtual ~Accessor();

	void SetAllocator(lct::foun::Allocator* pAllocator);

	virtual s32 GetFileSize(const char* pPath);
	virtual void ReadFile(const char* pPath, void* pMemory, u32 size);
	virtual void* LoadFile(const char* pPath, u32* pSize);
	virtual char* LoadFileString(const char* pPath, u32* pSize);

	virtual void GetCurrentDirectoryPath(char* pPath, u32 maxSize);

protected:
	lct::foun::Allocator* m_pAllocator;
};

//namespace file
}
//namespace lct
}

#endif//FILE_ACCESSOR_H
