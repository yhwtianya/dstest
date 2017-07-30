#include "graph_list.h"
#include "queue_ds.h"
using namespace DS;

void InitAdjList(PAdjList& pam, GraphKind kind)
{
	if (NULL != pam)
	{
		printf("the AdjList is not NULL\n");
		return;
	}

	pam = (PAdjList)malloc(sizeof(AdjList));
	pam->arcnum = pam->vexnum = 0;
	pam->kind = kind;
	for (size_t i = 0; i < GRAPH_MAX_VERTEX_NUM; i++)
	{
		pam->vertex[i].data = '\0';
		pam->vertex[i].firstarc = NULL;
	}
}

void FreeAdjList(PAdjList& pam)
{
	free(pam);
	pam = NULL;
}

int FindVertex(PAdjList& pam, char vex)
{
	for (size_t i = 0; i < pam->vexnum; i++)
	{
		if (pam->vertex[i].data == vex)
		{
			return i;
		}
	}

	return -1;
}

void CreateDG(PAdjList& al, char* sVertexs, char* sArcs)
{
	if (NULL == al)
	{
		InitAdjList(al, DG);
	}

	int count = 0;
	for (size_t i = 0; i < strlen(sVertexs); i++)
	{
		if (sVertexs[i] != ',' && sVertexs[i] != ' ')
		{
			if (-1 == FindVertex(al, sVertexs[i]))
			{
				al->vertex[count].data = sVertexs[i];
				al->vertex[count].firstarc = NULL;
				al->vexnum = ++count;
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
				int row = FindVertex(al, firstVertex);
				int col = FindVertex(al, secondVertex);
				PushArc(al, row, col);
				bFindFirstVertex = false;
			}
		}
	}
}

void CreateUG(PAdjList& pam, char* sVertexs, char* sArcs)
{
	if (NULL == pam)
	{
		InitAdjList(pam, UG);
	}
	return CreateDG(pam, sVertexs, sArcs);
}

void PrintAdjList(PAdjList& pam)
{
	for (size_t i = 0; i < pam->vexnum; i++)
	{
		if (0 == i)
			printf("\t");
		printf("%c\t", pam->vertex[i]);
	}
	printf("\n\n");
	for (size_t i = 0; i < pam->vexnum; i++)
	{
		printf("  %c\t", pam->vertex[i]);
		for (size_t j = 0; j < pam->vexnum; j++)
		{
			PArcNode pan = GetArc(pam, i, j);
			if (pan)
			{
				printf("%d %c%c\t", 1, pam->vertex[i].data, pam->vertex[j].data);
			}
			else
			{
				printf("%d\t", 0);
			}
		}
		printf("\n\n");
	}
}

PArcNode InitArcNode(int adjvex)
{
	PArcNode arc = (PArcNode)malloc(sizeof(ArcNode));
	arc->adjvex = adjvex;
	arc->info = NULL;
	arc->nextarc = NULL;
	return arc;
}

bool HasArc(PAdjList& al, int adjvex_src, int adjvex_dst)
{
	return NULL != GetArc(al, adjvex_src, adjvex_dst);
}

PArcNode GetArc(PAdjList& al, int adjvex_src, int adjvex_dst)
{
	PArcNode pan = al->vertex[adjvex_src].firstarc;
	while (NULL != pan)
	{
		if (pan->adjvex == adjvex_dst)
		{
			return pan;
		}
		pan = pan->nextarc;
	}

	return NULL;
}

bool PushArcInno(PAdjList& al, int adjvex_src, int adjvex_dst)
{
	PArcNode pan = al->vertex[adjvex_src].firstarc;
	if (NULL == pan)
	{
		al->vertex[adjvex_src].firstarc = InitArcNode(adjvex_dst);
		return true;
	}
	while (NULL != pan)
	{
		if (pan->adjvex == adjvex_dst)
		{
			break;
		}
		if (pan->nextarc == NULL)
		{
			pan->nextarc = InitArcNode(adjvex_dst);
			return true;
		}
		pan = pan->nextarc;
	}

	return false;
}

void PushArc(PAdjList& al, int adjvex_src, int adjvex_dst)
{
	if (PushArcInno(al, adjvex_src, adjvex_dst))
	{
		(al->arcnum)++;
	}

	if (UG == al->kind)
		PushArcInno(al, adjvex_dst, adjvex_src);
}

PArcNode GetNextArc(PArcNode& pan)
{
	return pan->nextarc;
}

PArcNode GetFirstArc(PAdjList& pal, int row)
{
	return pal->vertex[row].firstarc;
}

void Visit(PAdjList& pam, int index)
{
	return;
	printf("%c ", pam->vertex[index].data);
}

void VisitArc(PAdjList& pal, int src, int dst)
{
	printf("%c --> %c, ", pal->vertex[src].data, pal->vertex[dst].data);
}

void DepthFirstSearch(PAdjList& pal, bool visited[], int index_src)
{
	visited[index_src] = true;
	Visit(pal, index_src);
	PArcNode pan_dst = GetFirstArc(pal, index_src);
	while(NULL!=pan_dst)
	{
		if (visited[pan_dst->adjvex] == 0)
		{
			VisitArc(pal, index_src, pan_dst->adjvex);
			DepthFirstSearch(pal, visited, pan_dst->adjvex);
		}
		pan_dst = GetNextArc(pan_dst);
	}
}

/*	采用邻接表方式存储弧节点，弧链表元素的不同先后顺序会影响生成树元素的顺序。
*	采用邻接矩阵方式存储弧节点，因为按矩阵下标找相邻的弧，不存在这个问题	*/
void TraverseGraph(PAdjList& pal)
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
			DepthFirstSearch(pal, visited, i);
		}
	}
}

/*	递归支撑深度优先，队列支撑广度优先。切记广度优先里没有递归，
*	外层循环进行依次出队，内层循环进行层所有元素入队。		*/
void BreadFirstSearch(PAdjList& pal, bool visited[], int index)
{
	PQueue q = NULL;
	InitQueue(q);
	int index_src = index;
	EnQueue(q, &index_src);
	Visit(pal, index_src);
	visited[index_src] = true;
	int *pindex = NULL;
	while (NULL != (pindex=(int*)DelQueue(q)))
	{
		index_src = *pindex;
		PArcNode pan = pal->vertex[index_src].firstarc;
		while (NULL != pan)
		{
			if (visited[pan->adjvex]==false)
			{
				VisitArc(pal, index_src, pan->adjvex);
				visited[pan->adjvex] = true;
				EnQueue(q, &(pan->adjvex));
			}
			pan = pan->nextarc;
		}
	}
}

void TraverseGraphBFS(PAdjList& pal)
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

