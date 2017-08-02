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
	/* ����Ҳ���Ը�д�˺�����ֱ�ӽ������ҵ�����˺�����ƥ��ɹ��뿪���� */
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
		//���ݳ�ʼ�������ʼ����ѡ�ߣ��������ÿ��Ԫ�ع����������ı�
		//����ᰴ��Ȩֵ��С����ѡ��������㣬��������������������㣬����һ����
		//���������߱��������㼯�ϣ����汣��������㼯�ϣ����ȫ����Ϊ������
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
		//���ѭ����ѡ������������Ȩֵ��С�ıߣ����ƶ�����һ���������Ա�
		for (size_t j = i + 1; j < pal->vexnum - 1; j++)
		{
			//���αȽ����������㵽�����������Ȩֵ��
			//��Ȩֵ��С�ı߽ṹ�������������߼�������棬Ȼ�������ṹ�ķ���������Ϊ����������
			if (pmste[i].weight > pmste[j].weight)
			{
				CopyMST_Edge(pmste[i], mste_temp);
				CopyMST_Edge(pmste[j], pmste[i]);
				CopyMST_Edge(mste_temp, pmste[j]);
			}
		}

		//���ѭ���������������㵽�����������Ȩֵ���������������㵽���������������СȨ
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
	//���ѡ�ߵ�����������ͬһ�����Ķ��㼯���ڣ��򹹳��˻�·
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
		//��Ԫ���ٵļ��Ϻϲ���Ԫ�ض�ļ��ϣ�
		//��ΪԪ���ٵļ��ϵ�ÿ��Ԫ����Ҫ�������ڼ�������
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
		//��ʼ������Ԫ������
		pvik[i].nset = i;
		pvik[i].next = NULL;
		//��ʼ�����㼯��
		pset[i].count = 1;
		pset[i].vertexhead = &(pvik[i]);

		//��ʼ����Ȩ������
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

	//�Դ�Ȩ�߽������򣬷������Ȩֵ����ѡ��
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
	//Prim��Ҫ���ó�ʼ�����㣬����������������һ����ȫ����С��������
	//���ǻ��������ʼ���������С������
	//ȫ����С�����������ж��
	PMST_Edge pmste = (PMST_Edge)malloc((pamUN->vexnum - 1) * sizeof(MST_Edge));
	for (size_t i = 0; i < pamUN->vexnum; i++)
	{
		MinSpanningTree_Prim(pamUN, i, pmste);
		PrintMST_Edge(pamUN, pmste, pamUN->vexnum-1);
	}
}

void TestMinSpanningTree_Kruskal(PAdjList& pamUN)
{
	//Kruskal������Ψһ��ȫ����С������
	//ȫ����С�����������ж��
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
	//��������ͼ
	printf("\nCreateUG:\n");
	PAdjList pamUG = NULL;
	CreateUG(pamUG, svexs, sarcs);
	PrintAdjList(pamUG);
	//�ж϶����Ƿ���ͨ
	char csrc = 'A';
	char cdst = 'C';
	printf("%c %c %s\n", csrc, cdst, IsConnect(pamUG, csrc, cdst) ? " is connected" : "is not connectd");
	cdst = 'G';
	printf("%c %c %s\n", csrc, cdst, IsConnect(pamUG, csrc, cdst) ? " is connected" : "is not connectd");
	//��ȡ��ͨ����
	PGraphContentCompont pgcc = NULL;
	InitGraphContentCompont(pgcc);
	MakeConnectComponents(pamUG, pgcc);
	PrintGraphContentCompont(pgcc);

	svexs = "A, B, C, D, E, F";
	sarcs = "AB6, AC1, AD5, BC5, BE3, CD5, CE6, CF4, DF2, EF6";
	printf("vertex expression:%s\n", svexs);
	printf("arc expression:%s\n", sarcs);
	//����������
	printf("\nCreateUN:\n");
	PAdjList pamUN = NULL;
	CreateUN(pamUN, svexs, sarcs);
	PrintAdjList(pamUN);
	//Prim�㷨������С������
	printf("All MinSpanningTree with Prim:\n");
	TestMinSpanningTree_Prim(pamUN);
	//Kruskal�㷨������С������
	printf("MinSpanningTree with Kruskal:\n");
	TestMinSpanningTree_Kruskal(pamUN);


}
