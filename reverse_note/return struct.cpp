#include<stdio.h>

/*
结构体作为返回值, 会产生临时变量，将临时变量的地址做为参数传入

00704E48 6A 01                push        1
00704E4A 8D 85 08 FF FF FF    lea         eax,[ebp-0F8h]
00704E50 50                   push        eax
00704E51 E8 47 C5 FF FF       call        get_some_values (070139Dh)

//局部变量
	struct s rt;
	rt.a = a + 1;
00704318 8B 45 0C             mov         eax,dword ptr [a]
0070431B 83 C0 01             add         eax,1
0070431E 89 45 F0             mov         dword ptr [rt],eax
	rt.b = a + 2;
00704321 8B 45 0C             mov         eax,dword ptr [a]
00704324 83 C0 02             add         eax,2
00704327 89 45 F4             mov         dword ptr [ebp-0Ch],eax
	rt.c = a + 3;
0070432A 8B 45 0C             mov         eax,dword ptr [a]
0070432D 83 C0 03             add         eax,3
00704330 89 45 F8             mov         dword ptr [ebp-8],eax

//临时变量的地址
	return rt;
00704333 8B 45 08             mov         eax,dword ptr [ebp+8]

00704336 8B 4D F0             mov         ecx,dword ptr [rt]
00704339 89 08                mov         dword ptr [eax],ecx

0070433B 8B 55 F4             mov         edx,dword ptr [ebp-0Ch]
0070433E 89 50 04             mov         dword ptr [eax+4],edx

00704341 8B 4D F8             mov         ecx,dword ptr [ebp-8]
00704344 89 48 08             mov         dword ptr [eax+8],ecx

//将临时变量参数地址作为返回值
00704347 8B 45 08             mov         eax,dword ptr [ebp+8]

// = 
00844E59 8B 08                mov         ecx,dword ptr [eax]
00844E5B 89 8D 1C FF FF FF    mov         dword ptr [ebp-0E4h],ecx
00844E61 8B 50 04             mov         edx,dword ptr [eax+4]
00844E64 89 95 20 FF FF FF    mov         dword ptr [ebp-0E0h],edx
00844E6A 8B 40 08             mov         eax,dword ptr [eax+8]
00844E6D 89 85 24 FF FF FF    mov         dword ptr [ebp-0DCh],eax

// 赋值局部变量
00844E73 8B 8D 1C FF FF FF    mov         ecx,dword ptr [ebp-0E4h]
00844E79 89 4D F0             mov         dword ptr [a],ecx
00844E7C 8B 95 20 FF FF FF    mov         edx,dword ptr [ebp-0E0h]
00844E82 89 55 F4             mov         dword ptr [ebp-0Ch],edx
00844E85 8B 85 24 FF FF FF    mov         eax,dword ptr [ebp-0DCh]
00844E8B 89 45 F8             mov         dword ptr [ebp-8],eax
*/
struct s
{
	int a;
	int b;
	int c;
};

struct s get_some_values(int a)
{
	struct s rt;
	rt.a = a + 1;
	rt.b = a + 2;
	rt.c = a + 3;
	return rt;
};

int main()
{
	struct s a;
	a = get_some_values(1);

	printf("%d %d %d\r\n", a.a, a.b, a.c);
	return 0;
};