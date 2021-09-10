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
	//…œ∏°
	void ShiftUp(int iIndex);
	//œ¬¬À
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
