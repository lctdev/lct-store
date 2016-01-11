#ifndef FILE_PACKED_ACCESSOR_H
#define FILE_PACKED_ACCESSOR_H

#include <file/file_accessor.h>

#if defined(LCT_ANDROID)
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

#if defined (LCT_ANDROID)
	void SetAssetManager(AAssetManager* pAssetManager);
#endif

	virtual ssiz GetFileSize(const char* pPath);
	virtual void ReadFile(const char* pPath, void* pMemory, ssiz size);
	virtual void* LoadFile(const char* pPath, ssiz* pSize);
	virtual char* LoadFileString(const char* pPath, ssiz* pSize);

private:
#if defined (LCT_ANDROID)
	AAssetManager* m_pAssetManager;
#endif
};

//namespace file
}
//namespace lct
}

#endif//FILE_PACKED_ACCESSOR_H
