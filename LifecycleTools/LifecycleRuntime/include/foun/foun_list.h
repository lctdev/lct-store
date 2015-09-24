#ifndef FOUN_LIST_H
#define FOUN_LIST_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace foun
{

template <class V>
class ListIterator;
template <class V>
class List;

template <class V>
class ListNode
{
	friend class ListIterator<V>;
	friend class List<V>;

public:
	ListNode();
	ListNode(V value);

	void SetValue(const V& value) { m_value = value; }
	V& GetValue() { return m_value; }

private:
	ListNode<V>* m_pPrev;
	ListNode<V>* m_pNext;
	V m_value;
};

template <class V>
class ListIterator
{
	friend class List<V>;

public:
	void Next() { m_pCurrNode = m_pCurrNode->m_pNext; }
	bool IsEnd() { return m_pCurrNode == NULL; }
	V GetValue() { return m_pCurrNode->m_value; }

private:
	ListIterator(ListNode<V>* pNode);

	ListNode<V>* m_pCurrNode;
};

template <class V>
class List
{
public:
	List();

	void AddNode(ListNode<V>* pNode);
	void RemoveNode(ListNode<V>* pNode);
	ListIterator<V> Head() { return ListIterator<V>(m_pHeadNode); }
	ListIterator<V> Tail() { return ListIterator<V>(m_pTailNode); }
	u32 GetNodeCount() { return m_nodeCount; }

private:
	ListNode<V>* m_pHeadNode;
	ListNode<V>* m_pTailNode;
	u32 m_nodeCount;
};

/*
 * Public Template
 */
template <class V>
ListNode<V>::ListNode()
: m_pPrev(NULL)
, m_pNext(NULL)
, m_value()
{
}

template <class V>
ListNode<V>::ListNode(V value)
: m_pPrev(NULL)
, m_pNext(NULL)
, m_value(value)
{
}

/*
 * Private Template
 */
template <class V>
ListIterator<V>::ListIterator(ListNode<V>* pNode)
: m_pCurrNode(pNode)
{
}

/*
 * Public Template
 */
template <class V>
List<V>::List()
: m_pHeadNode(NULL)
, m_pTailNode(NULL)
, m_nodeCount(0)
{
}

template <class V>
void List<V>::AddNode(ListNode<V>* pNode)
{
	if (m_nodeCount == 0)
	{
		pNode->m_pNext = NULL;
		pNode->m_pPrev = NULL;

		m_pHeadNode = pNode;
		m_pTailNode = pNode;
		++m_nodeCount;
	}
	else
	{
		m_pTailNode->m_pNext = pNode;

		pNode->m_pNext = NULL;
		pNode->m_pPrev = m_pTailNode;

		m_pTailNode = pNode;
		++m_nodeCount;
	}
}

template <class V>
void List<V>::RemoveNode(ListNode<V>* pNode)
{
	if (m_nodeCount == 1)
	{
		m_pHeadNode = NULL;
		m_pTailNode = NULL;
		--m_nodeCount;
	}
	else
	{
		if (pNode == m_pHeadNode)
		{
			m_pHeadNode = pNode->m_pNext;
			--m_nodeCount;

			m_pHeadNode->m_pPrev = NULL;
		}
		else if (pNode == m_pTailNode)
		{
			m_pTailNode = pNode->m_pPrev;
			--m_nodeCount;

			m_pTailNode->m_pNext = NULL;
		}
		else
		{
			--m_nodeCount;

			pNode->m_pPrev->m_pNext = pNode->m_pNext;
			pNode->m_pNext->m_pPrev = pNode->m_pPrev;
		}
	}

	pNode->m_pPrev = NULL;
	pNode->m_pNext = NULL;
}

//namespace lct
}
//namespace foun
}

#endif//FOUN_LIST_H
