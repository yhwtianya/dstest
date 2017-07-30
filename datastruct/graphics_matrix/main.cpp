#include <stdio.h>
#include "graph_matrix.h"

int main()
{
	char* svexs = "A, B, C, D, E, C, A";
	char* sarcs = "AB, AD, B C , DC, DA, C E";
	printf("vertex expression:%s\n", svexs);
	printf("arc expression:%s\n", sarcs);
	//创建无向图
	printf("CreateUG:\n");
	PAdjMatrix pamUG = NULL;
	CreateUG(pamUG, svexs, sarcs);
	PrintAdjMatrix(pamUG);
	//压缩矩阵创建无向图
	printf("\nCreateUG with Compression Matrix:\n");
	PAdjMatrixCmpr pamUGCmpr = NULL;
	CreateUG(pamUGCmpr, svexs, sarcs);
	PrintAdjMatrix(pamUGCmpr);

	printf("\nCreateDG:\n");
	PAdjMatrix pamDG = NULL;
	CreateDG(pamDG, svexs, sarcs);
	PrintAdjMatrix(pamDG);

	char* svexs1 = "A, B, C, D, E, F, G, H, I";
	char* sarcs1 = "AB, AD, AE, BE, BC, CF, EG, DG, GH, HI";
	printf("vertex expression:%s\n", svexs1);
	printf("arc expression:%s\n", sarcs1);
	printf("CreateUG:\n");
	PAdjMatrix pamUG1 = NULL;
	CreateUG(pamUG1, svexs1, sarcs1);
	PrintAdjMatrix(pamUG1);
	//邻接矩阵无向图深度优先遍历生成树
	printf("\nDepthFirstSearch:");
	TraverseGraph(pamUG1);
	printf("\n");

	printf("\nCreateUG with Compression Matrix:\n");
	PAdjMatrixCmpr pamUGCmpr1 = NULL;
	CreateUG(pamUGCmpr1, svexs1, sarcs1);
	PrintAdjMatrix(pamUGCmpr1);
	//压缩矩阵无向图深度优先遍历生成树
	printf("\nDepthFirstSearch:");
	TraverseGraph(pamUGCmpr1);
	printf("\n");


	return 0;
}