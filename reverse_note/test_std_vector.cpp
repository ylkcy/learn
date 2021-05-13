/*
平台： X64 
long pointer 类型所占字节大小为8
迭代器基本知识
iterator 
	_M_current	TYPE*

vector中的迭代器
Random-access Iterators

//! Iterator used to iterate through a vector
template <class Pointer>
class vector_iterator
   :  public vector_const_iterator<Pointer>
{
   public:
   explicit vector_iterator(Pointer ptr)
	  : vector_const_iterator<Pointer>(ptr)
   {}
public:
	typedef std::random_access_iterator_tag                                       iterator_category;
	typedef typename boost::intrusive::pointer_traits<Pointer>::element_type      value_type;
	typedef typename boost::intrusive::pointer_traits<Pointer>::difference_type   difference_type;
	typedef Pointer                                                               pointer;
	typedef value_type&                                                           reference;

   //Constructors
   vector_iterator()
   {}

   //Pointer like operators
   reference operator*()  const
   {  return *this->m_ptr;  }

   value_type* operator->() const
   {  return  container_detail::to_raw_pointer(this->m_ptr);  }

   reference operator[](difference_type off) const
   {  return this->m_ptr[off];   }

   //Increment / Decrement
   vector_iterator& operator++()
   {  ++this->m_ptr; return *this;  }

   vector_iterator operator++(int)
   {  pointer tmp = this->m_ptr; ++*this; return vector_iterator(tmp);  }

   vector_iterator& operator--()
   {  --this->m_ptr; return *this;  }

   vector_iterator operator--(int)
   {  vector_iterator tmp = *this; --*this; return vector_iterator(tmp); }

   // Arithmetic
   vector_iterator& operator+=(difference_type off)
   {  this->m_ptr += off;  return *this;  }

   vector_iterator operator+(difference_type off) const
   {  return vector_iterator(this->m_ptr+off);  }

   friend vector_iterator operator+(difference_type off, const vector_iterator& right)
   {  return vector_iterator(off + right.m_ptr); }

   vector_iterator& operator-=(difference_type off)
   {  this->m_ptr -= off; return *this;   }

   vector_iterator operator-(difference_type off) const
   {  return vector_iterator(this->m_ptr-off);  }

   difference_type operator-(const vector_const_iterator<Pointer>& right) const
   {  return static_cast<const vector_const_iterator<Pointer>&>(*this) - right;   }
};

Allocator基本知识
	std::allocator<T>
vector 基本知识

template <class _Tp, class _Allocator>
class _Vector_alloc_base 
{
public:
  typedef typename _Alloc_traits<_Tp, _Allocator>::allocator_type
		  allocator_type;
  allocator_type get_allocator() const { return _M_data_allocator; }

  _Vector_alloc_base(const allocator_type& __a)
	: _M_data_allocator(__a), _M_start(0), _M_finish(0), _M_end_of_storage(0)
  {}

protected:
  allocator_type _M_data_allocator;
  _Tp* _M_start;
  _Tp* _M_finish;
  _Tp* _M_end_of_storage;

  _Tp* _M_allocate(size_t __n)
	{ return _M_data_allocator.allocate(__n); }
  void _M_deallocate(_Tp* __p, size_t __n)
	{ if (__p) _M_data_allocator.deallocate(__p, __n); }
};

内存布局
类型		名称                字段含义 
_M_impl
	iterator	_M_start_			表示目前空间所使用的头 TYPE*
	iterator	_M_finish_			表示目前空间所使用的尾 TYPE*
	iterator	_M_end_of_storage   表示可用空间的尾 TYPE*

//申明
template<class T, class Allocator = std::allocator<T>> 
class vector;
//构造
vector();
vector(size_type count, const T& value, const Allocator& alloc = Allocator());
vector(std::initializer_list<T> init,  const Allocator& alloc = Allocator());

begin() {return _M_start_;}
end() {return _M_finish_;}
size() {return (end() - begin());}
capacity() {return _M_end_of_storage - begin();}
empty() {return begin() == end();}
operator[](size_type n) {return *(begin() + n);}
front() {return *begin();}
back() {return *(end() - 1);}
*/

