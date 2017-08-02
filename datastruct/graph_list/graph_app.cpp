#include "graph_app.h"

void InitGraphContentCompont(PGraphContentCompont& pgcc)
{
	if (NULL == pgcc)
	{
		pgcc = (PGraphContentCompont)malloc(sizeof(GraphContentCompont));
		pgcc->next = NULL;
		pgcc->q = NULL;
	}
}

void FreeGraphContentCompont(PGraphContentCompont& pgccin)
{
	PGraphContentCompont pgcc = pgccin;
	while (pgcc && pgcc->q)
	{
		free(pgcc->q);
		pgcc = pgcc->next;
	}

	free(pgccin);
	pgccin = NULL;
}

bool IsConnect(PAdjList& pal, const VertexData src, const VertexData dst)
{
	int pos_src = FindVertex(pal, src);
	PQueue q = NULL;
	InitQueue(q);
	/* 这里也可以改写此函数，直接将被查找到传入此函数，匹配成功离开返回 */
	MakeConnectComponentForVertex(pal, pos_src, q);
	PVertexNode pvn = NULL;
	while (NULL != (pvn = (PVertexNode)DelQueue(q)))
	{
		if (pvn->data == dst)
		{
			return true;
		}
	}

	return false;
}

void MakeConnectComponents(PAdjList& pal, PGraphContentCompont pgcc)
{
	for (size_t i = 0; i < pal->vexnum; ++i)
	{
		pal->vertex[i].mark = 0;
	}
	for (size_t i = 0; i < pal->vexnum; ++i)
	{
		if (pal->vertex[i].mark == 0)
		{
			PQueue q = NULL;
			InitQueue(q);
			MakeConnectComponentForVertex(pal, i, q);

			if (NULL != pgcc->q)
			{
				InitGraphContentCompont(pgcc->next);
				pgcc = pgcc->next;
			}
			pgcc->q = q;
		}
	}
}

void PrintGraphContentCompont(PGraphContentCompont& pgcc)
{
	int count = 0;
	while (pgcc && pgcc->q)
	{
		++count;
		PVertexNode pvn;
		printf("Connect Component %d: ", count);
		while (NULL != (pvn = (PVertexNode)DelQueue(pgcc->q)))
		{
			printf("%c ", pvn->data);
		}
		printf("\n");
		pgcc = pgcc->next;
	}
}

void MakeConnectComponentForVertex(PAdjList& pal, int index_src, PQueue& q)
{
	pal->vertex[index_src].mark = 1;
	EnQueue(q, &(pal->vertex[index_src]));
	PArcNode pan_dst = GetFirstArc(pal, index_src);
	while (NULL != pan_dst)
	{
		if (pal->vertex[pan_dst->adjvex].mark == 0)
		{
			MakeConnectComponentForVertex(pal, pan_dst->adjvex, q);
		}
		pan_dst = GetNextArc(pan_dst);
	}
}

void MinSpanningTree_Prim(PAdjList& pal, int init_grow, PMST_Edge& pmste)
{
	int count = 0;
	for (size_t i = 0; i < pal->vexnum; i++)
	{
		//根据初始生长点初始化待选边，这个集合每个元素构成生成树的边
		//后面会按照权值大小依次选择非生长点，并将非生长点放入生长点，构成一个边
		//这个数组左边保存生长点集合，右面保存非生长点集合，最后全部变为生长点
		if (i != init_grow)
		{

			pmste[count].grow_vertex = init_grow;
			pmste[count].vertex = i;
			pmste[count].weight = GetArcWeight(pal, init_grow, i);
			++count;
		}
	}

	MST_Edge mste_temp;
	for (size_t i = 0; i < pal->vexnum; i++)
	{
		//这个循环是选出最新生长点权值最小的边，并移动到上一个生成树旁边
		for (size_t j = i + 1; j < pal->vexnum - 1; j++)
		{
			//依次比较最新生长点到各非生长点的权值，
			//把权值最小的边结构交换到生成树边集合最后面，然后把这个结构的非生长点作为最新生长点
			if (pmste[i].weight > pmste[j].weight)
			{
				CopyMST_Edge(pmste[i], mste_temp);
				CopyMST_Edge(pmste[j], pmste[i]);
				CopyMST_Edge(mste_temp, pmste[j]);
			}
		}

		//这个循环依据最新生长点到各非生长点的权值，更新所有生长点到各个非生长点的最小权
		for (size_t k = i + 1; k < pal->vexnum; k++)
		{
			if (k == init_grow)
				continue;

			int new_grow_weight = GetArcWeight(pal, pmste[i].vertex, pmste[k].vertex);
			if ( new_grow_weight < pmste[k].weight)
			{
				pmste[k].grow_vertex = pmste[i].vertex;
				pmste[k].weight = new_grow_weight;
			}
		}
	}
}

