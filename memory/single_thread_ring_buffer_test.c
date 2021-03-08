
#include "ring_buffer.h"
#include <pthread.h>
#include <time.h>

#define BUFFER_SIZE  512

typedef struct student_info
{
    uint64_t stu_id;
    uint32_t age;
    uint32_t score;
}student_info;


void print_student_info(const student_info *stu_info)
{
    assert(stu_info);
    printf("id:%lu\t",stu_info->stu_id);
    printf("age:%u\t",stu_info->age);
    printf("score:%u\n",stu_info->score);
}

student_info * get_student_info(time_t timer)
{
    student_info *stu_info = (student_info *)malloc(sizeof(student_info));
    if (!stu_info)
    {
        fprintf(stderr, "Failed to malloc memory.\n");
        return NULL;
    }
    srand(timer);
    stu_info->stu_id = 10000 + rand() % 9999;
    stu_info->age = rand() % 30;
    stu_info->score = rand() % 101;
    print_student_info(stu_info);
    return stu_info;
}

void * consumer_proc(void *arg)
{
    struct ring_buffer *ring_buf = (struct ring_buffer *)arg;
    student_info stu_info; 
    while(1)
    {
        usleep(2000000);
        printf("------------------------------------------\n");
        printf("get a student info from ring buffer.\n");
        ring_buffer_get(ring_buf, (void *)&stu_info, sizeof(student_info));
        printf("ring buffer read_pos: %u\n", ring_buf->read_pos);
        print_student_info(&stu_info);
        printf("------------------------------------------\n");
    }
    return (void *)ring_buf;
}

void * producer_proc(void *arg)
{
    time_t cur_time;
    struct ring_buffer *ring_buf = (struct ring_buffer *)arg;
    while(1)
    {
        time(&cur_time);
        srand(cur_time);
        int seed = rand() % 11111;
        printf("******************************************\n");
        student_info *stu_info = get_student_info(cur_time + seed);
        printf("put a student info to ring buffer.\n");
        ring_buffer_put(ring_buf, (void *)stu_info, sizeof(student_info));
        printf("ring buffer write_pos: %u\n", ring_buf->write_pos);
        printf("******************************************\n");
        usleep(1000000);
    }
    return (void *)ring_buf;
}

pthread_t consumer_thread(void *arg)
{
    int err;
    pthread_t tid;
    err = pthread_create(&tid, NULL, consumer_proc, arg);
    if (err != 0)
    {
        fprintf(stderr, "Failed to create consumer thread.errno:%u, reason:%s\n",
            errno, strerror(errno));
        return -1;
    }
    return tid;
}


pthread_t producer_thread(void *arg)
{
    int err;
    pthread_t tid;
    err = pthread_create(&tid, NULL, producer_proc, arg);
    if (err != 0)
    {
        fprintf(stderr, "Failed to create consumer thread.errno:%u, reason:%s\n",
            errno, strerror(errno));
        return -1;
    }
    return tid;
}


int main()
{
    void * buffer = NULL;
    uint32_t size = 0;
    struct ring_buffer *ring_buf = NULL;
    pthread_t consume_pid, produce_pid;

    buffer = (void *)malloc(BUFFER_SIZE);
    if (!buffer)
    {
        fprintf(stderr, "Failed to malloc memory.\n");
        return -1;
    }

    ring_buf = ring_buffer_init(BUFFER_SIZE);
    if (!ring_buf)
    {
        fprintf(stderr, "Failed to init ring buffer.\n");
        return -1;
    }

    printf("multi thread test.......\n");
    produce_pid  = producer_thread((void*)ring_buf);
    consume_pid  = consumer_thread((void*)ring_buf);
    printf("producer_id:%d, consume_pid:%d\n", produce_pid, consume_pid);


    pthread_join(produce_pid, NULL);
    pthread_join(consume_pid, NULL);
    ring_buffer_free(ring_buf);

    return 0;
}
