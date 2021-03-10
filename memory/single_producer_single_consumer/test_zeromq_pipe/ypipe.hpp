#ifndef __ZMQ_YPIPE_HPP_INCLUDED__
#define __ZMQ_YPIPE_HPP_INCLUDED__

#include "atomic_t.hpp"
#include "yqueue.hpp"
#include "ypipe_base.hpp"


template <typename T, int N> 
class ypipe_t : public ypipe_base_t<T>
{
  public:
    //  Initialises the pipe.
    ypipe_t ()
    {
        //  Insert terminator element into the queue.
        _queue.push ();

        //  Let all the pointers to point to the terminator.
        //  (unless pipe is dead, in which case c is set to NULL).
        _r = _w = _f = &_queue.back();
        _c.set (&_queue.back());
    }

    //  Write an item to the pipe.  Don't flush it yet. If incomplete is
    //  set to true the item is assumed to be continued by items
    //  subsequently written to the pipe. Incomplete items are never
    //  flushed down the stream.
    void write (const T &value_, bool incomplete_)
    {
        //  Place the value to the queue, add new terminator element.
        _queue.back() = value_;
        _queue.push();

        //  Move the "flush up to here" poiter.
        if (!incomplete_)
            _f = &_queue.back ();
    }

    //  Pop an incomplete item from the pipe. Returns true if such
    //  item exists, false otherwise.
    bool unwrite (T *value_)
    {
        if (_f == &_queue.back ())
            return false;
        _queue.unpush ();
        *value_ = _queue.back ();
        return true;
    }

    //  Flush all the completed items into the pipe. Returns false if
    //  the reader thread is sleeping. In that case, caller is obliged to
    //  wake the reader up before using the pipe again.
    bool flush ()
    {
        //  If there are no un-flushed items, do nothing.
        if (_w == _f)
            return true;

        //  Try to set 'c' to 'f'.
        if (_c.cas (_w, _f) != _w) 
        {
            //  Compare-and-swap was unseccessful because 'c' is NULL.
            //  This means that the reader is asleep. Therefore we don't
            //  care about thread-safeness and update c in non-atomic
            //  manner. We'll return false to let the caller know
            //  that reader is sleeping.
            _c.set (_f);
            _w = _f;
            return false;
        }

        //  Reader is alive. Nothing special to do now. Just move
        //  the 'first un-flushed item' pointer to 'f'.
        _w = _f;
        return true;
    }

    //  Check whether item is available for reading.
    bool check_read ()
    {
        //  Was the value prefetched already? If so, return.
        if (&_queue.front () != _r && _r)
            return true;

        //  There's no prefetched value, so let us prefetch more values.
        //  Prefetching is to simply retrieve the
        //  pointer from c in atomic fashion. If there are no
        //  items to prefetch, set c to NULL (using compare-and-swap).
        _r = _c.cas (&_queue.front(), NULL);

        //  If there are no elements prefetched, exit.
        //  During pipe's lifetime r should never be NULL, however,
        //  it can happen during pipe shutdown when items
        //  are being deallocated.
        if (&_queue.front() == _r || !_r)
            return false;

        //  There was at least one value prefetched.
        return true;
    }

    //  Reads an item from the pipe. Returns false if there is no value.
    //  available.
    bool read (T *value_)
    {
        //  Try to prefetch a value.
        if (!check_read ())
            return false;

        //  There was at least one value prefetched.
        //  Return it to the caller.
        *value_ = _queue.front ();
        _queue.pop ();
        return true;
    }

    //  Applies the function fn to the first elemenent in the pipe
    //  and returns the value returned by the fn.
    //  The pipe mustn't be empty or the function crashes.
    bool probe (bool (*fn_) (const T &))
    {
        const bool rc = check_read ();

        return (*fn_) (_queue.front ());
    }

  protected:
    //  Allocation-efficient queue to store pipe items.
    //  Front of the queue points to the first prefetched item, back of
    //  the pipe points to last un-flushed item. Front is used only by
    //  reader thread, while back is used only by writer thread.
	queue_t<T, N> _queue;

    //  Points to the first un-flushed item. This variable is used
    //  exclusively by writer thread.
    T *_w;

    //  Points to the first un-prefetched item. This variable is used
    //  exclusively by reader thread.
    T *_r;

    //  Points to the first item to be flushed in the future.
    T *_f;

    //  The single point of contention between writer and reader thread.
    //  Points past the last flushed item. If it is NULL,
    //  reader is asleep. This pointer should be always accessed using
    //  atomic operations.
    atomic_ptr_t<T> _c;
};

#endif
