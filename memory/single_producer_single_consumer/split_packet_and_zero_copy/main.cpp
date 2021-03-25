#include <cstdio>
#include "pipe.hpp"
#include <thread>
#include <string.h>
#include <unistd.h>

//发送数据缓存块
typedef struct tagBlockNode
{
	void	   *m_pBlock;		///用于存放数据内存。
	int32_t		m_iSize;  		///内存大小
	int32_t		m_iLen;			///实际数据大小
	int32_t		m_iSession;		///发送时session编号，用于判断发送绘画是否同一个
	int64_t 	m_iInsertTime;	///插入的时间

	tagBlockNode()
	{
		m_pBlock = NULL;
		m_iSize = m_iLen = m_iSession = 0;
		Realloc(256);
	}

	~tagBlockNode()
	{
		if (m_pBlock)
		{
			free(m_pBlock);
		}
	}

	int32_t Realloc(int32_t iReallocSize)
	{
		if (iReallocSize == m_iSize)
		{
			return 0;
		}

		void *pTemp = realloc(m_pBlock, iReallocSize);
		if (!pTemp)
		{
			return -1;
		}

		m_pBlock = pTemp;
		m_iSize = iReallocSize;

		return 0;
	}

	/// 增加拷贝数据功能，如果大小不够，自动扩充
	int32_t CopyData(void *pBuffer, int32_t iLength, int64_t iInsertTime, int32_t iSession = 0)
	{
		int iRtn;

		if (m_iSize < iLength)
		{
			iRtn = Realloc(iLength);
			if (iRtn != 0)
			{
				return iRtn;
			}
		}

		memcpy(m_pBlock, pBuffer, iLength);

		m_iLen = iLength;
		m_iSession = iSession;
		m_iInsertTime = iInsertTime;

		return 0;
	}
} BLOCK_NODE, *P_BLOCK_NODE;

void thread_producer(void* arg)
{
	CPipe<BLOCK_NODE, 1000>* pQueue = (CPipe<BLOCK_NODE, 1000>*)arg;

	for (int i = 0; i < 10000; ++i)
	{
		P_BLOCK_NODE pNode = pQueue->back();
		pNode->m_iLen = 4;
		pNode->m_iSession = i;
		pNode->m_iSize = 256;
		pQueue->write();
		
		if (i == 9999)
		{
			printf("producer finished\n");
		}
	}
}

void thread_consumer(void* arg)
{
	int i = 0;
	CPipe<BLOCK_NODE, 1000>* pQueue = (CPipe<BLOCK_NODE, 1000>*)arg;
	BLOCK_NODE* pPacket = NULL;
	while (true)
	{
		if (pQueue->read(&pPacket))
		{
			i++;
			printf("%d\n", pPacket->m_iSession);
		}
		if (9999 == i)
		{
			break;
		}
	}
}

int main()
{
	CPipe<BLOCK_NODE, 1000> queue;
	std::thread th1(thread_producer, &queue);
	std::thread th2(thread_consumer, &queue);

	th1.join();
	th2.join();

	printf("hello from test_zeromq_pipe!\n");
    return 0;
}