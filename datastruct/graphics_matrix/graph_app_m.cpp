#include "graph_app_m.h"
#include "stack_ds.h"
using namespace DS;

void ShortestPath_Dijkstra(PAdjMatrix& pam, int initV, vector<int> path[], int*& dist)
{
	//��ʼ����ʼ�㵽�������С·������
	//dist��һ�����飬��¼��ʼ�㵽ÿ����������·��Ȩֵ
	//path��һ����������飬��¼��ʼ�㵽ÿ�������γ����·�������о����Ķ���
	//�����ʼ�㵽Ŀ�궥�㲻�ܵ���򲻴������·������·������ΪUN_DN_MAX_WEIGHT
	bool* s = (bool*)malloc(sizeof(bool)*pam->vexnum);
	for (size_t i = 0; i < pam->vexnum; i++)
	{
		path[i].clear();
		dist[i] = GetArcWeight(pam, initV, i, UN_DN_MAX_WEIGHT);
		s[i] = false;
		if (dist[i] != UN_DN_MAX_WEIGHT)
		{
			path[i].push_back(initV);
			path[i].push_back(i);
		}
	}

	//��ʼ�����S����
	s[initV] = true;
	//�ظ�pam->vexnum-1��
	for (size_t i = 1; i <= pam->vexnum-1; i++)
	{
		int min = UN_DN_MAX_WEIGHT;
		int minIndex;
		for (size_t j = 0; j < pam->vexnum; j++)
		{
			//ѡ��S��U������Ȩֵ��С�ı�
			if (!s[j] && dist[j] < min)
			{
				minIndex = j;
				min = dist[j];
			}
		}
		if (UN_DN_MAX_WEIGHT == min)
		{
			//��СȨֵΪ���ɵ��˵��U��û�пɵ���ĵ���
			return;
		}

		//�����µ����S����
		s[minIndex] = true;
		//path[minIndex].push_back(minIndex);
		for (size_t k = 0; k < pam->vexnum; k++)
		{
			//�����µ㵽U�����и����Ȩֵ����S��U�������СȨֵ
			if (!s[k] && dist[minIndex] + GetArcWeight(pam, minIndex, k, UN_DN_MAX_WEIGHT) < dist[k])
			{
				dist[k] = dist[minIndex] + GetArcWeight(pam, minIndex, k, UN_DN_MAX_WEIGHT);
				path[k] = path[minIndex];
				path[k].push_back(k);
			}
		}
	}
}

void PrintShortestPath_Dijkstra(PAdjMatrix& pam, int initV, vector<int> path[], int*& dist)
{
	printf("InitVertex:%c ,ShortestPaths:\n", pam->vexs[initV]);
	for (size_t i = 0; i < pam->vexnum; i++)
	{
		if (i == initV)
		{
			printf("(%c-%c) w=000: %c-->%c.\n", pam->vexs[initV], pam->vexs[i], pam->vexs[initV], pam->vexs[i]);
		}
		else if (path[i].size() == 0)
		{
			printf("(%c-%c) w= - : %c-->%c.\n", pam->vexs[initV], pam->vexs[i], pam->vexs[initV], pam->vexs[i]);
		}
		else
		{
			printf("(%c-%c) w=%03d: ", pam->vexs[*(path[i].begin())], pam->vexs[*(path[i].rbegin())], dist[i]);
			char char_src = pam->vexs[initV];
			vector<int>::iterator it = path[i].begin();
			for (; it != path[i].end(); )
			{
				if (it == path[i].begin())
				{
					char_src = pam->vexs[*it];
					++it;
					continue;
				}

				printf("%c-->%c", char_src, pam->vexs[*it]);
				char_src = pam->vexs[*it];
				if (++it !=path[i].end())
				{
					printf(", ");
				}
			}

			printf(".\n");
		}
	}
}

void ShortestPath_Flody(PAdjMatrix& pam, int** pppath, int** ppdist)
{
	//pppath��¼����Ŀ�Ķ������·������һ������λ��
	//ppdist��¼����Ŀ�Ķ�������·��ֵ
	//��ʼ��pppath��ppdist
	for (size_t i = 0; i < pam->vexnum; i++)
	{
		for (size_t j = 0; j < pam->vexnum; j++)
		{
			ppdist[i][j] = GetArcWeight(pam, i, j, UN_DN_MAX_WEIGHT);
			pppath[i][j] = i;
		}
	}

	//�ӵ�1����K�����㣬��Ϊ�м���γ����·��
	for (size_t k = 0; k < pam->vexnum; k++)
	{
		for (size_t i = 0; i < pam->vexnum; i++)
		{
			for (size_t j = 0; j < pam->vexnum; j++)
			{
				if (ppdist[i][k] + ppdist[k][j] < ppdist[i][j])
				{
					//��������м��k��ʹ���·�����̣�����µ�ǰ���·��
					ppdist[i][j] = ppdist[i][k] + ppdist[k][j];
					pppath[i][j] = k;
				}
			}
		}
	}
}

