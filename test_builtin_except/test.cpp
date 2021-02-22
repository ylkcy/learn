#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#define likely(x) __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

int test(std::vector<int>& vec)
{
    long sum = 0;
    struct timeval tl;
    struct timezone tz;
    gettimeofday(&tl, &tz);

    int64_t t1 = (int64_t)tl.tv_sec * 1000 * 1000 + tl.tv_usec;
    for(auto a : vec)
    {
        if(a > 7)
        {
            sum += 2 * a;
        }
        else
        {
             sum += a;
        }
    }

    gettimeofday(&tl, &tz);
    int64_t t2 = (int64_t)tl.tv_sec * 1000 * 1000 + tl.tv_usec;

    printf("test time: %d, Sum = %d\n", t2 - t1, sum);
}

int test_likely(std::vector<int>& vec)
{
    long sum = 0;
    struct timeval tl;
    struct timezone tz;
    gettimeofday(&tl, &tz);

    int64_t t1 = (int64_t)tl.tv_sec * 1000 * 1000 + tl.tv_usec;
    for(auto a : vec)
    {
        if(unlikely(a > 7))
        {
            sum += 2 * a;
        }
        else
        {
            sum += a;
        }
    }
    
    gettimeofday(&tl, &tz);
    int64_t t2 = (int64_t)tl.tv_sec * 1000 * 1000 + tl.tv_usec;

    printf("test_likely time: %d, Sum = %d\n", t2 - t1, sum);
}

int main()
{
    std::vector<int> vec;
    int M = 1000000;

    for(int i = 0; i < M; ++i)
    {
        vec.push_back(rand() & 0x7);
    }

    test(vec);

    vec.clear();
    for(int i = 0; i < M; ++i)
    {
        vec.push_back(rand() & 0x7);
    }

    test_likely(vec);

    return 0;
}
