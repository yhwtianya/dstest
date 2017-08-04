#include "graph_DAG.h"
#include "stack_ds.h"
using namespace DS;

int TopoSort(PAdjList& pal, PQueue& pq)
{
	InitQueue(pq);
	PStack ps = NULL;
	InitStack(ps);
	for (size_t i = 0; i < pal->vexnum; i++)
	{
		//为了防止重复遍历入度为0的顶点，这里先选出所有入度为0的顶点，入栈，
		if (pal->vertex[i].indugree == 0)
		{
			PushStack(ps, &(pal->vertex[i]));
		}
	}

	PVertexNode pvn = NULL;
	//TOPO排序算法思想：选择一个入度为0的顶点，删除该顶点
	//并将以该顶点为起点的边删除。这里实现思路为依次将以该顶点为入度的顶点的入度数减1
	while ((pvn = (PVertexNode)PopStack(ps)) != NULL)
	{
		//队列保存排序结果
		EnQueue(pq, pvn);
		PArcNode pan = pvn->firstarc;
		while (NULL != pan)
		{
			if (--(pal->vertex[pan->adjvex].indugree) == 0)
			{
				//将以该顶点为入度的顶点的入度数减1，如果新的顶点入度为0，
				//则放入0入度顶点栈，参与0入度顶点的选择
				PushStack(ps, &(pal->vertex[pan->adjvex]));
			}
			pan = pan->nextarc;
		}
	}

	return SizeQueue(pq);
}

int TopoSortForCriticalPath(PAdjList& pal, PStack& psTopoSort, int*& ee)
{
	InitStack(psTopoSort);
	PStack psIndegree = NULL;
	InitStack(psIndegree);
	for (size_t i = 0; i < pal->vexnum; i++)
	{
		//为了防止重复遍历入度为0的顶点，这里先选出所有入度为0的顶点，入栈，
		if (pal->vertex[i].indugree == 0)
		{
			PushStack(psIndegree, &(pal->vertex[i]));
		}
	}

	PVertexNode pvn = NULL;
	//TOPO排序算法思想：选择一个入度为0的顶点，删除该顶点
	//并将以该顶点为起点的边删除。这里实现思路为依次将以该顶点为入度的顶点的入度数减1
	while ((pvn = (PVertexNode)PopStack(psIndegree)) != NULL)
	{
		//队列保存排序结果
		PushStack(psTopoSort, pvn);
		int index_src = FindVertex(pal, pvn->data);
		PArcNode pan = pvn->firstarc;
		while (NULL != pan)
		{
			if (ee[index_src] + pan->weight > ee[pan->adjvex])
			{
				ee[pan->adjvex] = ee[index_src] + pan->weight;
			}

			if (--(pal->vertex[pan->adjvex].indugree) == 0)
			{
				//将以该顶点为入度的顶点的入度数减1，如果新的顶点入度为0，
				//则放入0入度顶点栈，参与0入度顶点的选择
				PushStack(psIndegree, &(pal->vertex[pan->adjvex]));
			}
			pan = pan->nextarc;
		}
	}

	return SizeStack(psTopoSort);
}

