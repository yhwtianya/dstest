#include "graph_list.h"

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
