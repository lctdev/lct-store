#ifndef UTIL_BINARY_READER_H
#define UTIL_BINARY_READER_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace util
{

class BinaryReader
{
public:
	BinaryReader();
	virtual ~BinaryReader();

	void SetMemory(void* pMemory, u32 size);

	bool CanRead(u32 size);
	void* Read(u32 size);

	template <class T>
	bool CanReadType();
	template <class T>
	T* ReadType();
	template <class T>
	T* ReadTypeArray(u32 count);

	s32 GetPosition();

private:
	uptr m_beginAddr;
	uptr m_endAddr;
	uptr m_currAddr;
};

/*
 * Public Template
 */
template <class T>
bool BinaryReader::CanReadType()
{
	u32 size = sizeof(T);
	return CanRead(size);
}

template <class T>
T* BinaryReader::ReadType()
{
	u32 size = sizeof(T);
	void* pMemory = Read(size);
	return static_cast<T*>(pMemory);
}

template <class T>
T* BinaryReader::ReadTypeArray(u32 count)
{
	u32 size = sizeof(T) * count;
	void* pMemory = Read(size);
	return static_cast<T*>(pMemory);
}

//namespace util
}
//namespace lct
}

#endif//UTIL_BINARY_READER_H
