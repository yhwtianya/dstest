#include "graph_app_m.h"
#include "stack_ds.h"
using namespace DS;

void ShortestPath_Dijkstra(PAdjMatrix& pam, int initV, vector<int> path[], int*& dist)
{
	//初始化初始点到个点的最小路径长度
	//dist是一个数组，记录初始点到每个顶点的最短路径权值
	//path是一个数组的数组，记录初始点到每个顶点形成最短路径过程中经过的顶点
	//如果初始点到目标顶点不能到达，则不存在最短路径，则路径长度为UN_DN_MAX_WEIGHT
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

	//初始点放入S集合
	s[initV] = true;
	//重复pam->vexnum-1次
	for (size_t i = 1; i <= pam->vexnum-1; i++)
	{
		int min = UN_DN_MAX_WEIGHT;
		int minIndex;
		for (size_t j = 0; j < pam->vexnum; j++)
		{
			//选择S和U中最新权值最小的边
			if (!s[j] && dist[j] < min)
			{
				minIndex = j;
				min = dist[j];
			}
		}
		if (UN_DN_MAX_WEIGHT == min)
		{
			//最小权值为不可到达，说明U中没有可到达的点了
			return;
		}

		//将最新点放入S集合
		s[minIndex] = true;
		//path[minIndex].push_back(minIndex);
		for (size_t k = 0; k < pam->vexnum; k++)
		{
			//用最新点到U集合中个点的权值更新S到U各点的最小权值
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
	//pppath记录到达目的顶点最短路径的上一个顶点位置
	//ppdist记录到达目的顶点的最短路径值
	//初始化pppath和ppdist
	for (size_t i = 0; i < pam->vexnum; i++)
	{
		for (size_t j = 0; j < pam->vexnum; j++)
		{
			ppdist[i][j] = GetArcWeight(pam, i, j, UN_DN_MAX_WEIGHT);
			pppath[i][j] = i;
		}
	}

	//从第1到第K个顶点，作为中间点形成最短路径
	for (size_t k = 0; k < pam->vexnum; k++)
	{
		for (size_t i = 0; i < pam->vexnum; i++)
		{
			for (size_t j = 0; j < pam->vexnum; j++)
			{
				if (ppdist[i][k] + ppdist[k][j] < ppdist[i][j])
				{
					//如果经过中间点k能使最短路径更短，则更新当前最短路径
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
	//创建无向图
	printf("CreateUG:\n");
	PAdjMatrix pamUG = NULL;
	CreateUG(pamUG, svexs, sarcs);
	PrintAdjMatrix(pamUG);
	//压缩矩阵创建无向图
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
	//邻接矩阵无向图深度优先遍历生成树
	printf("\nDepthFirstSearch:");
	TraverseGraph(pamUG1);
	printf("\n");

	printf("\nCreateUG with Compression Matrix:\n");
	PAdjMatrixCmpr pamUGCmpr1 = NULL;
	CreateUG(pamUGCmpr1, svexs1, sarcs1);
	PrintAdjMatrix(pamUGCmpr1);
	//压缩矩阵无向图深度优先遍历生成树
	printf("\nDepthFirstSearch:");
	TraverseGraph(pamUGCmpr1);
	printf("\n");
	//邻接矩阵无向图广度优先遍历生成树
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
	//Dijkstra某起点到任意一点最短路径算法
	printf("ShortestPath_Dijkstra:\n");
	for (int i=0; i<pamDN->vexnum; ++i)
	{
		ShortestPath_Dijkstra(pamDN, i, ppath, dist);
		PrintShortestPath_Dijkstra(pamDN, i, ppath, dist);
		printf("\n");
	}

	//Flody任意两点最短路径算法
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
