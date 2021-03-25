#ifndef __ZMQ_YPIPE_HPP_INCLUDED__
#define __ZMQ_YPIPE_HPP_INCLUDED__

#include "atomic_t.hpp"
#include "queue.hpp"

template <typename T, int N> 
class CPipe
{
public:
	CPipe()
    {
        _r = _w = _queue.back();
		//通过_c是否发生变化来判断是否有数据可读, 写线程始终和_w相等
		//读线程如果无数据可读，会设置为NULL
        _c.set (_queue.back());
    }

	T* front()
	{
		return _queue.front();
	}

	T* back()
	{
		return _queue.back();
	}

    void write()
    {
		//更新下一次设置值的下标_end_pos
        _queue.push();
		//更新写指针
		_w = back();
		//设置_c = _w
		_c.set(back());
    }

	//检测是否有数据可读
    bool check_read ()
    {        
		//判断_c是否指向队首，指向队首，说明无数据, 设置为NULL
		//否则返回写入的位置
        _r = _c.cas (front(), NULL);

		if (front() == _r || !_r)
		{
			return false;
		}
        
        return true;
    }

    bool read (T** value_)
    {
		if (!check_read())
		{
			return false;
		}
        
        *value_ = front();
        _queue.pop();
        return true;
    }
private:
	CQueue<T, N> _queue;
    T *_w;
    T *_r;
    atomic_ptr_t<T> _c;
};

#endif
