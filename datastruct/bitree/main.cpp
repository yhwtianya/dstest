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

	//str = "A(B(,D),E(F(,H( J )),G))";
	//bt = NULL;
	//printf("input:\n%s\n", str.c_str());
	//CreateBiTree(bt, str.c_str());
	//DspTree(bt);

	//计算二叉树深度
	printf("\ndepth of tree is %d\n", BTNodeDepth(bt));

	//先序、中序、后序遍历
	printf("\nPreOrder:");
	PreOrder(bt);
	printf("\nInOrder:");
	InOrder(bt);
	printf("\nPostOrder:");
	PostOrder(bt);

	//层次遍历
	printf("\nLayerOrder:");
	LayerOrder(bt);

	//获取节点个数
	printf("\ncount of node:%d", NodeCount(bt));
	//获取叶子节点个数
	printf("\ncount of leaf:%d", LeafCount(bt));
	//复制树
	BiTree bt_new = NULL;
	CopyTree(bt, bt_new);
	printf("\n");
	DspTree(bt_new);
	//获取某节点层次
	char c = 'H';
	printf("\nlevel of note %c:%d", c, GetLevel(bt, c, 1));
	c = 'Y';
	printf("\nlevel of note %c:%d", c, GetLevel(bt, c, 1));

	//把叶子节点倒序输出路径
	g_Mystack->top = 0;
	printf("\n");
	LeafInverted(bt);


	return 0;
}