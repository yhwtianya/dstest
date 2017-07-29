#include <stdio.h>
#include "graph_list.h"

int main()
{
	char* svexs = "A, B, C, D, E, C, A";
	char* sarcs = "AB, AD, B C , DC, DA, C E";
	printf("vertex expression:%s\n", svexs);
	printf("arc expression:%s\n", sarcs);
	//创建无向图
	printf("\nCreateUG:\n\n");
	PAdjList pamUG = NULL;
	CreateUG(pamUG, svexs, sarcs);
	PrintAdjList(pamUG);
	//创建有向图
	printf("\nCreateDG:\n\n");
	PAdjList pamDG = NULL;
	CreateDG(pamDG, svexs, sarcs);
	PrintAdjList(pamDG);

	return 0;
}