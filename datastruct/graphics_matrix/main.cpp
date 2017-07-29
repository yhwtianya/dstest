#include <stdio.h>
#include "graph_matrix.h"

int main()
{
	char* svexs = "A, B, C, D, E, C, A";
	char* sarcs = "AB, AD, B C , DC, DA, C E";
	printf("vertex expression:%s\n", svexs);
	printf("arc expression:%s\n", sarcs);
	//��������ͼ
	printf("\nCreateUG:\n\n");
	PAdjMatrix pamUG = NULL;
	CreateUG(pamUG, svexs, sarcs);
	PrintAdjMatrix(pamUG);
	//ѹ�����󴴽�����ͼ
	printf("\nCreateUG with Compression Matrix:\n\n");
	PAdjMatrixCmpr pamUGCmpr = NULL;
	CreateUG(pamUGCmpr, svexs, sarcs);
	PrintAdjMatrix(pamUGCmpr);

	printf("\nCreateDG:\n\n");
	PAdjMatrix pamDG = NULL;
	CreateDG(pamDG, svexs, sarcs);
	PrintAdjMatrix(pamDG);

	return 0;
}