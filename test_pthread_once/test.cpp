#include <iostream>
#include <pthread.h>
using namespace std;

template <typename T> 
class Singleton 
{
public:
  	static T *getInstance() 
	{
    		pthread_once(&once_control, &Singleton::init);
    		return value_;
  	}
  	static void destroy() 
	{
    		if (nullptr != value_) 
		{
      			delete value_;
    		}
  	}

private:
 	Singleton();
  	~Singleton();
  	static void init() 
	{
    		value_ = new T();
    		::atexit(destroy);
  	}
  	static T *value_;
  	static pthread_once_t once_control;
};

template <typename T>
pthread_once_t Singleton<T>::once_control = PTHREAD_ONCE_INIT;

template <typename T> T *Singleton<T>::value_ = nullptr;

class A
{
public:
	A() {std::cout << "A" << std::endl;}
};

void* thread_fun(void* p)
{
	A* pA = Singleton<A>::getInstance();
}

int main(int argc, char* argv[])
{
	pthread_t tid;
	void* status;
	pthread_create(&tid, NULL, thread_fun, NULL);

	A* pA = Singleton<A>::getInstance();
	pthread_join(tid, &status);
	
	return 0;
}