void PrintShortestPath_Flody(PAdjMatrix& pam, int** pppath, int** ppdist)
{
	PStack ps = NULL;
	InitStack(ps);
	for (size_t i = 0; i < pam->vexnum; i++)
	{
		printf("InitVertex:%c ,ShortestPaths:\n", pam->vexs[i]);
		for (size_t j = 0; j < pam->vexnum; j++)
		{
			if (ppdist[i][j]!=UN_DN_MAX_WEIGHT)
				printf("(%c-%c) w=%03d: ", pam->vexs[i], pam->vexs[j], ppdist[i][j]);
			else
				printf("(%c-%c) w= - : ", pam->vexs[i], pam->vexs[j], ppdist[i][j]);

			if (i == j)
			{
				printf("%c-->%c.\n", pam->vexs[i], pam->vexs[j]);
			}
			else
			{
				char char_src = pam->vexs[i];
				int* nextpath = &(pppath[i][j]);
				while (*nextpath != i)
				{
					PushStack(ps, nextpath);
					nextpath = &(pppath[i][*nextpath]);
				}

				while (!IsEmptyStack(ps))
				{
					nextpath = (int*)(PopStack(ps));
					printf("%c-->%c, ", char_src, pam->vexs[*nextpath]);
					char_src = pam->vexs[*nextpath];
				}
				printf("%c-->%c.\n", char_src, pam->vexs[j]);
			}
		}
		printf("\n");
	}
}

void TestGraphMatrix()
{
	char* svexs = "A, B, C, D, E, C, A";
	char* sarcs = "AB, AD, B C , DC, DA, C E";
	printf("vertex expression:%s\n", svexs);
	printf("arc expression:%s\n", sarcs);
	//��������ͼ
	printf("CreateUG:\n");
	PAdjMatrix pamUG = NULL;
	CreateUG(pamUG, svexs, sarcs);
	PrintAdjMatrix(pamUG);
	//ѹ�����󴴽�����ͼ
	printf("\nCreateUG with Compression Matrix:\n");
	PAdjMatrixCmpr pamUGCmpr = NULL;
	CreateUG(pamUGCmpr, svexs, sarcs);
	PrintAdjMatrix(pamUGCmpr);

	printf("\nCreateDG:\n");
	PAdjMatrix pamDG = NULL;
	CreateDG(pamDG, svexs, sarcs);
	PrintAdjMatrix(pamDG);

	char* svexs1 = "A, B, C, D, E, F, G, H, I";
	char* sarcs1 = "AB, AD, AE, BE, BC, CF, EG, DG, GH, HI";
	printf("vertex expression:%s\n", svexs1);
	printf("arc expression:%s\n", sarcs1);
	printf("CreateUG:\n");
	PAdjMatrix pamUG1 = NULL;
	CreateUG(pamUG1, svexs1, sarcs1);
	PrintAdjMatrix(pamUG1);
	//�ڽӾ�������ͼ������ȱ���������
	printf("\nDepthFirstSearch:");
	TraverseGraph(pamUG1);
	printf("\n");

	printf("\nCreateUG with Compression Matrix:\n");
	PAdjMatrixCmpr pamUGCmpr1 = NULL;
	CreateUG(pamUGCmpr1, svexs1, sarcs1);
	PrintAdjMatrix(pamUGCmpr1);
	//ѹ����������ͼ������ȱ���������
	printf("\nDepthFirstSearch:");
	TraverseGraph(pamUGCmpr1);
	printf("\n");
	//�ڽӾ�������ͼ������ȱ���������
	printf("\nBradeFirstSearch:");
	TraverseGraphBFS(pamUG1);
	printf("\n");
}

void TestShortestPath()
{
	char* svexs1 = "A, B, C, D, E, F";
	char* sarcs1 = "AB50, AC10, AE45, BC15, BE10, CA20, CD15, DB20, DE35, ED30, FD3";
	printf("vertex expression:%s\n", svexs1);
	printf("arc expression:%s\n", sarcs1);
	printf("CreateUG:\n");
	PAdjMatrix pamDN = NULL;
	CreateDN(pamDN, svexs1, sarcs1);
	PrintAdjMatrix(pamDN);
	int* dist = (int*)malloc(sizeof(int)*pamDN->vexnum);
	vector<int> * ppath = new vector<int>[pamDN->vexnum];
	//Dijkstraĳ��㵽����һ�����·���㷨
	printf("ShortestPath_Dijkstra:\n");
	for (int i=0; i<pamDN->vexnum; ++i)
	{
		ShortestPath_Dijkstra(pamDN, i, ppath, dist);
		PrintShortestPath_Dijkstra(pamDN, i, ppath, dist);
		printf("\n");
	}

	//Flody�����������·���㷨
	int** pppath = (int**)malloc(sizeof(int*)*pamDN->vexnum);
	int** ppdist = (int**)malloc(sizeof(int*)*pamDN->vexnum);
	for (size_t i = 0; i < pamDN->vexnum; i++)
	{
		pppath[i] = (int*)malloc(sizeof(int)*pamDN->vexnum);
		ppdist[i] = (int*)malloc(sizeof(int)*pamDN->vexnum);
	}
	ShortestPath_Flody(pamDN, pppath, ppdist);
	printf("-------------\nShortestPath_Flody:\n");
	PrintShortestPath_Flody(pamDN, pppath, ppdist);

}