/*
IDA F5
int __cdecl main(int argc, const char **argv, const char **envp)
{
  std::vector<int>::reference v3; // rax
  __int64 v4; // rax
  std::vector<int>::reference v5; // rax
  __int64 v6; // rax
  std::vector<int>::reference v7; // rax
  __int64 v8; // rax
  std::vector<int>::iterator v9; // rax
  __gnu_cxx::__normal_iterator<int*,std::vector<int> > v10; // rax
  int *v11; // rbx
  std::vector<int>::iterator v12; // rax
  int *v13; // rbx
  std::vector<int>::iterator v14; // rax
  std::vector<int>::reference v15; // rax
  __int64 v16; // rax
  std::vector<int>::size_type v17; // rbx
  __gnu_cxx::__normal_iterator<int*,std::vector<int> >::reference v18; // rax
  __int64 v19; // rax
  std::vector<int>::iterator it; // [rsp+0h] [rbp-E0h] BYREF
  std::vector<int> vec2; // [rsp+10h] [rbp-D0h] BYREF
  std::vector<int> vec1; // [rsp+30h] [rbp-B0h] BYREF
  std::vector<int> vec; // [rsp+50h] [rbp-90h] BYREF
  std::allocator<int> __a; // [rsp+77h] [rbp-69h] BYREF
  std::vector<int>::value_type __value; // [rsp+78h] [rbp-68h] BYREF
  std::allocator<int> v27; // [rsp+7Fh] [rbp-61h] BYREF
  std::vector<int>::value_type __x; // [rsp+80h] [rbp-60h] BYREF
  std::vector<int>::value_type v29; // [rsp+84h] [rbp-5Ch] BYREF
  __gnu_cxx::__normal_iterator<int*,std::vector<int> >::difference_type __n; // [rsp+88h] [rbp-58h] BYREF
  __gnu_cxx::__normal_iterator<int*,std::vector<int> > v31; // [rsp+90h] [rbp-50h] BYREF
  __gnu_cxx::__normal_iterator<int*,std::vector<int> >::difference_type v32; // [rsp+98h] [rbp-48h] BYREF
  __gnu_cxx::__normal_iterator<int*,std::vector<int> > v33; // [rsp+A0h] [rbp-40h] BYREF
  __gnu_cxx::__normal_iterator<int*,std::vector<int> > __rhs; // [rsp+B0h] [rbp-30h] BYREF
  int i; // [rsp+BCh] [rbp-24h]

  //构造函数vector();
  //对应代码vector<int> vec;
  std::vector<int>::vector(&vec);

  //构造函数vector(size_type count, const T& value, const Allocator& alloc = Allocator());
  //对应代码vector<int> vec1(2, 5);
  std::allocator<int>::allocator(&__a);
  __value = 5;
  std::vector<int>::vector(&vec1, 2uLL, &__value, &__a);
  std::allocator<int>::~allocator(&__a);

  //.rodata:0000000000403D20 __89            dd 1, 2, 3, 4, 5        ; DATA XREF: main+66↑o
  //构造函数 vector(std::initializer_list<T> init,  const Allocator& alloc = Allocator());
  //对应代码 vector<int> vec2 = { 1, 2, 3, 4, 5 };
  std::allocator<int>::allocator(&v27);
  std::vector<int>::vector(&vec2, (std::initializer_list<int>)__PAIR128__(5LL, &_89), &v27);
  std::allocator<int>::~allocator(&v27);

  //operator[](size_type n) {return *(begin() + n);}
  //begin() {return _M_start_;}
  //对应代码 std::cout << vec2[1] << std::endl;
  v3 = std::vector<int>::operator[](&vec2, 1uLL);
  v4 = std::ostream::operator<<(&std::cout, (unsigned int)*v3);
  std::ostream::operator<<(v4, &std::endl<char,std::char_traits<char>>);

  //operator[](size_type n) {return *(begin() + n);}
  //begin() {return _M_start_;}
  //对应代码std::cout << vec2.begin() << std::endl;
  v5 = std::vector<int>::front(&vec2);
  v6 = std::ostream::operator<<(&std::cout, (unsigned int)*v5);
  std::ostream::operator<<(v6, &std::endl<char,std::char_traits<char>>);

  //operator[](size_type n) {return *(begin() + n);}
  //begin() {return _M_start_;}
  //对应代码std::cout << vec2.back() << std::endl;
  v7 = std::vector<int>::back(&vec2);
  v8 = std::ostream::operator<<(&std::cout, (unsigned int)*v7);
  std::ostream::operator<<(v8, &std::endl<char,std::char_traits<char>>);

  //技巧, 不用看前面的东西，直接查看对应的函数
  //针对迭代器，忽略到里面的成员
  //对应代码 
  //it = vec1.begin();
  //vec1.insert(it, 999);
  __x = 999;
  v9._M_current = std::vector<int>::begin(&vec1)._M_current;
  std::vector<int>::insert(&vec1, v9, &__x);

  ///vec2.push_back(888);
  ///vec2.pop_back();
  v29 = 888;
  std::vector<int>::push_back(&vec2, &v29);
  std::vector<int>::pop_back(&vec2);

  ///vec2.erase(vec2.end() - 1);
  __n = 1LL;
  v31._M_current = std::vector<int>::end(&vec2)._M_current;
  v10._M_current = __gnu_cxx::__normal_iterator<int *,std::vector<int>>::operator-(&v31, &__n)._M_current;
  std::vector<int>::erase(&vec2, v10);

  //std::sort(vec2.begin(), vec2.end());
  v11 = std::vector<int>::end(&vec2)._M_current;
  v12._M_current = std::vector<int>::begin(&vec2)._M_current;
  std::sort<__gnu_cxx::__normal_iterator<int *,std::vector<int>>>(
	v12,
	(__gnu_cxx::__normal_iterator<int*,std::vector<int> >)v11);

  //std::sort(vec2.begin(), vec2.begin() + 3);
  v32 = 3LL;
  v33._M_current = std::vector<int>::begin(&vec2)._M_current;
  v13 = __gnu_cxx::__normal_iterator<int *,std::vector<int>>::operator+(&v33, &v32)._M_current;
  v14._M_current = std::vector<int>::begin(&vec2)._M_current;
  std::sort<__gnu_cxx::__normal_iterator<int *,std::vector<int>>>(
	v14,
	(__gnu_cxx::__normal_iterator<int*,std::vector<int> >)v13);
  std::operator<<<std::char_traits<char>>(&std::cout, "vec1 : ");

  //for(size_t i = 0; i < vec1.size(); ++i){std::cout << vec1[i] << std::endl;}

  for ( i = 0; ; ++i )
  {
	v17 = i;
	if ( v17 >= std::vector<int>::size(&vec1) )
	  break;
	v15 = std::vector<int>::operator[](&vec1, i);
	v16 = std::ostream::operator<<(&std::cout, (unsigned int)*v15);
	std::operator<<<std::char_traits<char>>(v16, " ");
  }
  std::operator<<<std::char_traits<char>>(&std::cout, "\nvec2 : ");


  //for(it = vec2.begin(); it != vec2.end(); ++it) {std::cout << *it;}
  for ( it._M_current = std::vector<int>::begin(&vec2)._M_current;
		;
		__gnu_cxx::__normal_iterator<int *,std::vector<int>>::operator++(&it) )
  {
	__rhs._M_current = std::vector<int>::end(&vec2)._M_current;
	if ( !__gnu_cxx::operator!=<int *,std::vector<int>>(&it, &__rhs) )
	  break;
	v18 = __gnu_cxx::__normal_iterator<int *,std::vector<int>>::operator*(&it);
	v19 = std::ostream::operator<<(&std::cout, (unsigned int)*v18);
	std::operator<<<std::char_traits<char>>(v19, " ");
  }
  std::vector<int>::~vector(&vec2);
  std::vector<int>::~vector(&vec1);
  std::vector<int>::~vector(&vec);
  return 0;
}

*/

