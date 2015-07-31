#ifndef FOUN_STACK_H
#define FOUN_STACK_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace foun
{

template <class V, int CAPACITY>
class Stack
{
public:
	Stack();

	void PushValue(const V& value);
	V& PopValue();
	u32 GetValueCount() { return m_valueCount; }

private:
	V m_values[CAPACITY];
	u32 m_valueCount;
};

/*
* Public Template
*/
template <class V, int CAPACITY>
Stack<V, CAPACITY>::Stack()
: m_values()
, m_valueCount(0)
{
}

template <class V, int CAPACITY>
void Stack<V, CAPACITY>::PushValue(const V& value)
{
	m_values[m_valueCount++] = value;
}

template <class V, int CAPACITY>
V& Stack<V, CAPACITY>::PopValue()
{
	V& value = m_values[--m_valueCount];
	return value;
}

//namespace lct
}
//namespace foun
}

#endif//FOUN_STACK_H
