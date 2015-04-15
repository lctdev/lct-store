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

	void SetMemory(void* pMemory, u32 size);

	bool EnqueueMessage(const Message& message);
	bool GetFrontMessage(Message& message);
	bool DequeueMessage();

private:
	u32 FindFreeAddress(u32 startAddr, u32 size);

	u32 m_beginAddr;
	u32 m_endAddr;
	u32 m_writeAddr;
	u32 m_readAddr;
};

//namespace fram
}
//namespace lct
}

#endif//FRAM_MESSAGE_QUEUE_H
