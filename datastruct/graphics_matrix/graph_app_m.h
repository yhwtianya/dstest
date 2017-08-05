#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "graph_matrix.h"
using namespace std;

/*  Dijkstra���·���㷨���裺
	1 gΪ�ڽӾ����ʾ�Ĵ�Ȩͼ������ʼ��s���붥�㼯��S�����ඥ����뼯��U����s�����ඥ���·�����ȳ�ʼ��ΪȨֵ
	2 �Ӽ���Uѡ�񶥵�u��ʹ��S��ĳ����s��uΪ��S������U�����·��
	3 ��u�����ƶ���S���ϣ�����u��U��������·���޸Ĵ�S���ϵ�U���ϸ���������·��
		���dist[u]+g.arcs[u][ux] < dist[ux], ���鹹dist[ux] = dist[u]+g.arcs[u][ux]
	4 �ظ�2��3��n-1�μ������·�����ȵ�����˳��������s���������������·��*/
void ShortestPath_DJS(PAdjMatrix& pam, int initV, vector<int> path[], int*& dist);
void PrintShortestPath(PAdjMatrix& pam, vector<int> path[], int*& dist);
void TestGraphMatrix();

void TestShortestPath_DJS();