/*
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
	//初始化
	vector<int> vec;//声明，未初始化
	vector<int> vec1(2, 5);//2个5
	vector<int> vec2 = { 1, 2, 3, 4, 5 };//直接初始化

	//读取元素
	cout << vec2[1] << endl;
	cout << vec2.front() << endl;
	cout << vec2.back() << endl;
	//插入元素
	vec1.insert(vec1.begin(), 999);//para1-插入位置，要迭代器即指针，para2-插入内容
	vec2.push_back(888);//尾部插入
	vec2.pop_back();//删除尾部，类似于stack，所以有时候也可以把vector当stack用
	//删除元素
	vec2.erase(vec2.end() - 1);//参数也是迭代器类型，所以使用insert和erase时，最好用iterator来遍历
	//排序
	sort(vec2.begin(), vec2.end());//给出首位指针
	sort(vec2.begin(), vec2.begin() + 3);//甚至这样也可以，因为iterator本身就是类型指针

	//遍历--下标索引访问
	cout << "vec1 : ";

	//无符号数与有符号数作比较，会将有符号数转换为无符号数
	//int i = -1;					  //0xFFFFFFFF
	//unsigned int j = vec1.size();   //0x00000000
	//for (; i < j; ++i)              //不满足条件,不会执行输出
	for (int i = 0; i < vec1.size(); ++i)
	{
		cout << vec1[i] << " ";
	}

	//遍历--迭代器指针访问
	cout << "\nvec2 : ";
	vector<int>::iterator it;
	for (it = vec2.begin(); it != vec2.end(); ++it)
	{
		cout << *it << " ";
	}

	return 0;
}
*/


