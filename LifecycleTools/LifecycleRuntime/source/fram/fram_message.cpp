#include <fram/fram_message.h>

#include <string.h>

namespace lct
{
namespace fram
{

/*
 * Internal Constants
 */

/*
 * Public Instance
 */
Message::Message()
: m_type(0)
, m_pContent(NULL)
, m_contentSize(0)
{
}

void Message::SetType(u32 type)
{
	m_type = type;
}

u32 Message::GetType() const
{
	return m_type;
}

//namespace fram
}
//namespace lct
}
