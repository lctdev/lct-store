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

	virtual ssiz GetFileSize(const char* pPath);
	virtual void ReadFile(const char* pPath, void* pMemory, ssiz size);
	virtual void* LoadFile(const char* pPath, ssiz* pSize);
	virtual char* LoadFileString(const char* pPath, ssiz* pSize);

	virtual void GetCurrentDirectoryPath(char* pPath, ssiz maxSize);
};

//namespace file
}
//namespace lct
}

#endif//FILE_SYSTEM_ACCESSOR_H
