// test_strncpy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#pragma warning(disable:4996)
#include <iostream>
#include <stdio.h>
#include <string.h>

#ifdef TEST_VIRTUAL_FUN

typedef void(*FUN)(void*);

class A
{
public:
	A() { m_a = 0; }
	A(int a) { m_a = a; }
	~A() {}
public:
	virtual void fun1() { printf("A\n"); }
	virtual void fun2() { printf("B\n"); }
	virtual void fun3() { printf("C\n"); }

	void show() { printf("%d\n", m_a); }
private:
	int m_a;
};

int main(int argc, char* argv[])
{
	A a;
	A* pA = &a;
	pA->fun2();
	
	printf("虚函数表的地址为：%p\n", *(int*)pA);
	FUN fun2 = (FUN)*((int*)*(int*)pA + 1);
	fun2(pA);

	*((int*)pA + 1) = 1;

	pA->show();

	getchar();
	return 0;
}
#endif

#ifdef TEST_ARR_AND_LOCAL_DIFF
/*
局部变量和数组的区别
	int n1 = 1;
001B17D8  mov         dword ptr [ebp-8],1
	int n2 = 2;
001B17DF  mov         dword ptr [ebp-14h],2
	int n3 = 3;
001B17E6  mov         dword ptr [ebp-20h],3
	int n4 = 4;
001B17ED  mov         dword ptr [ebp-2Ch],4
	int n5 = 5;
001B17F4  mov         dword ptr [ebp-38h],5
	int nArr[5] = { 1,2,3,4,5 };
001B17FB  mov         dword ptr [ebp-54h],1
001B1802  mov         dword ptr [ebp-50h],2
001B1809  mov         dword ptr [ebp-4Ch],3
001B1810  mov         dword ptr [ebp-48h],4
001B1817  mov         dword ptr [ebp-44h],5
连续定义相同类型的局部变量和数组赋值顺序是相反的
*/
int main(int argc, char* argv[])
{
	int n1 = 1;
	int n2 = 2;
	int n3 = 3;
	int n4 = 4;
	int n5 = 5;
	int nArr[5] = { 1,2,3,4,5 };

	getchar();
	return 0;
}

#endif

#ifdef TEST_STRING_ARR
/*
与数组类似 将.data数据区的地址给连续的栈空间，相当于给数组赋初值
DEBUG版本：
00B117D8  mov         eax,dword ptr ds:[00B17BCCh]
00B117DD  mov         dword ptr [ebp-10h],eax
00B117E0  mov         ecx,dword ptr ds:[00B17BD0h]
00B117E6  mov         dword ptr [ebp-0Ch],ecx
00B117E9  mov         edx,dword ptr ds:[00B17BD4h]
00B117EF  mov         dword ptr [ebp-8],edx
*/
int main(int argc, char* argv[])
{
	char szArr[] = "hello world";
	printf("%s\n", szArr);
	getchar();
	return 0;
}

#endif

#ifdef TEST_POINTER_AND_ARRAY
/*
下标寻址：
002542E4  movsx       edx,byte ptr [ebp+ecx-14h]

*p方式寻址，多了一次间接寻址的过程
002542FD  mov         eax,dword ptr [ebp-20h]
00254300  movsx       ecx,byte ptr [eax]
*/
int main(int argc, char* argv[])
{
	char szArr[] = "hello world";
	printf("%d\n", szArr[0]);

	char* p = szArr;
	printf("%d\n", *p);

	getchar();
	return 0;
}
#endif

#ifdef TEST_MULTI_ARRAY
/*
二维数组从赋值上与一位数组无区别
	int arr[][3] = { {1,2,3},
006C4DE8  mov         dword ptr [ebp-34h],1
006C4DEF  mov         dword ptr [ebp-30h],2
006C4DF6  mov         dword ptr [ebp-2Ch],3
					   {4,5,6},
006C4DFD  mov         dword ptr [ebp-28h],4
006C4E04  mov         dword ptr [ebp-24h],5
006C4E0B  mov         dword ptr [ebp-20h],6
					   {7,8,9},
006C4E12  mov         dword ptr [ebp-1Ch],7
006C4E19  mov         dword ptr [ebp-18h],8
006C4E20  mov         dword ptr [ebp-14h],9
					   {10,11,12}};
006C4E27  mov         dword ptr [ebp-10h],0Ah
006C4E2E  mov         dword ptr [ebp-0Ch],0Bh
006C4E35  mov         dword ptr [ebp-8],0Ch
	printf("%d\n", arr[2][1]);
006C4E3C  mov         eax,0Ch   int[3] = 0Ch
006C4E41  shl         eax,1     eax = 2
006C4E43  lea         ecx,[ebp+eax-34h]
006C4E47  mov         edx,4
006C4E4C  shl         edx,0
006C4E4F  mov         eax,dword ptr [ecx+edx]

arr + i * sizeof(int[3]) + j * sizeof(int)
*/
int main(int argc, char* argv[])
{
	int arr[][3] = { {1,2,3},
					   {4,5,6},
					   {7,8,9},
					   {10,11,12}};
	printf("%d\n", arr[2][1]);

	getchar();
	return 0;
}
#endif

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
