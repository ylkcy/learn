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
		//ͨ��_c�Ƿ����仯���ж��Ƿ������ݿɶ�, д�߳�ʼ�պ�_w���
		//���߳���������ݿɶ���������ΪNULL
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
		//������һ������ֵ���±�_end_pos
        _queue.push();
		//����дָ��
		_w = back();
		//����_c = _w
		_c.set(back());
    }

	//����Ƿ������ݿɶ�
    bool check_read ()
    {        
		//�ж�_c�Ƿ�ָ����ף�ָ����ף�˵��������, ����ΪNULL
		//���򷵻�д���λ��
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