void PrintMST_Edge(PAdjList& pal, PMST_Edge& pmste, int size)
{
	printf("Start: %c, MSTree: ", pal->vertex[pmste[0].grow_vertex].data);
	int total_weight = 0;
	for (size_t i = 0; i < size; i++)
	{
		printf("%c-->%c %d", pal->vertex[pmste[i].grow_vertex].data,
			pal->vertex[pmste[i].vertex].data, pmste[i].weight);
		
		total_weight += pmste[i].weight;

		if (i + 1 < size)
			printf(", ");
	}

	printf(". Total Weigth: %d\n", total_weight);
}

void CopyMST_Edge(MST_Edge& esrc, MST_Edge& edst)
{
	edst.grow_vertex = esrc.grow_vertex;
	edst.vertex = esrc.vertex;
	edst.weight = esrc.weight;
}

bool IsLoopback(MST_Edge &msteedge, PVertexItemKruskal& pvik)
{
	//如果选边的两个顶点在同一子树的顶点集合内，则构成了回路
	if (pvik[msteedge.grow_vertex].nset == pvik[msteedge.vertex].nset)
		return true;
	return false;
}

void CopyetItemKruskal(SetItemKruskal& src, SetItemKruskal& dst)
{
	dst.count = src.count;
	dst.vertexhead = src.vertexhead;
}

int UnionSetItemKruskal(PSetItemKruskal& pset, int l, int r)
{
	if (pset[l].count < pset[r].count)
	{
		//将元素少的集合合并到元素多的集合，
		//因为元素少的集合的每个元素需要改其所在集合属性
		SetItemKruskal temp;
		CopyetItemKruskal(pset[l], temp);
		CopyetItemKruskal(pset[r], pset[l]);
		CopyetItemKruskal(temp, pset[r]);
	}
	PVertexItemKruskal pvx = pset[r].vertexhead;
	while (pvx != NULL)
	{
		pvx->nset = l;
		pvx = pvx->next;
	}

	pvx = pset[l].vertexhead;
	if (pvx == NULL)
	{
		pvx = pset[r].vertexhead;
	}
	else
	{
		while (pvx->next != NULL)
		{
			pvx = pvx->next;
		}
		pvx->next = pset[r].vertexhead;
	}

	pset[l].count = pset[l].count + pset[r].count;
	pset[r].count = 0;
	pset[r].vertexhead = NULL;
	return pset[l].count;
}

