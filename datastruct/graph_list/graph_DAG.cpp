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
		//Ϊ�˷�ֹ�ظ��������Ϊ0�Ķ��㣬������ѡ���������Ϊ0�Ķ��㣬��ջ��
		if (pal->vertex[i].indugree == 0)
		{
			PushStack(ps, &(pal->vertex[i]));
		}
	}

	PVertexNode pvn = NULL;
	//TOPO�����㷨˼�룺ѡ��һ�����Ϊ0�Ķ��㣬ɾ���ö���
	//�����Ըö���Ϊ���ı�ɾ��������ʵ��˼·Ϊ���ν��Ըö���Ϊ��ȵĶ�����������1
	while ((pvn = (PVertexNode)PopStack(ps)) != NULL)
	{
		//���б���������
		EnQueue(pq, pvn);
		PArcNode pan = pvn->firstarc;
		while (NULL != pan)
		{
			if (--(pal->vertex[pan->adjvex].indugree) == 0)
			{
				//���Ըö���Ϊ��ȵĶ�����������1������µĶ������Ϊ0��
				//�����0��ȶ���ջ������0��ȶ����ѡ��
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
		//Ϊ�˷�ֹ�ظ��������Ϊ0�Ķ��㣬������ѡ���������Ϊ0�Ķ��㣬��ջ��
		if (pal->vertex[i].indugree == 0)
		{
			PushStack(psIndegree, &(pal->vertex[i]));
		}
	}

	PVertexNode pvn = NULL;
	//TOPO�����㷨˼�룺ѡ��һ�����Ϊ0�Ķ��㣬ɾ���ö���
	//�����Ըö���Ϊ���ı�ɾ��������ʵ��˼·Ϊ���ν��Ըö���Ϊ��ȵĶ�����������1
	while ((pvn = (PVertexNode)PopStack(psIndegree)) != NULL)
	{
		//���б���������
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
				//���Ըö���Ϊ��ȵĶ�����������1������µĶ������Ϊ0��
				//�����0��ȶ���ջ������0��ȶ����ѡ��
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
	//��ʼ���¼����緢��ʱ��
	for (size_t i = 0; i < pal->vexnum; i++)
		ee[i] = 0;

	for (size_t i = 0; i < pal->arcnum; i++)
	{
		pcpi[i].src = pcpi[i].dst = pcpi[i].ae = pcpi[i].al = pcpi[i].loose = 0;
	}

	//����������ó��¼����Ⱥ�˳�����У�������¼������翪ʼʱ��
	if (pal->vexnum != TopoSortForCriticalPath(pal, psTopoSort, ee))
	{
		//������ڻ�·������������ϵ�����̲�������ɣ���������
		printf("Graph is not DAG\n");
		return;
	}

	//���¼����翪ʼ�¼���ʼ���¼�����ʼ�¼�
	for (size_t i = 0; i < pal->vexnum; i++)
		el[i] = ee[i];

	//ʹ����������˳�򣬵ó��¼�����ʼʱ��
	//���ﲻ��ʹ��һ������ͼ���õ�����˳��ʹ������˳������򼴿ɵõ�����˳��
	//�������˳��Ϊ����˳��󣬱ߵķ�������ν�ˣ��ߵ�Ȩֵ�����ʱ�����伴�ɡ�
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

	int arcindex = 0;	//��¼�������������
	//�������翪ʼʱ�䡢����ʼʱ�䡢�ɳ�ʱ�䣬�����б���
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
	//����������
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
	//��������ͼ
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
