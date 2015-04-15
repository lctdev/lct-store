#ifndef FRAM_MESSAGE_H
#define FRAM_MESSAGE_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace fram
{

class Message
{
	friend class MessageQueue;

public:
	Message();

	void SetType(u32 type);
	template <class T>
	void SetContent(const T* pContent);

	u32 GetType() const;
	template <class T>
	const T* GetContent() const;

private:
	u32 m_type;
	const void* m_pContent;
	u32 m_contentSize;
};

template <class T>
void Message::SetContent(const T* pContent)
{
	m_pContent = pContent;
	m_contentSize = sizeof(T);
}

template <class T>
const T* Message::GetContent() const
{
	if (m_contentSize == sizeof(T))
	{
		const T* pContent = static_cast<const T*>(m_pContent);
		return pContent;
	}
	else
	{
		return NULL;
	}
}

//namespace fram
}
//namespace lct
}

#endif//FRAM_MESSAGE_H
