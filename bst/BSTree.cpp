#include "BSTree.h"


BSTNode::BSTNode(int data, BSTNode* parent)
{
	this->data = data;
	this->parent = parent;
	this->left = NULL;
	this->right = NULL;
}

BSTNode::~BSTNode()
{
	this->parent = NULL;
	this->left = NULL;
	this->right = NULL;
}

BSTree::BSTree()
{
	this->root = NULL;
	this->size = 0;
}

BSTree::~BSTree()
{

}

void BSTree::AddNode(int data)
{
	if (NULL == root)
	{
		this->root = new BSTNode(data, NULL);
		this->size++;
		return;
	}

	BSTNode* node = root;
	BSTNode* parent = NULL;
	int icmp = -1;

	while (NULL != node)
	{
		parent = node;
		if (data < node->data)
		{
			node = node->left;
			icmp = 0;
		}
		else if (data > node->data)
		{
			node = node->right;
			icmp = 1;
		}
		else
		{
			node->data = data;
		}
	}

	if (0 == icmp)
	{
		parent->left = new BSTNode(data, parent);
	}
	else
	{
		parent->right = new BSTNode(data, parent);
	}

	this->size++;
}


BSTNode* BSTree::FindNode(int data)
{
	BSTNode* node = root;

	while (NULL != node)
	{
		if (data < node->data)
		{
			node = node->left;
		}
		else if (data > node->data)
		{
			node = node->right;
		}
		else
		{
			break;
		}
	}

	return node;
}

void BSTree::InOrderShow()
{
	InOrderShow(root);
	printf("\n");
}

void BSTree::InOrderShow(BSTNode* node)
{
	if (node)
	{
		InOrderShow(node->left);
		printf("%d ", node->data);
		InOrderShow(node->right);
	}
}

int BSTree::Height()
{
	return Height(root);
}

int BSTree::Height(BSTNode* node)
{
	if (NULL == node)
	{
		return 0;
	}
	int iLeft = Height(node->left);
	int iRight = Height(node->right);
	int iHeight = iLeft < iRight ? iRight : iLeft;
	return iHeight + 1;
}

void BSTree::DelNode(int data)
{
	BSTNode* node = root;
	int icmp = -1;

	while (NULL != node)
	{
		if (data < node->data)
		{
			node = node->left;
			icmp = 0;
		}
		else if (data > node->data)
		{
			node = node->right;
			icmp = 1;
		}
		else
		{
			break;
		}
	}

	if (NULL == node)
	{
		return ;
	}

	if (NULL != node->left && NULL != node->right)
	{
		BSTNode* delNode = GetPreNode(data);
		node->data = delNode->data;
		node = delNode;
	}

	if (NULL == node->left && NULL == node->right)
	{
		if (NULL == node->parent)
		{
			root = NULL;
		}
		else
		{
			if (0 == icmp)
			{
				node->parent->left = NULL;
			}
			else
			{
				node->parent->right = NULL;
			}
		}
	}
	else if (NULL != node->left && NULL == node->right)
	{
		if (NULL == node->parent)
		{
			root = node->left;
			node->left->parent = NULL;
		}
		else
		{
			if (node->parent->left == node)
			{
				node->parent->left = node->left;
			}
			else
			{
				node->parent->right = node->left;
			}
			node->left->parent = node->parent;
		}
	}
	else
	{
		if (NULL == node->parent)
		{
			root = node->right;
			node->left->parent = NULL;
		}
		else
		{
			if (node->parent->left == node)
			{
				node->parent->left = node->right;
			}
			else
			{
				node->parent->right = node->right;
			}
			
			node->right->parent = node->parent;
		}
	}
	
	if (NULL != node)
	{
		delete node;
		node = NULL;
	}

	size--;
}

BSTNode* BSTree::GetPreNode(int data)
{
	BSTNode* node = FindNode(data);
	if (NULL == node)
	{
		return node;
	}

	BSTNode* preNode = NULL;
	if (NULL != node->left)
	{
		preNode = node->left;
		while (NULL != preNode->right)
		{
			preNode = preNode->right;
		}
		return preNode;
	}

	if (NULL != node->parent)
	{
		while (NULL != node->parent && node->parent->right != node)
		{
			node = node->parent;
		}
	}
	
	return node->parent;
}

BSTNode* BSTree::GetNextNode(int data)
{
	BSTNode* node = FindNode(data);
	if (NULL == node)
	{
		return node;
	}

	BSTNode* nextNode = NULL;
	if (NULL != node->right)
	{
		nextNode = node->right;
		while (NULL != nextNode->left)
		{
			nextNode = nextNode->left;
		}
		return nextNode;
	}

	if (NULL != node->parent)
	{
		while (NULL != node->parent && node->parent->left != node)
		{
			node = node->parent;
		}
	}

	return node->parent;
}

void BSTree::LevelShow()
{
	if (NULL == root)
	{
		return;
	}
	
	std::queue<BSTNode*> q;
	BSTNode* node = root;

	q.push(node);

	while (!q.empty())
	{
		node = q.front();
		printf("%d ", node->data);
		q.pop();

		if (node->left)
		{
			q.push(node->left);
		}

		if (node->right)
		{
			q.push(node->right);
		}
	}

	printf("\n");
}

int BSTree::Height2()
{
	if (NULL == root)
	{
		return 0;
	}

	std::queue<BSTNode*> q;
	BSTNode* node = root;
	int iHeight = 0;
	int iLevelSize = 1;
	q.push(node);

	while (!q.empty())
	{
		node = q.front();
		q.pop();

		if (node->left)
		{
			q.push(node->left);
		}

		if (node->right)
		{
			q.push(node->right);
		}

		iLevelSize--;
		if (0 == iLevelSize)
		{
			iLevelSize = q.size();
			iHeight++;
		}
	}

	return iHeight;
}

