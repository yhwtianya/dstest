#include "graph_matrix.h"
#include "queue_ds.h"
using namespace DS;

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
			pam->arcs[i][j].weight = 0;
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
		pam->arcs[i].weight = 0;
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
	//(0,0)引起转换索引越位，这里直接转到默认索引，这个位置保存了默认值
	//这个越位可能会也可能不会引起程序崩溃，它读到了低位内存地址所在变量的内容
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
				if (pam->arcs[row][col].weight==0)
				{
					pam->arcs[row][col].weight = 1;
					if (UG == pam->kind)
						pam->arcs[col][row].weight = 1;
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
					//无向图不存在自身到自身的弧
					continue;
				}
				int k = GetArcIndex(row, col);
				if (GetArc(pam, row, col).weight==0)
				{
					pam->arcs[k].weight = 1;
					(pam->arcnum)++;
				}
				bFindFirstVertex = false;
			}
		}
	}
}

void CreateDN(PAdjMatrix& pam, char* sVertexs, char* sArcs)
{
	if (NULL == pam)
	{
		InitAdjMatrix(pam, DN);
	}
	return CreateUN(pam, sVertexs, sArcs);
}

void CreateUN(PAdjMatrix& pam, char* sVertexs, char* sArcs)
{
	if (NULL == pam)
	{
		InitAdjMatrix(pam, UN);
	}

	int count = 0;
	for (size_t i = 0; i < strlen(sVertexs); i++)
	{
		if (sVertexs[i] != ',' && sVertexs[i] != ' ')
		{
			if (-1 == FindVertex(pam, sVertexs[i]))
			{
				pam->vexs[count] = sVertexs[i];
				pam->vexnum = ++count;
			}
		}
	}

	bool bFindFirstVertex = false;
	bool bFindSecondVertex = false;
	char firstVertex, secondVertex;
	int weight = 0;
	for (size_t i = 0; i < strlen(sArcs); ++i)
	{
		if (sArcs[i] != ',' && sArcs[i] != ' ')
		{
			if (!bFindFirstVertex)
			{
				firstVertex = sArcs[i];
				bFindFirstVertex = true;
			}
			else if (bFindFirstVertex && !bFindSecondVertex)
			{
				secondVertex = sArcs[i];
				bFindSecondVertex = true;
			}
			else
			{
				weight = atoi(&(sArcs[i]));
				int row = FindVertex(pam, firstVertex);
				int col = FindVertex(pam, secondVertex);
				if (row == col)
				{
					//无向图不存在自身到自身的弧
					continue;
				}
				if (pam->arcs[row][col].weight == 0)
				{
					pam->arcs[row][col].weight = weight;
					(pam->arcnum)++;
				}
				bFindFirstVertex = false;
				bFindSecondVertex = false;
				while (++i < strlen(sArcs) && sArcs[i] != ',');
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
			if (pam->arcs[i][j].weight)
			{
				printf("%c%c%d\t", pam->vexs[i], pam->vexs[j], pam->arcs[i][j].weight);
			}
			else
			{
				//printf("%d\t", pam->arcs[i][j].AdjType);
				printf("%c\t", '-');
			}
		}
		printf("\n");
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
			if (GetArc(pam, i, j).weight)
			{
				printf("%c%c%d\t", pam->vexs[i], pam->vexs[j], GetArc(pam, i, j).weight);
			}
			else
			{
				//printf("%d\t", GetArc(pam, i, j).AdjType);
				printf("%c\t", '-');

			}
		}
		printf("\n");
	}
}

int GetNextArc(PAdjMatrix& pam, int row, int startcol)
{
	for (size_t i = startcol; i < GRAPH_MAX_VERTEX_NUM; i++)
	{
		//PAdjMatrixCmpr和PAdjMatrix相比，仅这里不同
		if (pam->arcs[row][i].weight == 1)
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

/*	采用邻接表方式存储弧节点，弧链表元素的不同先后顺序会影响生成树元素的顺序。
*	采用邻接矩阵方式存储弧节点，因为按矩阵下标找相邻的弧，不存在这个问题	*/
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
	an.weight = adjtype;
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
		//PAdjMatrixCmpr和PAdjMatrix相比，仅这里不同
		if (GetArc(pam, row, i).weight == 1)
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

/*	递归支撑深度优先，队列支撑广度优先。切记广度优先里没有递归，
*	外层循环进行依次出队，内层循环进行层所有元素入队。		*/
void BreadFirstSearch(PAdjMatrix& pam, bool visited[], int index)
{
	PQueue q = NULL;
	InitQueue(q);
	int index_src = index;
	EnQueue(q, &index_src);
	Visit(pam, index_src);
	visited[index_src] = true;
	int *pindex = NULL;
	while (NULL != (pindex = (int*)DelQueue(q)))
	{
		index_src = *pindex;
		int* pindex_dst = (int*)malloc(sizeof(int));
		*pindex_dst = GetFirstArc(pam, index_src);
		int index_dst = *pindex_dst;

		while (index_dst != -1)
		{
			if (visited[index_dst] == false)
			{
				VisitArc(pam, index_src, index_dst);
				visited[index_dst] = true;
				EnQueue(q, pindex_dst);
			}

			pindex_dst = (int*)malloc(sizeof(int));
			*pindex_dst = GetNextArc(pam, index_src, index_dst + 1);
			index_dst = *pindex_dst;
		}
	}
}

void TraverseGraphBFS(PAdjMatrix& pal)
{
	bool *visited = (bool*)malloc(sizeof(bool)*pal->vexnum);
	for (size_t i = 0; i < pal->vexnum; i++)
	{
		visited[i] = false;
	}

	for (size_t i = 0; i < pal->vexnum; i++)
	{
		if (visited[i] != true)
		{
			BreadFirstSearch(pal, visited, i);
		}
	}
}

int GetArcWeight(PAdjMatrix& pam, int row, int col, int def /*= 0*/)
{
	if (pam->arcs[row][col].weight == 0)
	{
		if (row == col)
			return 0;
		return def;
	}

	return pam->arcs[row][col].weight;
}

