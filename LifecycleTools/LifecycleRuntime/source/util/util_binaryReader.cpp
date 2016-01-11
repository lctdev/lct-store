#include <util/util_binaryReader.h>

namespace lct
{
namespace util
{

/*
 * Public Instance
 */
BinaryReader::BinaryReader()
: m_beginAddr(0)
, m_endAddr(0)
, m_currAddr(0)
{
}

BinaryReader::~BinaryReader()
{
}

void BinaryReader::SetMemory(void* pMemory, ssiz size)
{
	m_beginAddr = reinterpret_cast<uptr>(pMemory);
	m_endAddr = m_beginAddr + size;
	m_currAddr = m_beginAddr;
}

bool BinaryReader::CanRead(ssiz size)
{
	uptr nextAddr = m_currAddr + size;
	if (nextAddr <= m_endAddr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void* BinaryReader::Read(ssiz size)
{
	void* pMemory = reinterpret_cast<void*>(m_currAddr);
	uptr nextAddr = m_currAddr + size;
	if (nextAddr <= m_endAddr)
	{
		m_currAddr = nextAddr;
		return pMemory;
	}
	else
	{
		return NULL;
	}
}

ssiz BinaryReader::GetPosition()
{
	ssiz position = static_cast<ssiz>(m_currAddr) - static_cast<ssiz>(m_beginAddr);
	return position;
}

//namespace util
}
//namespace lct
}
