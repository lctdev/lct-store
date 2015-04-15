#ifndef FOUN_CALLBACK_H
#define FOUN_CALLBACK_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace foun
{

template <class P, class R>
class Callback
{
public:
	Callback() {}
	virtual ~Callback() {}

	virtual R Invoke(P parameter) = 0;
};

template <class P, class R>
class StaticCallback : public Callback<P, R>
{
public:
	typedef R (*Function)(P);

public:
	StaticCallback() : Callback<P, R>(), m_pFunction(NULL) {}
	virtual ~StaticCallback() {}

	void Bind(Function pFunction);

	virtual R Invoke(P parameter);

private:
	Function m_pFunction;
};

template <class P, class R>
void StaticCallback<P, R>::Bind(Function pFunction)
{
	m_pFunction = pFunction;
}

template <class P, class R>
R StaticCallback<P, R>::Invoke(P parameter)
{
	return m_pFunction(parameter);
}

template <class I, class P, class R>
class InstanceCallback : public Callback<P, R>
{
public:
	typedef R (I::*Function)(P);

public:
	InstanceCallback() : Callback<P, R>(), m_pInstance(NULL), m_pFunction(NULL) {}
	virtual ~InstanceCallback() {}

	void Bind(I* pInstance, Function pFunction);

	virtual R Invoke(P parameter);

private:
	I* m_pInstance;
	Function m_pFunction;
};

template <class I, class P, class R>
void InstanceCallback<I, P, R>::Bind(I* pInstance, Function pFunction)
{
	m_pInstance = pInstance;
	m_pFunction = pFunction;
}

template <class I, class P, class R>
R InstanceCallback<I, P, R>::Invoke(P parameter)
{
	return (m_pInstance->*m_pFunction)(parameter);
}

//namespace lct
}
//namespace foun
}

#endif//FOUN_CALLBACK_H
