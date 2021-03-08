#ifndef KFIFO_HEADER_H 
#define KFIFO_HEADER_H

#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#define ALIGN 2
//判断x是否是2的次方
#define is_power_of_2(x) ((x) != 0 && (((x) & ((x) - 1)) == 0))
//取a和b中最小值
#define min(a, b) (((a) < (b)) ? (a) : (b))
//将x调整为ALIGN的倍数
#define adjust_power_of_2(x) (((x) + (size_t)ALIGN - 1) & ~((size_t)ALIGN - 1));   

struct ring_buffer
{
    void         *buffer;         //缓冲区
    uint32_t      size;           //大小
    uint32_t      read_pos;       //读位置
    uint32_t      write_pos;      //写位置
};

//初始化缓冲区
struct ring_buffer* ring_buffer_init(uint32_t size)
{
    struct ring_buffer *ring_buf = NULL;
    if (!is_power_of_2(size))
    {
        size = adjust_power_of_2(size);
    }

    ring_buf = (struct ring_buffer *)malloc(sizeof(struct ring_buffer));
    if (!ring_buf)
    {
        fprintf(stderr, "Failed to malloc memory,errno:%u,reason:%s",
            errno, strerror(errno));
        return ring_buf;
    }

    ring_buf->buffer = malloc(size);
    if(!ring_buf->buffer)
    {
        fprintf(stderr, "Failed to malloc memory,errno:%u,reason:%s",
            errno, strerror(errno));
        free(ring_buf);
        ring_buf = NULL;
        return ring_buf;
    }

    memset(ring_buf->buffer, 0, size);
    ring_buf->size = size;
    ring_buf->read_pos = 0;
    ring_buf->write_pos = 0;

    return ring_buf;
}

//释放缓冲区
void ring_buffer_free(struct ring_buffer *ring_buf)
{
    if (ring_buf)
    {
        if (ring_buf->buffer)
        {
            free(ring_buf->buffer);
            ring_buf->buffer = NULL;
        }
        free(ring_buf);
        ring_buf = NULL;
    }
}

//从缓冲区中取数据
uint32_t ring_buffer_get(struct ring_buffer *ring_buf, void * buffer, uint32_t size)
{
    uint32_t len = 0;
    //获取可读取的数据大小 = write_pos - read_pos
    size  = min(size, ring_buf->write_pos - ring_buf->read_pos);        
    /* first get the data from fifo->out until the end of the buffer */
    len = min(size, ring_buf->size - (ring_buf->read_pos & (ring_buf->size - 1)));
    memcpy(buffer, ring_buf->buffer + (ring_buf->read_pos & (ring_buf->size - 1)), len);
    /* then get the rest (if any) from the beginning of the buffer */
    memcpy(buffer + len, ring_buf->buffer, size - len);
    ring_buf->read_pos += size;
    return size;
}

//向缓冲区中存放数据
uint32_t ring_buffer_put(struct ring_buffer *ring_buf, void *buffer, uint32_t size)
{
    uint32_t len = 0;
    //获取可写入的数据 = size - (write_pos - read_pos);
    size = min(size, ring_buf->size - ring_buf->write_pos + ring_buf->read_pos);
    /* first put the data starting from fifo->write_pos to buffer end */
    len  = min(size, ring_buf->size - (ring_buf->write_pos & (ring_buf->size - 1)));
    memcpy(ring_buf->buffer + (ring_buf->write_pos & (ring_buf->size - 1)), buffer, len);
    /* then put the rest (if any) at the beginning of the buffer */
    memcpy(ring_buf->buffer, buffer + len, size - len);
    ring_buf->write_pos += size;
    return size;
}


#endif
