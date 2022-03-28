// test_filed_offset.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>

/*
 * 预处理 /P
 * 生成汇编文件 /FAs
 */

#define FIELD_OFFSET(type, field) (int)(&(((type*)0)->field))

#define CONTAINING_RECORD(address,type,field) ((type*)((char*)(address)-FIELD_OFFSET(type,field)))

typedef struct test
{
	int a;
	char b;
	short c;
}Test;

int main()
{
	int iOffset = 0;
	iOffset = (int)(&(((Test*)0)->b));

	Test test;
	test.a = 4;
	test.b = 'A';
	test.c = 1;

	printf("c Addr: %08x\r\n", &test.c);
	printf("test Addr: %08x\r\n", CONTAINING_RECORD(&test.c, Test, c));

	printf("%d\n", iOffset);

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
