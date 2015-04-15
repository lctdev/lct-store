#include <file/file_systemAccessor.h>

#include <foun/foun_debug.h>

#include <stdio.h>
#if defined (WIN32)
#include <Windows.h>
#else
#include <unistd.h>
#endif

namespace lct
{
namespace file
{

/*
 * Public Instance
 */
SystemAccessor::SystemAccessor()
: Accessor()
{
}

SystemAccessor::~SystemAccessor()
{
}

s32 SystemAccessor::GetFileSize(const char* pPath)
{
#if defined(WIN32)
	FILE* pFile;
	fopen_s(&pFile, pPath, "rb");
#else
	FILE* pFile = fopen(pPath, "rb");
#endif

	fseek(pFile, 0, SEEK_END);
	s32 fileSize = ftell(pFile);

	fclose(pFile);

	return fileSize;
}

void SystemAccessor::ReadFile(const char* pPath, void* pMemory, u32 size)
{
#if defined(WIN32)
	FILE* pFile;
	fopen_s(&pFile, pPath, "rb");
#else
	FILE* pFile = fopen(pPath, "rb");
#endif

	fread(pMemory, 1, size, pFile);

	fclose(pFile);
}

void* SystemAccessor::LoadFile(const char* pPath, u32* pSize)
{
#if defined(WIN32)
	FILE* pFile;
	fopen_s(&pFile, pPath, "rb");
#else
	FILE* pFile = fopen(pPath, "rb");
#endif
	if (pFile == NULL)
	{
		LCT_TRACE("Could not open file path: %s\n", pPath);
		return NULL;
	}

	fseek(pFile, 0, SEEK_END);
	s32 size = ftell(pFile);

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

char* SystemAccessor::LoadFileString(const char* pPath, u32* pSize)
{
#if defined(WIN32)
	FILE* pFile;
	fopen_s(&pFile, pPath, "rb");
#else
	FILE* pFile = fopen(pPath, "rb");
#endif

	fseek(pFile, 0, SEEK_END);
	s32 fileSize = ftell(pFile);

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

void SystemAccessor::GetCurrentDirectoryPath(char* pPath, u32 maxSize)
{
#if defined (WIN32)
	static const u32 PATH_BUFFER_SIZE = 256;
	wchar_t pathBuffer[256];
	GetCurrentDirectory(PATH_BUFFER_SIZE, pathBuffer);
	wcstombs_s(NULL, pPath, maxSize, pathBuffer, maxSize);
#else
	getcwd(pPath, maxSize);
#endif
}

//namespace file
}
//namespace lct
}
