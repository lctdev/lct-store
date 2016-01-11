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

	void SetMemory(void* pMemory, ssiz size);

	bool CanRead(ssiz size);
	void* Read(ssiz size);

	template <class T>
	bool CanReadType();
	template <class T>
	T* ReadType();
	template <class T>
	T* ReadTypeArray(u32 count);

	ssiz GetPosition();

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
