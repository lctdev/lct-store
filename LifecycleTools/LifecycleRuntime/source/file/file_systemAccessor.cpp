#include <file/file_systemAccessor.h>

#include <foun/foun_debug.h>

#include <stdio.h>
#if defined (LCT_WINDOWS)
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

ssiz SystemAccessor::GetFileSize(const char* pPath)
{
#if defined(LCT_WINDOWS)
	FILE* pFile;
	fopen_s(&pFile, pPath, "rb");
#else
	FILE* pFile = fopen(pPath, "rb");
#endif
	if (pFile == NULL)
	{
		LCT_TRACE("Could not open file path: %s\n", pPath);
		return 0;
	}

	fseek(pFile, 0, SEEK_END);
	s32 fileSize = static_cast<s32>(ftell(pFile));

	fclose(pFile);

	return fileSize;
}

void SystemAccessor::ReadFile(const char* pPath, void* pMemory, ssiz size)
{
#if defined(LCT_WINDOWS)
	FILE* pFile;
	fopen_s(&pFile, pPath, "rb");
#else
	FILE* pFile = fopen(pPath, "rb");
#endif
	if (pFile == NULL)
	{
		LCT_TRACE("Could not open file path: %s\n", pPath);
		return;
	}

	fread(pMemory, 1, size, pFile);

	fclose(pFile);
}

void* SystemAccessor::LoadFile(const char* pPath, ssiz* pSize)
{
#if defined(LCT_WINDOWS)
	FILE* pFile;
	fopen_s(&pFile, pPath, "rb");
#else
	FILE* pFile = fopen(pPath, "rb");
#endif
	if (pFile == NULL)
	{
		LCT_TRACE("Could not open file path: %s\n", pPath);
		*pSize = 0;
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

char* SystemAccessor::LoadFileString(const char* pPath, ssiz* pSize)
{
#if defined(LCT_WINDOWS)
	FILE* pFile;
	fopen_s(&pFile, pPath, "rb");
#else
	FILE* pFile = fopen(pPath, "rb");
#endif
	if (pFile == NULL)
	{
		LCT_TRACE("Could not open file path: %s\n", pPath);
		*pSize = 0;
		return NULL;
	}

	fseek(pFile, 0, SEEK_END);
	ssiz fileSize = ftell(pFile);

	char* pString = NULL;
	ssiz stringSize = 0;
	if (fileSize > 0)
	{
		stringSize = fileSize + 1;
		void* pMemory = m_pAllocator->Alloc(stringSize, foun::Allocator::TYPE_ALIGN);
		pString = reinterpret_cast<char*>(pMemory);
		fseek(pFile, 0, SEEK_SET);
		fread(pString, 1, fileSize, pFile);
		pString[fileSize] = 0;
	}

	fclose(pFile);

	*pSize = stringSize;
	return pString;
}

void SystemAccessor::GetCurrentDirectoryPath(char* pPath, ssiz maxSize)
{
#if defined (LCT_WINDOWS)
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