void CriticalPath(PAdjList& pal, PCriticalPathItem& pcpi)
{
	int* ee = (int*)malloc(sizeof(int)*pal->vexnum);
	int* el = (int*)malloc(sizeof(int)*pal->vexnum);
	PStack psTopoSort = NULL;
	//初始化事件最早发生时间
	for (size_t i = 0; i < pal->vexnum; i++)
		ee[i] = 0;

	for (size_t i = 0; i < pal->arcnum; i++)
	{
		pcpi[i].src = pcpi[i].dst = pcpi[i].ae = pcpi[i].al = pcpi[i].loose = 0;
	}

	//由拓扑排序得出事件的先后顺序序列，并求出事件的最早开始时间
	if (pal->vexnum != TopoSortForCriticalPath(pal, psTopoSort, ee))
	{
		//如果存在回路，根据依赖关系，工程不可能完成（死锁）。
		printf("Graph is not DAG\n");
		return;
	}

	//用事件最早开始事件初始化事件最晚开始事件
	for (size_t i = 0; i < pal->vexnum; i++)
		el[i] = ee[i];

	//使用逆序拓扑顺序，得出事件最晚开始时间
	//这里不必使用一个逆向图来得到逆向顺序，使用拓扑顺序的逆向即可得到逆向顺序
	//顶点遍历顺序为逆向顺序后，边的方向无所谓了，边的权值（活动耗时）不变即可。
	while (!IsEmptyStack(psTopoSort))
	{
		PVertexNode pvn = (PVertexNode)PopStack(psTopoSort);
		int index_src = FindVertex(pal, pvn->data);
		PArcNode pan = pvn->firstarc;
		while (NULL != pan)
		{
			if (el[pan->adjvex] - pan->weight > el[index_src])
			{
				el[index_src] = el[pan->adjvex] - pan->weight;
			}
			pan = pan->nextarc;
		}
	}

	int arcindex = 0;	//记录活动数量，即边数
	//计算活动最早开始时间、最晚开始时间、松弛时间，并进行保存
	for (size_t i = 0; i < pal->vexnum; i++)
	{
		PArcNode pan = pal->vertex[i].firstarc;
		while (NULL != pan)
		{
			pcpi[arcindex].src = i;
			pcpi[arcindex].dst = pan->adjvex;
			pcpi[arcindex].ae = ee[i];
			pcpi[arcindex].al = el[pan->adjvex] - pan->weight;
			pcpi[arcindex].loose = pcpi[arcindex].al - pcpi[arcindex].ae;
			++arcindex;
			pan = pan->nextarc;
		}
	}
}

void PrintCriticalPathItem(PAdjList& pal, PCriticalPathItem& pcpi)
{
	for (size_t i = 0; i < pal->arcnum; i++)
	{
		char tag = ' ';
		if (pcpi[i].loose == 0)
			tag = '*';

		printf("%c %c-->%c\t%d\t%d\t%d\n",
			tag,
			pal->vertex[pcpi[i].src].data,
			pal->vertex[pcpi[i].dst].data,
			pcpi[i].ae,
			pcpi[i].al,
			pcpi[i].loose);
	}
}

void TestCriticalPath()
{
	char* svexs = "A, B, C, D, E, F, G, H, I";
	char* sarcs = "AB6, AC4, AD5, BE1, CE1, DF2, EG9, EH7, FH5, GI2, HI4";
	printf("vertex expression:%s\n", svexs);
	printf("arc expression:%s\n", sarcs);
	//创建有向网
	printf("CreateDN:\n");
	PAdjList paLDG = NULL;
	CreateDN(paLDG, svexs, sarcs);
	PrintAdjList(paLDG);
	PCriticalPathItem pcpi = (PCriticalPathItem)malloc(sizeof(CriticalPathItem)*(paLDG->arcnum));
	CriticalPath(paLDG, pcpi);
	printf("CriticalPath:\n");
	PrintCriticalPathItem(paLDG, pcpi);
}

void TestTopoSort()
{
	char* svexs = "A, B, C, D, E, F";
	char* sarcs = "AB, AC, AD, CB, CE, DE, FD, FE";
	printf("vertex expression:%s\n", svexs);
	printf("arc expression:%s\n", sarcs);
	//创建有向图
	printf("CreateDG:\n");
	PAdjList paLDG = NULL;
	CreateDG(paLDG, svexs, sarcs);
	PrintAdjList(paLDG);
	PQueue pq = NULL;
	TopoSort(paLDG, pq);
	if (SizeQueue(pq) != paLDG->vexnum)
	{
		printf("Graph is not DAG\n");
		return;
	}

	printf("TopoSort: ");
	PVertexNode pvn = NULL;
	while ((pvn = (PVertexNode)DelQueue(pq)) != NULL)
	{
		printf("%c ", pvn->data);
	}
	printf("\n");
}

void TestGraphDAG()
{
	TestTopoSort();
	TestCriticalPath();
}
