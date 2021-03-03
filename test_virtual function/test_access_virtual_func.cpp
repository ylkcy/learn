#include <iostream>
#include <stdio.h>
#include <string.h>

typedef void (*FUN)(void*);

class A
{
public:
	A() {m_a = 0;}
	A(int a) {m_a = a;}
	~A() {}
public:
	virtual void fun1() {printf("A\n");}
	virtual void fun2() {printf("B\n");}
	virtual void fun3() {printf("C\n");}
private:
	int m_a;
};

int main(int argc, char* argv[])
{
	A a;
	A* pA = &a;
	
	printf("虚函数表的地址为：%p\n", *(int*)pA);
	FUN fun2 = (FUN)*((int*)*(int*)pA + 1);
	fun2(pA);

	getchar();			
	return 0;
}
