#ifndef _ATOMIC_T_HPP_
#define _ATOMIC_T_HPP_

#include <stddef.h>

inline void *atomic_xchg_ptr (void **ptr_, void *const val_)
{
    void *old;
    __asm__ volatile("lock; xchg %0, %2"
                        : "=r"(old), "=m"(*ptr_)
                        : "m"(*ptr_), "0"(val_));
    return old;
}

inline void *atomic_cas(void *volatile *ptr_, void *cmp_, void *val_)
{
    void *old;
    __asm__ volatile("lock; cmpxchg %2, %3"
                    : "=a"(old), "=m"(*ptr_)
                    : "r"(val_), "m"(*ptr_), "0"(cmp_)
                    : "cc");
    return old;
}

template <typename T> 
class atomic_ptr_t
{
public:
    atomic_ptr_t () 
    { 
        _ptr = NULL; 
    }

    void set (T *ptr_)  
    { 
        _ptr = ptr_; 
    }

    T *xchg (T *val_)
    {
        return (T *)atomic_xchg_ptr((void **) &_ptr, val_);
    }

    T *cas (T *cmp_, T *val_)
    {
        return (T *)atomic_cas((void **) &_ptr, cmp_, val_);
    }

    volatile T *_ptr;
};

    
struct atomic_value_t
{
    atomic_value_t (const int value_) : _value (value_) 
    {

    }

    atomic_value_t (const atomic_value_t &src_) : _value (src_.load ())
    {

    }

    void store (const int value_)
    {
        atomic_xchg_ptr((void **) &_value, (void *) (ptrdiff_t)value_);
    }

    int load () const
    {
        return (int)(ptrdiff_t)atomic_cas((void **)&_value, 0, 0);
    }

private:
    volatile ptrdiff_t _value;
};


#endif
