#include <stdio.h>
#include <string>
#include "test.h"
extern my_stack* g_Mystack;

int main()
{
	//���ַ���ת���ɶ�����
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

	//������������
	printf("\ndepth of tree is %d\n", BTNodeDepth(bt));

	//�������򡢺������
	printf("\nPreOrder:");
	PreOrder(bt);
	printf("\nInOrder:");
	InOrder(bt);
	printf("\nPostOrder:");
	PostOrder(bt);

	//��α���
	printf("\nLayerOrder:");
	LayerOrder(bt);

	//��ȡ�ڵ����
	printf("\ncount of node:%d", NodeCount(bt));
	//��ȡҶ�ӽڵ����
	printf("\ncount of leaf:%d", LeafCount(bt));
	//������
	BiTree bt_new = NULL;
	CopyTree(bt, bt_new);
	printf("\n");
	DspTree(bt_new);
	//��ȡĳ�ڵ���
	char c = 'H';
	printf("\nlevel of note %c:%d", c, GetLevel(bt, c, 1));
	c = 'Y';
	printf("\nlevel of note %c:%d", c, GetLevel(bt, c, 1));

	//��Ҷ�ӽڵ㵹�����·��
	g_Mystack->top = 0;
	printf("\n");
	LeafInverted(bt);


	return 0;
}