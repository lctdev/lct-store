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
static const ssiz RESOURCE_PATH_SIZE = 256;
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

ssiz PackedAccessor::GetFileSize(const char* pPath)
{
	AAsset* pAsset = AAssetManager_open(m_pAssetManager, pPath, AASSET_MODE_UNKNOWN);

	ssiz fileSize = AAsset_getLength(pAsset);

	AAsset_close(pAsset);

	return fileSize;
}

void PackedAccessor::ReadFile(const char* pPath, void* pMemory, ssiz size)
{
	AAsset* pAsset = AAssetManager_open(m_pAssetManager, pPath, AASSET_MODE_BUFFER);

	AAsset_read(pAsset, pMemory, size);

	AAsset_close(pAsset);
}

void* PackedAccessor::LoadFile(const char* pPath, ssiz* pSize)
{
	AAsset* pAsset = AAssetManager_open(m_pAssetManager, pPath, AASSET_MODE_BUFFER);

	ssiz fileSize = AAsset_getLength(pAsset);

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

char* PackedAccessor::LoadFileString(const char* pPath, ssiz* pSize)
{
	AAsset* pAsset = AAssetManager_open(m_pAssetManager, pPath, AASSET_MODE_BUFFER);

	ssiz fileSize = AAsset_getLength(pAsset);

	char* pString = NULL;
	ssiz stringSize = 0;
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

ssiz PackedAccessor::GetFileSize(const char* pPath)
{
	char resourcePath[RESOURCE_PATH_SIZE];
	ConvertResourcePath(pPath, resourcePath, RESOURCE_PATH_SIZE);
	FILE* pFile = fopen(resourcePath, "rb");

	fseek(pFile, 0, SEEK_END);
	ssiz fileSize = ftell(pFile);

	fclose(pFile);

	return fileSize;
}

void PackedAccessor::ReadFile(const char* pPath, void* pMemory, ssiz size)
{
	char resourcePath[RESOURCE_PATH_SIZE];
	ConvertResourcePath(pPath, resourcePath, RESOURCE_PATH_SIZE);
	FILE* pFile = fopen(resourcePath, "rb");

	fread(pMemory, 1, size, pFile);

	fclose(pFile);
}

void* PackedAccessor::LoadFile(const char* pPath, ssiz* pSize)
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
	ssiz size = ftell(pFile);

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

char* PackedAccessor::LoadFileString(const char* pPath, ssiz* pSize)
{
	char resourcePath[RESOURCE_PATH_SIZE];
	ConvertResourcePath(pPath, resourcePath, RESOURCE_PATH_SIZE);
	FILE* pFile = fopen(resourcePath, "rb");

	fseek(pFile, 0, SEEK_END);
	ssiz fileSize = ftell(pFile);

	char* pString = NULL;
	ssiz stringSize = 0;
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
