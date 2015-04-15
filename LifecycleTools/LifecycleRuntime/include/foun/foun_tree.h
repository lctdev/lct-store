#ifndef FOUN_TREE_H
#define FOUN_TREE_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace foun
{

template <class K, class V>
class TreeIterator;
template <class K, class V>
class Tree;

template <class K, class V>
class TreeNode
{
	friend class TreeIterator<K, V>;
	friend class Tree<K, V>;

public:
	TreeNode();

	void SetKey(const K& key) { m_key = key; }
	K& GetKey() { return m_key; }
	void SetValue(const V& value) { m_value = value; }
	V& GetValue() { return m_value; }

private:
	TreeNode<K, V>* m_pParent;
	TreeNode<K, V>* m_pLeft;
	TreeNode<K, V>* m_pRight;
	s32 m_height;
	Tree<K, V>* m_pTree;
	K m_key;
	V m_value;
};

template <class K, class V>
class TreeIterator
{
	friend class Tree<K, V>;

public:
	void Next();
	bool IsEnd() { return m_pCurrNode == NULL; }
	K& GetKey() { return m_pCurrNode->m_key; }
	V& GetValue() { return m_pCurrNode->m_value; }

private:
	TreeIterator(TreeNode<K, V>* pNode);

	TreeNode<K, V>* m_pCurrNode;
};

template <class K, class V>
class Tree
{
public:
	Tree();

	void AddNode(TreeNode<K, V>* pNode);
	void RemoveNode(TreeNode<K, V>* pNode);
	TreeNode<K, V>* FindNode(const K& key);
	TreeIterator<K, V> Begin();
	u32 GetNodeCount() { return m_nodeCount; }

private:
	void UpdateHeight(TreeNode<K, V>* pNode);
	void Rebalance(TreeNode<K, V>* pNode);
	void RotateLeft(TreeNode<K, V>* pNode);
	void RotateRight(TreeNode<K, V>* pNode);

	TreeNode<K, V>* m_pRootNode;
	u32 m_nodeCount;
};

/*
 * Public Template
 */
template <class K, class V>
TreeNode<K, V>::TreeNode()
: m_pParent(NULL)
, m_pLeft(NULL)
, m_pRight(NULL)
, m_height(0)
, m_pTree(NULL)
{
}

/*
 * Public Template
 */
template <class K, class V>
void TreeIterator<K, V>::Next()
{
	TreeNode<K, V>* pParentNode = m_pCurrNode->m_pParent;
	TreeNode<K, V>* pRightNode = m_pCurrNode->m_pRight;

	TreeNode<K, V>* pNextNode = NULL;
	if (pRightNode != NULL)
	{
		pNextNode = pRightNode;
		while (pNextNode->m_pLeft != NULL)
		{
			pNextNode = pNextNode->m_pLeft;
		}
	}
	else
	{
		TreeNode<K, V>* pPrevNode = m_pCurrNode;
		while ((pNextNode == NULL) && (pParentNode != NULL))
		{
			if (pPrevNode == pParentNode->m_pLeft)
			{
				pNextNode = pParentNode;
			}
			else
			{
				pPrevNode = pParentNode;
				pParentNode = pParentNode->m_pParent;
			}
		}
	}
	m_pCurrNode = pNextNode;
}

/*
 * Private Template
 */
template <class K, class V>
TreeIterator<K, V>::TreeIterator(TreeNode<K, V>* pNode)
: m_pCurrNode(pNode)
{
}

/*
 * Public Template
 */
template <class K, class V>
Tree<K, V>::Tree()
: m_pRootNode(NULL)
, m_nodeCount(0)
{
}

template <class K, class V>
void Tree<K, V>::AddNode(TreeNode<K, V>* pNode)
{
	if (pNode->m_pTree != NULL)
	{
		return;
	}

	if (m_pRootNode == NULL)
	{
		pNode->m_pParent = NULL;
		pNode->m_pLeft = NULL;
		pNode->m_pRight = NULL;
		pNode->m_height = 0;
		pNode->m_pTree = this;

		m_pRootNode = pNode;
	}
	else
	{
		TreeNode<K, V>* pParentNode = m_pRootNode;
		pNode->m_pParent = NULL;
		while (pNode->m_pParent == NULL)
		{
			s32 result = K::Compare(pNode->m_key, pParentNode->m_key);
			if (result < 0)
			{
				if (pParentNode->m_pLeft == NULL)
				{
					pParentNode->m_pLeft = pNode;
					pNode->m_pParent = pParentNode;
				}
				else
				{
					pParentNode = pParentNode->m_pLeft;
				}
			}
			else
			{
				if (pParentNode->m_pRight == NULL)
				{
					pParentNode->m_pRight = pNode;
					pNode->m_pParent = pParentNode;
				}
				else
				{
					pParentNode = pParentNode->m_pRight;
				}
			}
		}
		pNode->m_pLeft = NULL;
		pNode->m_pRight = NULL;

		UpdateHeight(pNode);

		pNode->m_pTree = this;

		Rebalance(pNode);
	}

	++m_nodeCount;
}

template <class K, class V>
void Tree<K, V>::RemoveNode(TreeNode<K, V>* pNode)
{
	if (pNode->m_pTree != this)
	{
		return;
	}

	TreeNode<K, V>* pParentNode = pNode->m_pParent;
	TreeNode<K, V>* pLeftNode = pNode->m_pLeft;
	TreeNode<K, V>* pRightNode = pNode->m_pRight;

	TreeNode<K, V>* pReplaceNode = NULL;
	if ((pLeftNode != NULL) && (pRightNode == NULL))
	{
		pReplaceNode = pLeftNode;
	}
	else if ((pLeftNode == NULL) && (pRightNode != NULL))
	{
		pReplaceNode = pRightNode;
	}
	else if ((pLeftNode != NULL) && (pRightNode != NULL))
	{
		if (pLeftNode->m_height > pRightNode->m_height)
		{
			pReplaceNode = pLeftNode;
			while (pReplaceNode->m_pRight != NULL)
			{
				pReplaceNode = pReplaceNode->m_pRight;
			}
		}
		else
		{
			pReplaceNode = pRightNode;
			while (pReplaceNode->m_pLeft != NULL)
			{
				pReplaceNode = pReplaceNode->m_pLeft;
			}
		}
	}

	TreeNode<K, V>* pEvalNode = NULL;
	if (pReplaceNode != NULL)
	{
		TreeNode<K, V>* pReplaceParentNode = pReplaceNode->m_pParent;
		TreeNode<K, V>* pReplaceLeftNode = pReplaceNode->m_pLeft;
		TreeNode<K, V>* pReplaceRightNode = pReplaceNode->m_pRight;

		if (pReplaceParentNode == pNode)
		{
			pEvalNode = pReplaceNode;
		}
		else
		{
			pEvalNode = pReplaceParentNode;

			if (pReplaceParentNode != NULL)
			{
				if (pReplaceParentNode->m_pLeft == pReplaceNode)
				{
					pReplaceParentNode->m_pLeft = pReplaceRightNode;
					if (pReplaceRightNode != NULL)
					{
						pReplaceRightNode->m_pParent = pReplaceParentNode;
					}
				}
				else
				{
					pReplaceParentNode->m_pRight = pReplaceLeftNode;
					if (pReplaceLeftNode != NULL)
					{
						pReplaceLeftNode->m_pParent = pReplaceParentNode;
					}
				}
			}
		}

		pReplaceNode->m_pParent = pParentNode;
		if (pReplaceNode != pLeftNode)
		{
			pReplaceNode->m_pLeft = pLeftNode;
		}
		if (pReplaceNode != pRightNode)
		{
			pReplaceNode->m_pRight = pRightNode;
		}
	}
	else
	{
		pEvalNode = pParentNode;
	}

	if (pParentNode != NULL)
	{
		if (pParentNode->m_pLeft == pNode)
		{
			pParentNode->m_pLeft = pReplaceNode;
		}
		else
		{
			pParentNode->m_pRight = pReplaceNode;
		}
	}
	if ((pLeftNode != NULL) && (pLeftNode != pReplaceNode))
	{
		pLeftNode->m_pParent = pReplaceNode;
	}
	if ((pRightNode != NULL) && (pRightNode != pReplaceNode))
	{
		pRightNode->m_pParent = pReplaceNode;
	}

	if (m_pRootNode == pNode)
	{
		m_pRootNode = pReplaceNode;
	}

	if (pEvalNode != NULL)
	{
		UpdateHeight(pEvalNode);

		Rebalance(pEvalNode);
	}

	pNode->m_pParent = NULL;
	pNode->m_pLeft = NULL;
	pNode->m_pRight = NULL;
	pNode->m_height = 0;
	pNode->m_pTree = NULL;

	--m_nodeCount;
}

template <class K, class V>
TreeNode<K, V>* Tree<K, V>::FindNode(const K& key)
{
	TreeNode<K, V>* pCurrNode = m_pRootNode;
	while (pCurrNode != NULL)
	{
		s32 result = K::Compare(key, pCurrNode->m_key);
		if (result < 0)
		{
			pCurrNode = pCurrNode->m_pLeft;
		}
		else if (result > 0)
		{
			pCurrNode = pCurrNode->m_pRight;
		}
		else
		{
			return pCurrNode;
		}
	}
	return NULL;
}

template <class K, class V>
TreeIterator<K, V> Tree<K, V>::Begin()
{
	TreeNode<K, V>* pCurrNode = m_pRootNode;
	if (pCurrNode != NULL)
	{
		while (pCurrNode->m_pLeft != NULL)
		{
			pCurrNode = pCurrNode->m_pLeft;
		}
	}
	return TreeIterator<K, V>(pCurrNode);
}

/*
 * Private Template
 */
template <class K, class V>
void Tree<K, V>::UpdateHeight(TreeNode<K, V>* pNode)
{
	while (pNode != NULL)
	{
		s32 greatestChildHeight = -1;
		if (pNode->m_pLeft != NULL)
		{
			s32 leftChildHeight = pNode->m_pLeft->m_height;
			if (leftChildHeight > greatestChildHeight)
			{
				greatestChildHeight = leftChildHeight;
			}
		}
		if (pNode->m_pRight != NULL)
		{
			s32 rightChildHeight = pNode->m_pRight->m_height;
			if (rightChildHeight > greatestChildHeight)
			{
				greatestChildHeight = rightChildHeight;
			}
		}

		pNode->m_height = greatestChildHeight + 1;
		pNode = pNode->m_pParent;
	}
}

template <class K, class V>
void Tree<K, V>::Rebalance(TreeNode<K, V>* pNode)
{
	while (pNode != NULL)
	{
		s32 leftChildHeight = -1;
		if (pNode->m_pLeft != NULL)
		{
			leftChildHeight = pNode->m_pLeft->m_height;
		}
		s32 rightChildHeight = -1;
		if (pNode->m_pRight != NULL)
		{
			rightChildHeight = pNode->m_pRight->m_height;
		}

		s32 childBalance = leftChildHeight - rightChildHeight;
		if (childBalance < -1)
		{
			TreeNode<K, V>* pRightNode = pNode->m_pRight;

			s32 leftGrandChildHeight = -1;
			if (pRightNode->m_pLeft != NULL)
			{
				leftGrandChildHeight = pRightNode->m_pLeft->m_height;
			}
			s32 rightGrandChildHeight = -1;
			if (pRightNode->m_pRight != NULL)
			{
				rightGrandChildHeight = pRightNode->m_pRight->m_height;
			}

			s32 grandChildBalance = leftGrandChildHeight - rightGrandChildHeight;
			if (grandChildBalance > 0)
			{
				RotateRight(pRightNode);
			}

			RotateLeft(pNode);
		}
		else if (childBalance > 1)
		{
			TreeNode<K, V>* pLeftNode = pNode->m_pLeft;

			s32 leftGrandChildHeight = -1;
			if (pLeftNode->m_pLeft != NULL)
			{
				leftGrandChildHeight = pLeftNode->m_pLeft->m_height;
			}
			s32 rightGrandChildHeight = -1;
			if (pLeftNode->m_pRight != NULL)
			{
				rightGrandChildHeight = pLeftNode->m_pRight->m_height;
			}

			s32 grandChildBalance = leftGrandChildHeight - rightGrandChildHeight;
			if (grandChildBalance < 0)
			{
				RotateLeft(pLeftNode);
			}

			RotateRight(pNode);
		}

		pNode = pNode->m_pParent;
	}
}

template <class K, class V>
void Tree<K, V>::RotateLeft(TreeNode<K, V>* pNode)
{
	TreeNode<K, V>* pParentNode = pNode->m_pParent;
	TreeNode<K, V>* pRightNode = pNode->m_pRight;
	TreeNode<K, V>* pRightLeftNode = pRightNode->m_pLeft;

	pNode->m_pParent = pRightNode;
	pNode->m_pRight = pRightLeftNode;
	if (pRightLeftNode != NULL)
	{
		pRightLeftNode->m_pParent = pNode;
	}
	pRightNode->m_pLeft = pNode;
	pRightNode->m_pParent = pParentNode;
	if (pParentNode != NULL)
	{
		if (pParentNode->m_pLeft == pNode)
		{
			pParentNode->m_pLeft = pRightNode;
		}
		else
		{
			pParentNode->m_pRight = pRightNode;
		}
	}

	UpdateHeight(pNode);

	if (m_pRootNode == pNode)
	{
		m_pRootNode = pRightNode;
	}
}

template <class K, class V>
void Tree<K, V>::RotateRight(TreeNode<K, V>* pNode)
{
	TreeNode<K, V>* pParentNode = pNode->m_pParent;
	TreeNode<K, V>* pLeftNode = pNode->m_pLeft;
	TreeNode<K, V>* pLeftRightNode = pLeftNode->m_pRight;

	pNode->m_pParent = pLeftNode;
	pNode->m_pLeft = pLeftRightNode;
	if (pLeftRightNode != NULL)
	{
		pLeftRightNode->m_pParent = pNode;
	}
	pLeftNode->m_pRight = pNode;
	pLeftNode->m_pParent = pParentNode;
	if (pParentNode != NULL)
	{
		if (pParentNode->m_pLeft == pNode)
		{
			pParentNode->m_pLeft = pLeftNode;
		}
		else
		{
			pParentNode->m_pRight = pLeftNode;
		}
	}

	UpdateHeight(pNode);

	if (m_pRootNode == pNode)
	{
		m_pRootNode = pLeftNode;
	}
}

//namespace lct
}
//namespace foun
}

#endif//FOUN_TREE_H
