#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

CMaxHeap::CMaxHeap(int iCapacity)
{
	m_iCapacity = iCapacity;
	m_iSize = 0;
	m_pData = (int*)malloc(sizeof(int) * iCapacity);
}

CMaxHeap::~CMaxHeap()
{
	if (nullptr != m_pData)
	{
		delete m_pData;
		m_pData = nullptr;
	}
}

int CMaxHeap::GetSize()
{
	return m_iSize;
}

int CMaxHeap::GetCapacity()
{
	return m_iCapacity;
}

int CMaxHeap::Push(int value)
{
	if (m_iSize == m_iCapacity)
	{
		int* pData = nullptr;
		pData = (int*)realloc(m_pData, sizeof(int) * m_iCapacity * 2);
		if (nullptr != pData)
		{
			m_pData = pData;
			m_iCapacity = m_iCapacity << 1;
		}
	}

	m_pData[m_iSize] = value;
	ShiftUp(m_iSize);
	m_iSize++;

	return 0;
}

int CMaxHeap::Replace(int value)
{
	int result = m_pData[0];
	m_pData[0] = value;
	ShiftDown(0);

	return result;
}

void CMaxHeap::ShiftUp(int iIndex)
{
	//iIndex大于0，说明存在父节点
	while (iIndex > 0)
	{
		int iParentIndex = (iIndex - 1) >> 1;

		if (m_pData[iIndex] <= m_pData[iParentIndex])
		{
			break;
		}

		int temp = m_pData[iIndex];
		m_pData[iIndex] = m_pData[iParentIndex];
		m_pData[iParentIndex] = temp;

		iIndex = iParentIndex;
	}
}

void CMaxHeap::ShiftDown(int iIndex)
{
	//存在子节点, 即小于第一个非叶子节点的位置
	//根据完全二叉树的定义， 节点数量 = 度为1的节点数 + 度为0的节点数 + 度为2的节点数
	//总分支数 = 2 * n2(度为2的数量) + n1(度为1的数量)
	//总分支数 = 节点数 - 1 = n2 + n1 + n0 - 1;
	//即 2 * n2 + n1 = n2 + n1 + n0 - 1;
	//即 n2 = n0 - 1;
	// n = n2 + n1 + n0 ==> n = n0 - 1 + n1 + n0 = 2 * n0 + n1 -1;
	//由于为完全二叉树，度为1的节点数要么为0，要么为1
	//n1 = 0; n = 2 * n0 - 1; n0 = (n + 1) >> 2
	//n1 = 1; n = 2 * n0 + 1 - 1; n0 = n >> 2
	int half = m_iSize >> 1;
	while (iIndex <= half)
	{
		int iLeftChildIndex = (iIndex << 1) + 1;
		int iMaxChildIndex = iLeftChildIndex;
		int iRightChildIndex = iLeftChildIndex + 1;
		//判断是否存在右子节点
		if (iRightChildIndex <= m_iSize)
		{
			if (m_pData[iRightChildIndex] > m_pData[iMaxChildIndex])
			{
				iMaxChildIndex = iRightChildIndex;
			}
		}

		if (m_pData[iMaxChildIndex] <= m_pData[iIndex])
		{
			break;
		}

		int temp = m_pData[iIndex];
		m_pData[iIndex] = m_pData[iMaxChildIndex];
		m_pData[iMaxChildIndex] = temp;

		iIndex = iMaxChildIndex;
	}
}

int CMaxHeap::Pop()
{
	if (m_iSize <= 0)
	{
		return -1;
	}

	int value = m_pData[0];
	m_iSize--;
	m_pData[0] = m_pData[m_iSize];
	m_pData[m_iSize] = 0;


	//下浮
	ShiftDown(0);

	return value;
}

void CMaxHeap::Show()
{
	for (int i = 0; i < m_iSize; i++)
	{
		printf("%d_", m_pData[i]);
	}
	printf("\r\n");
}


void CMaxHeap::Heapfiy()
{
	//上浮与选择排序类似，将最大的元素放到最头部

	//下沉与归并排序类似，左右两边已经是有序的
	//叶子节点无需下沉
	for (int iIndex = m_iSize >> 1; iIndex >= 0; iIndex--)
	{
		ShiftDown(iIndex);
	}
}
