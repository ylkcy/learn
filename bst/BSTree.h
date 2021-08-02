#ifndef _BSTREE_H_
#define _BSTREE_H_

#include <stdio.h>
#include <queue>

class BSTNode
{
public:
	BSTNode(int data, BSTNode* parent);
	~BSTNode();
	BSTNode* left;
	BSTNode* right;
	BSTNode* parent;
	int data;
};

class BSTree
{
public:
	BSTree();
	~BSTree();
public:
	void AddNode(int data);
	BSTNode* FindNode(int data);
	int Height();
	int Height(BSTNode* node);

	int Height2();
	void InOrderShow();
	void InOrderShow(BSTNode* node);
	void LevelShow();
	void DelNode(int data);
	BSTNode* GetPreNode(int data);
	BSTNode* GetNextNode(int data);
	bool IsFullTree();
private:
	BSTNode* root;
	int size;
};

#endif

