#include <cstdio>
#include "ypipe.hpp"
#include <thread>

void thread_producer(void* arg)
{
	ypipe_t<int, 100000>* pQueue = (ypipe_t<int, 100000>*)arg;
	for (int i = 0; i < 100000; ++i)
	{
		pQueue->write(i, false);
		pQueue->flush();
	}
}

void thread_consumer(void* arg)
{
	ypipe_t<int, 100000>* pQueue = (ypipe_t<int, 100000>*)arg;
	int a = 0;
	int sum = 0;
	while (true)
	{
		if (pQueue->read(&a))
		{
			sum += a;
			printf("thread_consumer sum=%d\n", sum);
		}
	}
}

int main()
{
	ypipe_t<int, 100000> queue;
	std::thread th1(thread_producer, &queue);
	std::thread th2(thread_consumer, &queue);

	th1.join();
	th2.join();
	
	printf("hello from test_zeromq_pipe!\n");
    return 0;
}