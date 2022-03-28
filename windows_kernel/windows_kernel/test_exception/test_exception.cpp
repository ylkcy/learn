// test_exception.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>


EXCEPTION_DISPOSITION myhandler(struct _EXCEPTION_RECORD *pExceptionRecord, void* pEstablisherFrame, struct _CONTEXT *ContextRecord, void* pDispatcherContext)
{
	printf("%08x\r\n", pExceptionRecord->ExceptionAddress);

	return ExceptionContinueExecution;
}


/*
x86
typedef struct _EXCEPTION_REGISTRATION_RECORD 
{
	struct _EXCEPTION_REGISTRATION_RECORD *Next;    
	PEXCEPTION_ROUTINE Handler;
} EXCEPTION_REGISTRATION_RECORD;

typedef struct _NT_TIB 
{
	struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList; //最常使用的异常控制块链表
	PVOID StackBase;
	PVOID StackLimit;
	PVOID SubSystemTib;
	_ANONYMOUS_UNION union 
	{
		PVOID FiberData;
		ULONG Version;
	} DUMMYUNIONNAME;
	PVOID ArbitraryUserPointer;
	struct _NT_TIB *Self;
}NT_TIB, *PNT_TIB;

低地址
esp	|    0xcc     |
	|    0xcc     |
	|             |
	|             |
0xFC|     edi     |
	|     esi     |
	|     ebx     |
esp	|     ecx     |

-c	|ExceptionList| eax  next
-8  | 0x00AF2820  | handler

    | 0xFFFFFFFF  | 
ebp	|     ebp     | ebp = eax ^ security_cookie
	|   ret addr  |
	|     env     |
	|     argv    |
	|     argc    |
高地址

00AF2820 55                   push        ebp
00AF2821 8B EC                mov         ebp,esp
00AF2823 6A FF                push        0FFFFFFFFh
00AF2825 68 68 75 AF 00       push        0AF7568h
00AF282A 64 A1 00 00 00 00    mov         eax,dword ptr fs:[00000000h]
00AF2830 50                   push        eax


00AF2831 51                   push        ecx
00AF2832 81 EC FC 00 00 00    sub         esp,0FCh
00AF2838 53                   push        ebx
00AF2839 56                   push        esi
00AF283A 57                   push        edi
00AF283B 8D BD F4 FE FF FF    lea         edi,[ebp-10Ch]
00AF2841 B9 3F 00 00 00       mov         ecx,3Fh
00AF2846 B8 CC CC CC CC       mov         eax,0CCCCCCCCh
00AF284B F3 AB                rep stos    dword ptr es:[edi]
00AF284D A1 04 C0 AF 00       mov         eax,dword ptr [__security_cookie (0AFC004h)]
00AF2852 33 C5                xor         eax,ebp
00AF2854 89 45 EC             mov         dword ptr [ebp-14h],eax
00AF2857 50                   push        eax


00AF2858 8D 45 F4             lea         eax,[ebp-0Ch]                           //&EXCEPTION_REGISTRATION_RECORD        
00AF285B 64 A3 00 00 00 00    mov         dword ptr fs:[00000000h],eax            //ExceptionList->next = &_EXCEPTION_REGISTRATION_RECORD


00AF2861 89 65 F0             mov         dword ptr [ebp-10h],esp
00AF2864 B9 27 F0 AF 00       mov         ecx,offset _7397CD3B_test_exception@cpp (0AFF027h)


__ehhandler$_main:
00107568 8B 54 24 08          mov         edx,dword ptr [esp+8]
0010756C 8D 42 0C             lea         eax,[edx+0Ch]
0010756F 8B 8A F0 FE FF FF    mov         ecx,dword ptr [edx-110h]
00107575 33 C8                xor         ecx,eax
00107577 E8 E1 9C FF FF       call        @__security_check_cookie@4 (010125Dh)
0010757C 8B 4A F8             mov         ecx,dword ptr [edx-8]
0010757F 33 C8                xor         ecx,eax
00107581 E8 D7 9C FF FF       call        @__security_check_cookie@4 (010125Dh)
00107586 B8 94 B1 10 00       mov         eax,10B194h
0010758B E9 A6 9B FF FF       jmp         ___CxxFrameHandler3 (0101136h)
*/

struct test
{
	int a;
	int b;
};

//栈地址为高地址到低地址

int main()
{
	test t;

	printf("Addr a = %08x, Addr b = %08x\r\n", &t.a, &t.b);
	__asm
	{

		push myhandler
		push fs : 0

		mov fs:0 , esp

		add esp, 8
	}



    std::cout << "Hello World!\n";

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