#include <stdio.h>
#include <vector>
#include <algorithm>
#include <functional>


struct vector_of_ints
{
	// MSVC names:
	int *Myfirst;
	int *Mylast;
	int *Myend;
	// GCC structure is the same, but names are: _M_start, _M_finish, _M_end_of_storage
};


void dump(struct vector_of_ints *in)
{
	printf("_Myfirst=%p, _Mylast=%p, _Myend=%p\n", in->Myfirst, in->Mylast, in->Myend);
	size_t size = (in->Mylast - in->Myfirst);
	size_t capacity = (in->Myend - in->Myfirst);
	printf("size=%d, capacity=%d\n", size, capacity);
	for (size_t i = 0; i < size; i++)
		printf("element %d: %d\n", i, in->Myfirst[i]);
};


int main()
{
	std::vector<int> c;

	int ivecSize = sizeof(c);

	dump((struct vector_of_ints*)(void*)&c);

	c.push_back(1);
	ivecSize = sizeof(c);

	dump((struct vector_of_ints*)(void*)&c);
	c.push_back(2);
	dump((struct vector_of_ints*)(void*)&c);
	c.push_back(3);
	dump((struct vector_of_ints*)(void*)&c);
	c.push_back(4);
	dump((struct vector_of_ints*)(void*)&c);
	c.reserve(6);
	dump((struct vector_of_ints*)(void*)&c);
	c.push_back(5);
	dump((struct vector_of_ints*)(void*)&c);
	c.push_back(6);
	dump((struct vector_of_ints*)(void*)&c);

	printf("%d\n", c.at(5)); // with bounds checking
	printf("%d\n", c[8]); // operator[], without bounds checking
};



