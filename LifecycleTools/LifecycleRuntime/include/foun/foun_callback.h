#ifndef FOUN_CALLBACK_H
#define FOUN_CALLBACK_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace foun
{

class Callback
{
public:
	Callback() : m_pInstance(NULL), m_pFunction(NULL) {}

	template <class I>
	void Bind(I* pInstance, void (I::*pFunction)());
	bool IsBound() { return (m_pInstance != NULL) && (m_pFunction != NULL); }

	void Invoke();

private:
	class Placeholder
	{
	public:
		void Function() {}
	};

	typedef void (Placeholder::*Function)();

	Placeholder* m_pInstance;
	Function m_pFunction;
};

/*
* Public Template
*/
template <class I>
void Callback::Bind(I* pInstance, void (I::*pFunction)())
{
	m_pInstance = reinterpret_cast<Placeholder*>(pInstance);
	m_pFunction = reinterpret_cast<Function>(pFunction);
}

/*
* Public Instance
*/
inline void Callback::Invoke()
{
	(m_pInstance->*m_pFunction)();
}

//namespace lct
}
//namespace foun
}

#endif//FOUN_CALLBACK_H
