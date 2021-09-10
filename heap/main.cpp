#include "heap.h"
#include <stdio.h>
#include <string.h>

int main()
{
	CMaxHeap heap;
	int ary[] = { 2, 5, 3, 4, 8, 6, 7, 9, 10, 12, 19 };
 	
	memcpy(heap.m_pData, ary, sizeof(ary));

	heap.m_iSize = sizeof(ary) / sizeof(int);

	heap.Show();

	heap.Heapfiy();
	/*
	for (int i = 0; i < sizeof(ary) / sizeof(int); i++)
	{
		heap.Push(ary[i]);
	}

	for (int i = 0; i < sizeof(ary) / sizeof(int); i++)
	{
		int ivalue = heap.Pop();
		printf("%d\r\n", ivalue);
	}
	*/
	heap.Show();

	return 0;
}