#ifndef FRAM_MESSAGE_QUEUE_H
#define FRAM_MESSAGE_QUEUE_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace fram
{

class Message;

class MessageQueue
{
public:
	MessageQueue();

	void SetMemory(void* pMemory, ssiz size);

	bool EnqueueMessage(const Message& message);
	bool GetFrontMessage(Message& message);
	bool DequeueMessage();

private:
	bool FindWriteRange(ssiz size, uptr& beginAddr, uptr& endAddr, bool& doesWrap);

	uptr m_beginAddr;
	uptr m_endAddr;
	uptr m_writeAddr;
	uptr m_readAddr;
	uptr m_wrapAddr;
};

//namespace fram
}
//namespace lct
}

#endif//FRAM_MESSAGE_QUEUE_H
