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

	return 0;
}