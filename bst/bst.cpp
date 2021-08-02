// bst.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "BSTree.h"

int main()
{
	BSTree tree;
	int data[] = { 7, 4, 10, 3, 6, 5, 12, 18, 22 };
	for (int i = 0; i < sizeof(data) / sizeof(int); ++i)
	{
		tree.AddNode(data[i]);
	}

	tree.InOrderShow();

	int iHigh = tree.Height();

	std::cout << iHigh << std::endl;

	tree.LevelShow();

	BSTNode* node = tree.GetPreNode(3);
	std::cout << "3 null" << std::endl;
	
	node = tree.GetPreNode(7);
	std::cout << node->data << std::endl;

	node = tree.GetPreNode(5);
	std::cout << node->data << std::endl;

	node = tree.GetPreNode(12);
	std::cout << node->data << std::endl;

	node = tree.GetPreNode(6);
	std::cout << node->data << std::endl;

	tree.DelNode(4);
	tree.InOrderShow();

	tree.DelNode(10);
	tree.InOrderShow();

	tree.DelNode(22);
	tree.InOrderShow();

	tree.DelNode(7);
	tree.InOrderShow();

	tree.LevelShow();

	return 0;
}