void MinSpanningTree_Kruskal(PAdjList& pal, PMST_Edge& pmste)
{
	PSetItemKruskal pset = (PSetItemKruskal)malloc(sizeof(SetItemKruskal)*pal->vexnum);
	PVertexItemKruskal pvik = (PVertexItemKruskal)malloc(sizeof(VertexItemKruskal)*pal->vexnum);

	int count = 0;
	for (size_t i=0; i<pal->vexnum; ++i)
	{
		//初始化顶点元素数组
		pvik[i].nset = i;
		pvik[i].next = NULL;
		//初始化顶点集合
		pset[i].count = 1;
		pset[i].vertexhead = &(pvik[i]);

		//初始化带权边数组
		PArcNode pan = GetFirstArc(pal, i);
		while (NULL!=pan)
		{
			if (pan->adjvex>i)
			{
				pmste[count].grow_vertex = i;
				pmste[count].vertex = pan->adjvex;
				pmste[count].weight = pan->weight;
				++count;
			}
			pan = pan->nextarc;
		}
	}
	//PrintMST_Edge(pal, pmste, pal->arcnum);

	//对带权边进行排序，方便后序按权值依次选择
	MST_Edge mste_temp;
	for (size_t i=0; i<count; ++i)
	{
		for (size_t j = (i+1); j<count; ++j)
		{
			if (pmste[i].weight > pmste[j].weight)
			{
				CopyMST_Edge(pmste[i], mste_temp);
				CopyMST_Edge(pmste[j], pmste[i]);
				CopyMST_Edge(mste_temp, pmste[j]);
			}
		}
	}
	//PrintMST_Edge(pal, pmste, pal->arcnum);

	int index = 0;
	for (size_t i = 0; i < count; i++)
	{
		if (IsLoopback(pmste[i], pvik))
			continue;
		else
		{
			int maxcount = UnionSetItemKruskal(pset, pvik[pmste[i].grow_vertex].nset, pvik[pmste[i].vertex].nset);
			CopyMST_Edge(pmste[i], pmste[index]);
			++index;
			if (index == pal->vexnum-1 || maxcount==pal->vexnum)
				break;
		}
	}
}

void TestMinSpanningTree_Prim(PAdjList& pamUN)
{
	//Prim需要设置初始生长点，所以它的生成树不一定是全局最小生成树，
	//它是基于这个初始生长点的最小生成树
	//全局最小生成树可能有多个
	PMST_Edge pmste = (PMST_Edge)malloc((pamUN->vexnum - 1) * sizeof(MST_Edge));
	for (size_t i = 0; i < pamUN->vexnum; i++)
	{
		MinSpanningTree_Prim(pamUN, i, pmste);
		PrintMST_Edge(pamUN, pmste, pamUN->vexnum-1);
	}
}

void TestMinSpanningTree_Kruskal(PAdjList& pamUN)
{
	//Kruskal会生成唯一的全局最小生成树
	//全局最小生成树可能有多个
	PMST_Edge pmste = (PMST_Edge)malloc((pamUN->arcnum) * sizeof(MST_Edge));
	MinSpanningTree_Kruskal(pamUN, pmste);
	PrintMST_Edge(pamUN, pmste, pamUN->vexnum-1);
}

void TestGraphApp()
{
	char* svexs = "A, B, C, D, E, F, G";
	char* sarcs = "AB, BC, CD, DA, EF, FG, GE";
	printf("vertex expression:%s\n", svexs);
	printf("arc expression:%s\n", sarcs);
	//创建无向图
	printf("\nCreateUG:\n");
	PAdjList pamUG = NULL;
	CreateUG(pamUG, svexs, sarcs);
	PrintAdjList(pamUG);
	//判断顶点是否连通
	char csrc = 'A';
	char cdst = 'C';
	printf("%c %c %s\n", csrc, cdst, IsConnect(pamUG, csrc, cdst) ? " is connected" : "is not connectd");
	cdst = 'G';
	printf("%c %c %s\n", csrc, cdst, IsConnect(pamUG, csrc, cdst) ? " is connected" : "is not connectd");
	//获取连通分量
	PGraphContentCompont pgcc = NULL;
	InitGraphContentCompont(pgcc);
	MakeConnectComponents(pamUG, pgcc);
	PrintGraphContentCompont(pgcc);

	svexs = "A, B, C, D, E, F";
	sarcs = "AB6, AC1, AD5, BC5, BE3, CD5, CE6, CF4, DF2, EF6";
	printf("vertex expression:%s\n", svexs);
	printf("arc expression:%s\n", sarcs);
	//创建无向网
	printf("\nCreateUN:\n");
	PAdjList pamUN = NULL;
	CreateUN(pamUN, svexs, sarcs);
	PrintAdjList(pamUN);
	//Prim算法生成最小生成树
	printf("All MinSpanningTree with Prim:\n");
	TestMinSpanningTree_Prim(pamUN);
	//Kruskal算法生成最小生成树
	printf("MinSpanningTree with Kruskal:\n");
	TestMinSpanningTree_Kruskal(pamUN);


}
