#include <file/file_packedAccessor.h>

#if defined(__ANDROID__)
#include <android/asset_manager.h>
#endif

namespace lct
{
namespace file
{

/*
 * Public Instance
 */
PackedAccessor::PackedAccessor()
: Accessor()
#if defined(__ANDROID__)
, m_pAssetManager(NULL)
#endif
{
}

PackedAccessor::~PackedAccessor()
{
}

#if defined(__ANDROID__)
void PackedAccessor::SetAssetManager(AAssetManager* pAssetManager)
{
	m_pAssetManager = pAssetManager;
}
#endif

s32 PackedAccessor::GetFileSize(const char* pPath)
{
#if defined(__ANDROID__)
	AAsset* pAsset = AAssetManager_open(m_pAssetManager, pPath, AASSET_MODE_UNKNOWN);

	s32 fileSize = AAsset_getLength(pAsset);

	AAsset_close(pAsset);

	return fileSize;
#else
	return 0;
#endif
}

void PackedAccessor::ReadFile(const char* pPath, void* pMemory, u32 size)
{
#if defined(__ANDROID__)
	AAsset* pAsset = AAssetManager_open(m_pAssetManager, pPath, AASSET_MODE_BUFFER);

	AAsset_read(pAsset, pMemory, size);

	AAsset_close(pAsset);
#endif
}

void* PackedAccessor::LoadFile(const char* pPath, u32* pSize)
{
#if defined(__ANDROID__)
	AAsset* pAsset = AAssetManager_open(m_pAssetManager, pPath, AASSET_MODE_BUFFER);

	s32 fileSize = AAsset_getLength(pAsset);

	void* pMemory = NULL;
	if (fileSize > 0)
	{
		pMemory = m_pAllocator->Alloc(fileSize, 4);
		AAsset_read(pAsset, pMemory, fileSize);
	}

	AAsset_close(pAsset);

	*pSize = fileSize;
	return pMemory;
#else
	return NULL;
#endif
}

char* PackedAccessor::LoadFileString(const char* pPath, u32* pSize)
{
#if defined(__ANDROID__)
	AAsset* pAsset = AAssetManager_open(m_pAssetManager, pPath, AASSET_MODE_BUFFER);

	s32 fileSize = AAsset_getLength(pAsset);

	char* pString = NULL;
	u32 stringSize = 0;
	if (fileSize > 0)
	{
		stringSize = fileSize + 1;
		pString = m_pAllocator->AllocTypeArray<char>(stringSize);
		AAsset_read(pAsset, pString, fileSize);
		pString[fileSize] = 0;
	}

	AAsset_close(pAsset);

	*pSize = stringSize;
	return pString;
#else
	return NULL;
#endif
}

//namespace file
}
//namespace lct
}
