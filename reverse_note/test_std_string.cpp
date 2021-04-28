// test_std_string.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <string>
#include <stdio.h>

struct std_debug_string
{
	std::string** self;
	union
	{
		char buf[16];
		char* ptr;
	} u;
	size_t size; 
	size_t capacity; 
};

struct std_release_string
{
	union
	{
		char buf[16];
		char* ptr;
	} u;
	size_t size; 
	size_t capacity;
};

void dump_std_debug_string(std::string s)
{
	struct std_debug_string *p = (struct std_debug_string*)&s;
	printf("[%s] size:%d capacity:%d\n", p->size > 16 - 1 ? p->u.ptr : p->u.buf, p->size, p->capacity);

	struct std_debug_string *p2 = (struct std_debug_string*)(*(p->self));
	printf("[%s] size:%d capacity:%d\n", p2->size > 16 - 1 ? p2->u.ptr : p2->u.buf, p2->size, p2->capacity);
};

void dump_std_release_string(std::string s)
{
	struct std_release_string *p = (struct std_release_string*)&s;
	printf("[%s] size:%d capacity:%d\n", p->size > 16 - 1 ? p->u.ptr : p->u.buf, p->size, p->capacity);
};

int main()
{
#ifdef DEBUG
	printf("size: %d\n", sizeof(struct std_debug_string));

	std::string s1 = "shortstring1234";
	std::string s2 = "string longer that 16 bytes";
	dump_std_debug_string(s1);
	dump_std_debug_string(s2);
#endif
	printf("size: %d\n", sizeof(struct std_release_string));

	std::string s1 = "shortstring1234";
	std::string s2 = "string longer that 16 bytes";
	dump_std_release_string(s1);
	dump_std_release_string(s2);

	return 0;
};

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
