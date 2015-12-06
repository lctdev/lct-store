#include <file/file_packedAccessor.h>

#if defined(LCT_ANDROID)
#include <android/asset_manager.h>
#elif defined(LCT_IOS)
#include <file/file_packedAccessor_OBJC.h>
#include <stdio.h>
#endif

#include <foun/foun_debug.h>

namespace lct
{
namespace file
{

#if defined(LCT_ANDROID) || defined(LCT_IOS)
/*
* Internal Constants
*/
static const u32 RESOURCE_PATH_SIZE = 256;
#endif

#if defined(LCT_ANDROID)
/*
 * Public Instance
 */
PackedAccessor::PackedAccessor()
: Accessor()
, m_pAssetManager(NULL)
{
}

PackedAccessor::~PackedAccessor()
{
}

void PackedAccessor::SetAssetManager(AAssetManager* pAssetManager)
{
	m_pAssetManager = pAssetManager;
}

s32 PackedAccessor::GetFileSize(const char* pPath)
{
	AAsset* pAsset = AAssetManager_open(m_pAssetManager, pPath, AASSET_MODE_UNKNOWN);

	s32 fileSize = AAsset_getLength(pAsset);

	AAsset_close(pAsset);

	return fileSize;
}

void PackedAccessor::ReadFile(const char* pPath, void* pMemory, u32 size)
{
	AAsset* pAsset = AAssetManager_open(m_pAssetManager, pPath, AASSET_MODE_BUFFER);

	AAsset_read(pAsset, pMemory, size);

	AAsset_close(pAsset);
}

void* PackedAccessor::LoadFile(const char* pPath, u32* pSize)
{
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
}

char* PackedAccessor::LoadFileString(const char* pPath, u32* pSize)
{
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
}
#elif defined (LCT_IOS)
PackedAccessor::PackedAccessor()
: Accessor()
{
}

PackedAccessor::~PackedAccessor()
{
}

s32 PackedAccessor::GetFileSize(const char* pPath)
{
	char resourcePath[RESOURCE_PATH_SIZE];
	ConvertResourcePath(pPath, resourcePath, RESOURCE_PATH_SIZE);
	FILE* pFile = fopen(resourcePath, "rb");

	fseek(pFile, 0, SEEK_END);
	s32 fileSize = static_cast<s32>(ftell(pFile));

	fclose(pFile);

	return fileSize;
}

void PackedAccessor::ReadFile(const char* pPath, void* pMemory, u32 size)
{
	char resourcePath[RESOURCE_PATH_SIZE];
	ConvertResourcePath(pPath, resourcePath, RESOURCE_PATH_SIZE);
	FILE* pFile = fopen(resourcePath, "rb");

	fread(pMemory, 1, size, pFile);

	fclose(pFile);
}

void* PackedAccessor::LoadFile(const char* pPath, u32* pSize)
{
	char resourcePath[RESOURCE_PATH_SIZE];
	ConvertResourcePath(pPath, resourcePath, RESOURCE_PATH_SIZE);
	FILE* pFile = fopen(resourcePath, "rb");

	if (pFile == NULL)
	{
		LCT_TRACE("Could not open file path: %s\n", pPath);
		return NULL;
	}

	fseek(pFile, 0, SEEK_END);
	s32 size = static_cast<s32>(ftell(pFile));

	void* pMemory = NULL;
	if (size > 0)
	{
		pMemory = m_pAllocator->Alloc(size, 4);
		fseek(pFile, 0, SEEK_SET);
		fread(pMemory, 1, size, pFile);
	}

	fclose(pFile);

	*pSize = size;
	return pMemory;
}

char* PackedAccessor::LoadFileString(const char* pPath, u32* pSize)
{
	char resourcePath[RESOURCE_PATH_SIZE];
	ConvertResourcePath(pPath, resourcePath, RESOURCE_PATH_SIZE);
	FILE* pFile = fopen(resourcePath, "rb");

	fseek(pFile, 0, SEEK_END);
	s32 fileSize = static_cast<s32>(ftell(pFile));

	char* pString = NULL;
	u32 stringSize = 0;
	if (fileSize > 0)
	{
		stringSize = fileSize + 1;
		pString = m_pAllocator->AllocTypeArray<char>(stringSize);
		fseek(pFile, 0, SEEK_SET);
		fread(pString, 1, fileSize, pFile);
		pString[fileSize] = 0;
	}

	fclose(pFile);

	*pSize = stringSize;
	return pString;
}
#endif

//namespace file
}
//namespace lct
}
