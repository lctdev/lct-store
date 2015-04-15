#ifndef FOUN_FLAGS_H
#define FOUN_FLAGS_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace foun
{

template <class N>
class Flags
{
public:
	Flags() : m_values(0) {}

	void Set(u32 index) { m_values |= (1 << index); }
	void Clear(u32 index) { m_values &= ~(1 << index); }
	bool Test(u32 index) { return (((1 << index) & m_values) != 0); } 
	void ClearAll() { m_values = 0; }

	N GetValues() { return m_values; }
	void SetValues(N values) { m_values = values; }

private:
	N m_values;
};

//namespace foun
}
//namespace lct
}

#endif//FOUN_FLAGS_H
 