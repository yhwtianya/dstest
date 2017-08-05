#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "graph_matrix.h"
using namespace std;

/*  Dijkstra最短路径算法步骤：
	1 g为邻接矩阵表示的带权图，将初始点s放入顶点集合S，其余顶点放入集合U，将s到其余顶点的路径长度初始化为权值
	2 从集合U选择顶点u，使得S中某顶点s到u为从S出发到U的最短路径
	3 将u顶点移动到S集合，根据u到U个点的最短路径修改从S集合到U集合各顶点的最短路径
		如果dist[u]+g.arcs[u][ux] < dist[ux], 则虚构dist[ux] = dist[u]+g.arcs[u][ux]
	4 重复2、3步n-1次即按最短路径长度递增的顺序，逐个求出s到其余各顶点的最短路径*/
void ShortestPath_DJS(PAdjMatrix& pam, int initV, vector<int> path[], int*& dist);
void PrintShortestPath(PAdjMatrix& pam, vector<int> path[], int*& dist);
void TestGraphMatrix();

void TestShortestPath_DJS();

