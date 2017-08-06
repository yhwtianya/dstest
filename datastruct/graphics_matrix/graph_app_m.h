#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "graph_matrix.h"
using namespace std;

/*  Dijkstra用来解决某一顶点到任意其他顶点的最短路径。
	最短路径算法步骤：
	1 g为邻接矩阵表示的带权图，将初始点s放入顶点集合S，其余顶点放入集合U，将s到其余顶点的路径长度初始化为权值
	2 从集合U选择顶点u，使得S中某顶点s到u为从S出发到U的最短路径
	3 将u顶点移动到S集合，根据u到U集合每个点的最短路径修改从S集合到U集合各顶点的最短路径
		如果dist[u]+g.arcs[u][ux] < dist[ux], 则修改dist[ux] = dist[u]+g.arcs[u][ux]
	4 重复2、3步n-1次即按最短路径长度递增的顺序，逐个求出s到其余各顶点的最短路径*/
void ShortestPath_Dijkstra(PAdjMatrix& pam, int initV, vector<int> path[], int*& dist);
void PrintShortestPath_Dijkstra(PAdjMatrix& pam, vector<int> path[], int*& dist);

/*  Flody用来解决所有的任意两个顶点的最短路径。
	最短路径算法步骤：
	1 用邻接矩阵表示的权值初始化任意两直接相邻点的最短路径值dist，如果不相邻则为无穷大，
		用任意边的起始点初始化最短路径path，如果两顶点不相邻则起点为自己
	2 选择一个顶点k，依次比较任意两点i、j的距离dist[i][j]和i经过顶点k到达j的距离的大小，
		如果dist[i][j] > dist[i][k]+dist[k][j]的值，则更新dist[i][j] = dist[i][k]+dist[k][j]，
		同时记录i到达j最短路径的上一个顶点位置，即path[i][j] = k
	3 从第一个顶点到最后一个顶点依次作为k，执行第二步
	4 执行完成后dist就保存了各顶点间的最短路径值，path保存了到达最短路径上一个顶点的值*/
void ShortestPath_Flody(PAdjMatrix& pam, int** pppath, int** ppdist);

void PrintShortestPath_Flody(PAdjMatrix& pam, int** pppath, int** ppdist);
void TestGraphMatrix();

void TestShortestPath();

