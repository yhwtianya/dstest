#include "graph_matrix.h"

void InitAdjMatrix(PAdjMatrix& pam, GraphKind kind)
{
	if (NULL != pam)
	{
		printf("the AdjMatrix is not NULL\n");
		return;
	}

	pam = (PAdjMatrix)malloc(sizeof(AdjMatrix));
	pam->arcnum = pam->vexnum = 0;
	pam->kind = kind;
	memset(pam->vexs, 0, sizeof(pam->vexs));
	for (size_t i = 0; i < GRAPH_MAX_VERTEX_NUM; i++)
	{
		for (size_t j = 0; j < GRAPH_MAX_VERTEX_NUM; j++)
		{
			pam->arcs[i][j].AdjType = 0;
		}
	}
}

void InitAdjMatrix(PAdjMatrixCmpr& pam, GraphKind kind)
{
	if (NULL != pam)
	{
		printf("the AdjMatrix is not NULL\n");
		return;
	}

	pam = (PAdjMatrixCmpr)malloc(sizeof(AdjMatrixCmpr));
	pam->arcnum = pam->vexnum = 0;
	pam->kind = kind;
	memset(pam->vexs, 0, sizeof(pam->vexs));
	for (size_t i = 0; i < CMPR_MATRIX_ARC_MAX_NUM; i++)
	{
		pam->arcs[i].AdjType = 0;
	}
}

void FreeAdjMatrix(PAdjMatrix& pam)
{
	free(pam);
	pam = NULL;
}

void FreeAdjMatrix(PAdjMatrixCmpr& pam)
{
	free(pam);
	pam = NULL;
}

int FindVertex(PAdjMatrix& pam, char vex)
{
	for (size_t i = 0; i < pam->vexnum; i++)
	{
		if (pam->vexs[i] == vex)
		{
			return i;
		}
	}

	return -1;
}

int FindVertex(PAdjMatrixCmpr& pam, char vex)
{
	for (size_t i = 0; i < pam->vexnum; i++)
	{
		if (pam->vexs[i] == vex)
		{
			return i;
		}
	}

	return -1;
}

int GetArcIndex(int row, int col)
{
	//(0,0)����ת������Խλ������ֱ��ת��Ĭ�����������λ�ñ�����Ĭ��ֵ
	//���Խλ���ܻ�Ҳ���ܲ����������������������˵�λ�ڴ��ַ���ڱ���������
	if (row == col)
		return CMPR_MATRIX_ARC_MAX_NUM-1;

	if (row<col)
	{
		int tmp = row;
		row = col;
		col = tmp;
	}

	++row;
	return ((row*(row - 1) / 2 + col) - 1);
}

void CreateDG(PAdjMatrix& pam, char* sVertexs, char* sArcs)
{
	if (NULL == pam)
	{
		InitAdjMatrix(pam, DG);
	}

	int count = 0;
	for (size_t i = 0; i < strlen(sVertexs); i++)
	{
		if (sVertexs[i] != ',' && sVertexs[i] != ' ')
		{
			if (-1 == FindVertex(pam, sVertexs[i]))
			{
				pam->vexs[count++] = sVertexs[i];
				pam->vexnum = count;
			}
		}
	}

	bool bFindFirstVertex = false;
	char firstVertex, secondVertex;
	for (size_t i = 0; i < strlen(sArcs); i++)
	{
		if (sArcs[i] != ',' && sArcs[i] != ' ')
		{
			if (!bFindFirstVertex)
			{
				firstVertex = sArcs[i];
				bFindFirstVertex = true;
			}
			else
			{
				secondVertex = sArcs[i];
				int row = FindVertex(pam, firstVertex);
				int col = FindVertex(pam, secondVertex);
				if (pam->arcs[row][col].AdjType==0)
				{
					pam->arcs[row][col].AdjType = 1;
					if (UG == pam->kind)
						pam->arcs[col][row].AdjType = 1;
					(pam->arcnum)++;
				}
				bFindFirstVertex = false;
			}
		}
	}
}

void CreateUG(PAdjMatrix& pam, char* sVertexs, char* sArcs)
{
	if (NULL == pam)
	{
		InitAdjMatrix(pam, UG);
	}
	return CreateDG(pam, sVertexs, sArcs);
}

void CreateUG(PAdjMatrixCmpr& pam, char* sVertexs, char* sArcs)
{
	if (NULL == pam)
	{
		InitAdjMatrix(pam, DG);
	}

	int count = 0;
	for (size_t i = 0; i < strlen(sVertexs); i++)
	{
		if (sVertexs[i] != ',' && sVertexs[i] != ' ')
		{
			if (-1 == FindVertex(pam, sVertexs[i]))
			{
				pam->vexs[count++] = sVertexs[i];
				pam->vexnum = count;
			}
		}
	}

	bool bFindFirstVertex = false;
	char firstVertex, secondVertex;
	for (size_t i = 0; i < strlen(sArcs); i++)
	{
		if (sArcs[i] != ',' && sArcs[i] != ' ')
		{
			if (!bFindFirstVertex)
			{
				firstVertex = sArcs[i];
				bFindFirstVertex = true;
			}
			else
			{
				secondVertex = sArcs[i];
				int row = FindVertex(pam, firstVertex);
				int col = FindVertex(pam, secondVertex);
				if (row==col)
				{
					//����ͼ�����������������Ļ�
					continue;
				}
				int k = GetArcIndex(row, col);
				if (GetArc(pam, row, col).AdjType==0)
				{
					pam->arcs[k].AdjType = 1;
					(pam->arcnum)++;
				}
				bFindFirstVertex = false;
			}
		}
	}
}

