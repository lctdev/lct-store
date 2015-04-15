#include <fram/fram_messageQueue.h>
#include <fram/fram_message.h>

#include <string.h>

namespace lct
{
namespace fram
{

/*
 * Internal Types
 */
struct MessageHeader
{
	u32 type;
	u32 contentAddr;
	u32 contentSize;
	u32 nextAddr;
};

/*
 * Internal Constants
 */

/*
 * Public Instance
 */
MessageQueue::MessageQueue()
: m_beginAddr(0)
, m_endAddr(0)
, m_writeAddr(0)
, m_readAddr(0)
{
}

void MessageQueue::SetMemory(void* pMemory, u32 size)
{
	m_beginAddr = (u32)pMemory;
	m_endAddr = m_beginAddr + size;
	m_writeAddr = m_beginAddr;
	m_readAddr = m_beginAddr;
}

bool MessageQueue::EnqueueMessage(const Message& message)
{
	u32 endAddr = m_writeAddr + sizeof(MessageHeader);

	u32 contentAddr = 0;
	if (message.m_pContent != NULL)
	{
		contentAddr = FindFreeAddress(endAddr, message.m_contentSize);
		if (contentAddr == 0)
		{
			return false;
		}
		endAddr = contentAddr + message.m_contentSize;
	}

	u32 nextAddr = FindFreeAddress(endAddr, sizeof(MessageHeader));
	if (nextAddr == 0)
	{
		return false;
	}

	MessageHeader header;
	header.type = message.m_type;
	header.contentAddr = contentAddr;
	header.contentSize = message.m_contentSize;
	header.nextAddr = nextAddr;

	void* pHeaderMemory = reinterpret_cast<void*>(m_writeAddr);
	memcpy(pHeaderMemory, &header, sizeof(MessageHeader));

	if (message.m_pContent != NULL)
	{
		void* pContentMemory = reinterpret_cast<void*>(contentAddr);
		memcpy(pContentMemory, message.m_pContent, message.m_contentSize);
	}

	// TODO: make threadsafe?
	m_writeAddr = nextAddr;

	return true;
}

bool MessageQueue::GetFrontMessage(Message& message)
{
	if (m_readAddr == m_writeAddr)
	{
		return false;
	}

	MessageHeader header;

	void* pHeaderMemory = reinterpret_cast<void*>(m_readAddr);
	memcpy(&header, pHeaderMemory, sizeof(MessageHeader));

	message.m_type = header.type;
	message.m_pContent = reinterpret_cast<void*>(header.contentAddr);
	message.m_contentSize = header.contentSize;

	return true;
}

bool MessageQueue::DequeueMessage()
{
	if (m_readAddr == m_writeAddr)
	{
		return false;
	}

	MessageHeader header;

	void* pHeaderMemory = reinterpret_cast<void*>(m_readAddr);
	memcpy(&header, pHeaderMemory, sizeof(MessageHeader));

	// TODO: make threadsafe?
	m_readAddr = header.nextAddr;

	return true;
}

u32 MessageQueue::FindFreeAddress(u32 startAddr, u32 size)
{
	u32 currAddr = startAddr;
	u32 nextAddr = startAddr + size;

	if (nextAddr > m_endAddr)
	{
		currAddr = m_beginAddr;
		nextAddr = m_beginAddr + size;
		if (currAddr == m_readAddr)
		{
			return 0;
		}
	}

	if ((currAddr < m_readAddr) && (nextAddr >= m_readAddr))
	{
		return 0;
	}

	return currAddr;
}

//namespace fram
}
//namespace lct
}
