#include <stdio.h>
#include "graph_list.h"

int main()
{
	char* svexs = "A, B, C, D, E, C, A";
	char* sarcs = "AB, AD, B C , DC, DA, C E";
	printf("vertex expression:%s\n", svexs);
	printf("arc expression:%s\n", sarcs);
	//��������ͼ
	printf("\nCreateUG:\n\n");
	PAdjList pamUG = NULL;
	CreateUG(pamUG, svexs, sarcs);
	PrintAdjList(pamUG);
	//��������ͼ
	printf("\nCreateDG:\n\n");
	PAdjList pamDG = NULL;
	CreateDG(pamDG, svexs, sarcs);
	PrintAdjList(pamDG);

	char* svexs1 = "A, B, C, D, E, F, G, H, I";
	char* sarcs1 = "AB, AD, AE, BC, CF, DG, BE, EG, GH, HI";
	printf("vertex expression:%s\n", svexs1);
	printf("arc expression:%s\n", sarcs1);
	printf("CreateUG:\n");
	PAdjList pamUG1 = NULL;
	CreateUG(pamUG1, svexs1, sarcs1);
	PrintAdjList(pamUG1);
	//�ڽӱ�����ͼ������ȱ���������
	printf("\nDepthFirstSearch:");
	TraverseGraph(pamUG1);
	printf("\n");


	return 0;
}