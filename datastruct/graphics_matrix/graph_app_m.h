#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "graph_matrix.h"
using namespace std;

/*  Dijkstra�������ĳһ���㵽����������������·����
	���·���㷨���裺
	1 gΪ�ڽӾ����ʾ�Ĵ�Ȩͼ������ʼ��s���붥�㼯��S�����ඥ����뼯��U����s�����ඥ���·�����ȳ�ʼ��ΪȨֵ
	2 �Ӽ���Uѡ�񶥵�u��ʹ��S��ĳ����s��uΪ��S������U�����·��
	3 ��u�����ƶ���S���ϣ�����u��U����ÿ��������·���޸Ĵ�S���ϵ�U���ϸ���������·��
		���dist[u]+g.arcs[u][ux] < dist[ux], ���޸�dist[ux] = dist[u]+g.arcs[u][ux]
	4 �ظ�2��3��n-1�μ������·�����ȵ�����˳��������s���������������·��*/
void ShortestPath_Dijkstra(PAdjMatrix& pam, int initV, vector<int> path[], int*& dist);
void PrintShortestPath_Dijkstra(PAdjMatrix& pam, vector<int> path[], int*& dist);

/*  Flody����������е�����������������·����
	���·���㷨���裺
	1 ���ڽӾ����ʾ��Ȩֵ��ʼ��������ֱ�����ڵ�����·��ֵdist�������������Ϊ�����
		������ߵ���ʼ���ʼ�����·��path����������㲻���������Ϊ�Լ�
	2 ѡ��һ������k�����αȽ���������i��j�ľ���dist[i][j]��i��������k����j�ľ���Ĵ�С��
		���dist[i][j] > dist[i][k]+dist[k][j]��ֵ�������dist[i][j] = dist[i][k]+dist[k][j]��
		ͬʱ��¼i����j���·������һ������λ�ã���path[i][j] = k
	3 �ӵ�һ�����㵽���һ������������Ϊk��ִ�еڶ���
	4 ִ����ɺ�dist�ͱ����˸����������·��ֵ��path�����˵������·����һ�������ֵ*/
void ShortestPath_Flody(PAdjMatrix& pam, int** pppath, int** ppdist);

void PrintShortestPath_Flody(PAdjMatrix& pam, int** pppath, int** ppdist);
void TestGraphMatrix();

void TestShortestPath();

