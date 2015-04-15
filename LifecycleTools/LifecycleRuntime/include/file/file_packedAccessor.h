#ifndef FILE_PACKED_ACCESSOR_H
#define FILE_PACKED_ACCESSOR_H

#include <file/file_accessor.h>

#if defined(__ANDROID__)
struct AAssetManager;
#endif

namespace lct
{
namespace file
{

class PackedAccessor: public Accessor
{
public:
	PackedAccessor();
	virtual ~PackedAccessor();

#if defined (__ANDROID__)
	void SetAssetManager(AAssetManager* pAssetManager);
#endif

	virtual s32 GetFileSize(const char* pPath);
	virtual void ReadFile(const char* pPath, void* pMemory, u32 size);
	virtual void* LoadFile(const char* pPath, u32* pSize);
	virtual char* LoadFileString(const char* pPath, u32* pSize);

private:
#if defined (__ANDROID__)
	AAssetManager* m_pAssetManager;
#endif
};

//namespace file
}
//namespace lct
}

#endif//FILE_PACKED_ACCESSOR_H