void PrintAdjMatrix(PAdjMatrix& pam)
{
	for (size_t i = 0; i < pam->vexnum; i++)
	{
		if (0 == i)
			printf("\t");
		printf("%c\t", pam->vexs[i]);
	}
	printf("\n\n");
	for (size_t i = 0; i < pam->vexnum; i++)
	{
		printf("  %c\t", pam->vexs[i]);
		for (size_t j = 0; j < pam->vexnum; j++)
		{
			if (pam->arcs[i][j].AdjType)
			{
				printf("%d %c%c\t", pam->arcs[i][j].AdjType, pam->vexs[i], pam->vexs[j]);
			}
			else
			{
				printf("%d\t", pam->arcs[i][j].AdjType);
			}
		}
		printf("\n\n");
	}
}

void PrintAdjMatrix(PAdjMatrixCmpr& pam)
{
	for (size_t i = 0; i < pam->vexnum; i++)
	{
		if (0 == i)
			printf("\t");
		printf("%c\t", pam->vexs[i]);
	}
	printf("\n\n");
	for (size_t i = 0; i < pam->vexnum; i++)
	{
		printf("  %c\t", pam->vexs[i]);
		for (size_t j = 0; j < pam->vexnum; j++)
		{
			if (GetArc(pam, i, j).AdjType)
			{
				printf("%d %c%c\t", GetArc(pam, i, j).AdjType, pam->vexs[i], pam->vexs[j]);
			}
			else
			{
				printf("%d\t", GetArc(pam, i, j).AdjType);
			}
		}
		printf("\n\n");
	}
}

int GetNextArc(PAdjMatrix& pam, int row, int startcol)
{
	for (size_t i = startcol; i < GRAPH_MAX_VERTEX_NUM; i++)
	{
		//PAdjMatrixCmpr��PAdjMatrix��ȣ������ﲻͬ
		if (pam->arcs[row][i].AdjType == 1)
			return i;
	}

	return -1;
}

int GetFirstArc(PAdjMatrix& pam, int row)
{
	return GetNextArc(pam, row, 0);
}

void Visit(PAdjMatrix& pam, int index)
{
	return;
	printf("%c ", pam->vexs[index]);
}

void VisitArc(PAdjMatrix& pam, int src, int dst)
{
	printf("%c --> %c, ", pam->vexs[src], pam->vexs[dst]);
}

void DepthFirstSearch(PAdjMatrix& pam, bool visited[], int index_src)
{
	visited[index_src] = true;
	Visit(pam, index_src);
	int index_dst = GetFirstArc(pam, index_src);
	while (-1 != index_dst)
	{
		if (visited[index_dst] == 0)
		{
			VisitArc(pam, index_src, index_dst);
			DepthFirstSearch(pam, visited, index_dst);
		}
		index_dst = GetNextArc(pam, index_src, index_dst + 1);
	}
}

/*	�����ڽӱ���ʽ�洢���ڵ㣬������Ԫ�صĲ�ͬ�Ⱥ�˳���Ӱ��������Ԫ�ص�˳��
*	�����ڽӾ���ʽ�洢���ڵ㣬��Ϊ�������±������ڵĻ����������������	*/
void TraverseGraph(PAdjMatrix& pam)
{
	bool *visited = (bool*)malloc(sizeof(bool)*pam->vexnum);
	for (size_t i = 0; i < pam->vexnum; i++)
	{
		visited[i] = false;
	}

	for (size_t i = 0; i < pam->vexnum; i++)
	{
		if (visited[i] != true)
		{
			DepthFirstSearch(pam, visited, i);
		}
	}
}

ArcNode InitArcNode(int adjtype)
{
	ArcNode an;
	an.AdjType = adjtype;
	return an;
}

ArcNode GetArc(PAdjMatrixCmpr& pam, int row, int col, ArcNode def/*=InitArcNode(0)*/)
{
	return pam->arcs[GetArcIndex(row, col)];
}

int GetNextArc(PAdjMatrixCmpr& pam, int row, int startcol)
{
	for (size_t i = startcol; i < GRAPH_MAX_VERTEX_NUM; i++)
	{
		//PAdjMatrixCmpr��PAdjMatrix��ȣ������ﲻͬ
		if (GetArc(pam, row, i).AdjType == 1)
			return i;
	}

	return -1;
}

int GetFirstArc(PAdjMatrixCmpr& pam, int row)
{
	return GetNextArc(pam, row, 0);
}

void Visit(PAdjMatrixCmpr& pam, int index)
{
	return;
	printf("%c ", pam->vexs[index]);
}

void VisitArc(PAdjMatrixCmpr& pam, int src, int dst)
{
	printf("%c --> %c, ", pam->vexs[src], pam->vexs[dst]);
}

void DepthFirstSearch(PAdjMatrixCmpr& pam, bool visited[], int index_src)
{
	visited[index_src] = true;
	Visit(pam, index_src);
	int index_dst = GetFirstArc(pam, index_src);
	while (-1!= index_dst)
	{
		if (visited[index_dst] == 0)
		{
			VisitArc(pam, index_src, index_dst);
			DepthFirstSearch(pam, visited, index_dst);
		}
		index_dst = GetNextArc(pam, index_src, index_dst + 1);
	}
}

void TraverseGraph(PAdjMatrixCmpr& pam)
{
	bool *visited = (bool*)malloc(sizeof(bool)*pam->vexnum);
	for (size_t i = 0; i < pam->vexnum; i++)
	{
		visited[i] = false;
	}

	for (size_t i = 0; i < pam->vexnum; i++)
	{
		if (visited[i] != true)
		{
			DepthFirstSearch(pam, visited, i);
		}
	}
}
