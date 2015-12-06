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
	u32 contentSize;
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
, m_wrapAddr(0)
{
}

void MessageQueue::SetMemory(void* pMemory, ssiz size)
{
	m_beginAddr = (uptr)pMemory;
	m_endAddr = m_beginAddr + size;
	m_writeAddr = m_beginAddr;
	m_readAddr = m_beginAddr;
}

bool MessageQueue::EnqueueMessage(const Message& message)
{
	ssiz size = sizeof(MessageHeader) + message.m_contentSize;
	uptr beginAddr;
	uptr endAddr;
	bool isWrap;
	if (!FindWriteRange(size, beginAddr, endAddr, isWrap))
	{
		return false;
	}
	
	MessageHeader header;
	header.type = message.m_type;
	header.contentSize = message.m_contentSize;

	void* pHeaderMemory = reinterpret_cast<void*>(beginAddr);
	memcpy(pHeaderMemory, &header, sizeof(MessageHeader));

	if (message.m_pContent != NULL)
	{
		void* pContentMemory = reinterpret_cast<void*>(beginAddr + sizeof(MessageHeader));
		memcpy(pContentMemory, message.m_pContent, message.m_contentSize);
	}

	// TODO: make atomic?
	if (isWrap)
	{
		m_wrapAddr = endAddr;
		m_writeAddr = m_beginAddr;
	}
	else
	{
		m_writeAddr = endAddr;
	}
	return true;
}

bool MessageQueue::GetFrontMessage(Message& message)
{
	// TODO: make atomic?
	if (m_readAddr == m_writeAddr)
	{
		return false;
	}

	MessageHeader header;

	void* pHeaderMemory = reinterpret_cast<void*>(m_readAddr);
	memcpy(&header, pHeaderMemory, sizeof(MessageHeader));

	message.m_type = header.type;
	if (header.contentSize != 0)
	{
		void* pContentMemory = reinterpret_cast<void*>(m_readAddr + sizeof(MessageHeader));
		message.m_pContent = pContentMemory;
		message.m_contentSize = header.contentSize;
	}
	else
	{
		message.m_pContent = NULL;
		message.m_contentSize = 0;
	}

	return true;
}

bool MessageQueue::DequeueMessage()
{
	// TODO: make atomic?
	if (m_readAddr == m_writeAddr)
	{
		return false;
	}

	MessageHeader header;

	void* pHeaderMemory = reinterpret_cast<void*>(m_readAddr);
	memcpy(&header, pHeaderMemory, sizeof(MessageHeader));
	
	ssiz size = sizeof(MessageHeader) + header.contentSize;
	uptr readAddr = m_readAddr + size;
	if (readAddr == m_wrapAddr)
	{
		readAddr = m_beginAddr;
	}

	// TODO: make atomic?
	m_readAddr = readAddr;

	return true;
}

/*
 * Private Instance
 */
bool MessageQueue::FindWriteRange(ssiz size, uptr& beginAddr, uptr& endAddr, bool& doesWrap)
{
	// TODO: make atomic?
	uptr readAddr = m_readAddr;
	
	// set up the initial range
	beginAddr = m_writeAddr;
	endAddr = m_writeAddr + size;
	doesWrap = false;
	
	if ((beginAddr < readAddr) && (endAddr >= readAddr))
	{
		// range would overlap
		return false;
	}
	
	if (endAddr < m_endAddr)
	{
		// not at the end, nothing else to check
		return true;
	}
	else if (endAddr == m_endAddr)
	{
		// we are at the end, so we need to wrap
		doesWrap = true;
		
		if (readAddr == m_beginAddr)
		{
			// wrap would overlap
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		// we are past the end, so we need to reset the range and wrap
		beginAddr = m_beginAddr;
		endAddr = m_beginAddr + size;
		doesWrap = true;
		
		if ((beginAddr < readAddr) && (endAddr >= readAddr))
		{
			// wrapped range overlaps
			return false;
		}
		else
		{
			return true;
		}
	}
}

//namespace fram
}
//namespace lct
}
