#ifndef FILE_SYSTEM_ACCESSOR_H
#define FILE_SYSTEM_ACCESSOR_H

#include <file/file_accessor.h>

namespace lct
{
namespace file
{

class SystemAccessor: public Accessor
{
public:
	SystemAccessor();
	virtual ~SystemAccessor();

	virtual s32 GetFileSize(const char* pPath);
	virtual void ReadFile(const char* pPath, void* pMemory, u32 size);
	virtual void* LoadFile(const char* pPath, u32* pSize);
	virtual char* LoadFileString(const char* pPath, u32* pSize);

	virtual void GetCurrentDirectoryPath(char* pPath, u32 maxSize);
};

//namespace file
}
//namespace lct
}

#endif//FILE_SYSTEM_ACCESSOR_H
