#ifndef _QUEUE_HPP_
#define _QUEUE_HPP_

#include <stdlib.h>
#include <stddef.h>

#include "atomic_t.hpp"

template <typename T, int N> 
class CQueue
{
public:
    inline CQueue()
    {
        _begin_chunk = allocate_chunk();
        _begin_pos = 0;
        _back_pos = 0;
		_end_chunk = _begin_chunk;
		_back_chunk = _end_chunk;
    }

    inline ~CQueue()
    {
        while (true) 
        {
            //只有一个trunk
            if (_begin_chunk == _end_chunk) 
            {
                //释放内存退出
                delete _begin_chunk;
                break;
            }
            //遍历trunk释放内存
            chunk_t *o = _begin_chunk;
            _begin_chunk = _begin_chunk->next;
            delete o;
        }

        chunk_t *sc = _spare_chunk.xchg (NULL);
        delete sc;
    }

    inline T* front() 
    { 
        return &_begin_chunk->values[_begin_pos];
    }

    inline T* back() 
    { 
        return &_back_chunk->values[_back_pos]; 
    }

    inline void push()
    {
        //正在写入的trunk未满，push成功直接返回
        if (++_back_pos != N)
        {
            return;
        }
    
        chunk_t *sc = _spare_chunk.xchg (NULL);
        if (sc) 
        {//如果存在空闲trunk, 将空闲trunk加入到链表中
            _end_chunk->next = sc;
            sc->prev = _end_chunk;
        } 
        else 
        {//不存在空闲trunk, 重新申请trunk, 挂入到链表中
            _end_chunk->next = allocate_chunk ();
            _end_chunk->next->prev = _end_chunk;
        }
        //_end_chunk指向最后一个trunk
        _end_chunk = _end_chunk->next;
        //重新设置下次待写入位置为0
		_back_pos = 0;
		//更新trunk
		_back_chunk = _end_chunk;
    }

    inline void pop()
    {
        //trunk已经读取完成
        if (++_begin_pos == N) 
        {
            //指向下一个trunk，并重新设置读取的位置
            chunk_t *o = _begin_chunk;
            _begin_chunk = _begin_chunk->next;
            _begin_chunk->prev = NULL;
            _begin_pos = 0;

            //设置空闲trunk的值为o, 如果之前空闲trunk不存在，为NULL, free不执行任何操作，如果存在free空闲trunk
            chunk_t *cs = _spare_chunk.xchg(o);
            delete cs;
        }
    }

private:
    struct chunk_t
    {
        T values[N];
        chunk_t *prev;
        chunk_t *next;
    };

    inline chunk_t *allocate_chunk ()
    {
		return new chunk_t;
    }

    chunk_t *_begin_chunk;
    int _begin_pos;
    chunk_t *_back_chunk;
    int _back_pos;
    chunk_t *_end_chunk;
    atomic_ptr_t<chunk_t> _spare_chunk;      
};


#endif
