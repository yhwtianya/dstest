#include <stdio.h>
#include <string>
#include "test.h"
extern my_stack* g_Mystack;

int main()
{
	//将字符串转化成二叉树
	string str = "A(B(C,D),E(F(,H( J,I )),G))";
	BiTree bt = NULL;
	printf("input:\n%s\n", str.c_str());
	CreateBiTree(bt, str.c_str());
	DspTree(bt);
	printf("\nInOrder:");
	InOrder(bt);

	ThreadHead th = NULL;
	CreateThreadTree(bt, th);
	printf("\n中序线索二叉树遍历:");
	DspThreadTree(th);
	printf("\n");
	return 0;
}