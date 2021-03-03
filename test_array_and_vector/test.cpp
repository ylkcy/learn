#include <stdio.h>
#include <stdlib.h>
#include <vector>
//#include <sys/time.h>

int main()
{
	int* pAry = new int[100000];
	std::vector<int> vec;

	for(int i = 0; i < 100000; i++)
	{
		pAry[i] = i;
		vec.push_back(i);
	}

	int64_t iStartTime = 0;
	int64_t iEndTime = 0;

	int sum1 = 0;
	int sum2 = 0;

	//struct timeval tv = {0};
	//gettimeofday(&tv, NULL);
	//iStartTime = (int64_t)(tv.tv_sec * 1000 * 1000) + (int64_t)tv.tv_usec;

	for(int i = 0; i < 100000; i++)	
	{
		sum1 += pAry[i];
	}

	//gettimeofday(&tv, NULL);
	//iEndTime = (int64_t)(tv.tv_sec * 1000 * 1000) + (int64_t)tv.tv_usec;

	//printf("Array ms: %ld\n", iEndTime - iStartTime);
	//
	//gettimeofday(&tv, NULL);
	//iStartTime = (int64_t)(tv.tv_sec * 1000 * 1000) + (int64_t)tv.tv_usec;

	for(int i = 0; i < 100000; i++)
	{
		sum2 += vec[i];
	}

	//gettimeofday(&tv, NULL);
	//iEndTime = (int64_t)(tv.tv_sec * 1000 * 1000) + (int64_t)tv.tv_usec;

	//printf("Array ms: %ld\n", iEndTime - iStartTime);
	printf("sum1: %d, sum2: %d\n", sum1, sum2);

	return 0;
}
