#include "queue.hpp"
#include <sys/time.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	int64_t iFinishBeginTime = 0;
 	int64_t iFinishEndTime = 0;
    struct timeval tv;
 	struct timezone tz;
    gettimeofday(&tv, &tz);
    iFinishBeginTime = (int64_t)tv.tv_sec *1000 * 1000 + (int64_t)tv.tv_usec;
	class queue_t<int, 10> q;
	for(int i = 0; i < 100000; ++i)
	{
		q.push();
	}

	for (int i = 0; i < 100000; ++i)
	{
		q.pop();
	}

	gettimeofday(&tv, &tz);
 	iFinishEndTime = (int64_t)tv.tv_sec *1000 * 1000 + (int64_t)tv.tv_usec;
    printf("push us: %ld\n", iFinishEndTime - iFinishBeginTime);

	getchar();
	
	return 0;
}
