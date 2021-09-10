#ifndef _HEAP_H_
#define _HEAP_H_

class CMaxHeap
{
public:
	CMaxHeap(int iCapacity = 100);
	~CMaxHeap();
	int GetSize();
	int GetCapacity();
	int Push(int value);
	int Pop();
	//�ϸ�
	void ShiftUp(int iIndex);
	//����
	void ShiftDown(int iIndex);

	int Replace(int value);

	void Heapfiy();

	void Show();
public:
	int* m_pData;
	int m_iCapacity;
	int m_iSize;
};

#endif
