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

struct ring_buffer* ring_buffer_init(uint32_t size);
void ring_buffer_free(struct ring_buffer *ring_buf);
uint32_t ring_buffer_get(struct ring_buffer *ring_buf, void * buffer, uint32_t size);
uint32_t ring_buffer_put(struct ring_buffer *ring_buf, void *buffer, uint32_t size);


#endif
