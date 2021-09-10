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
	//iIndex����0��˵�����ڸ��ڵ�
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
	//�����ӽڵ�, ��С�ڵ�һ����Ҷ�ӽڵ��λ��
	//������ȫ�������Ķ��壬 �ڵ����� = ��Ϊ1�Ľڵ��� + ��Ϊ0�Ľڵ��� + ��Ϊ2�Ľڵ���
	//�ܷ�֧�� = 2 * n2(��Ϊ2������) + n1(��Ϊ1������)
	//�ܷ�֧�� = �ڵ��� - 1 = n2 + n1 + n0 - 1;
	//�� 2 * n2 + n1 = n2 + n1 + n0 - 1;
	//�� n2 = n0 - 1;
	// n = n2 + n1 + n0 ==> n = n0 - 1 + n1 + n0 = 2 * n0 + n1 -1;
	//����Ϊ��ȫ����������Ϊ1�Ľڵ���ҪôΪ0��ҪôΪ1
	//n1 = 0; n = 2 * n0 - 1; n0 = (n + 1) >> 2
	//n1 = 1; n = 2 * n0 + 1 - 1; n0 = n >> 2
	int half = m_iSize >> 1;
	while (iIndex <= half)
	{
		int iLeftChildIndex = (iIndex << 1) + 1;
		int iMaxChildIndex = iLeftChildIndex;
		int iRightChildIndex = iLeftChildIndex + 1;
		//�ж��Ƿ�������ӽڵ�
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


	//�¸�
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
	//�ϸ���ѡ���������ƣ�������Ԫ�طŵ���ͷ��

	//�³���鲢�������ƣ����������Ѿ��������
	//Ҷ�ӽڵ������³�
	for (int iIndex = m_iSize >> 1; iIndex >= 0; iIndex--)
	{
		ShiftDown(iIndex);
	}
}
