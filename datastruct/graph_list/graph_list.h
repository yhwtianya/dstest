#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define GRAPH_MAX_VERTEX_NUM	128
typedef enum
{
	DG,	//����ͼ
	DN,	//����������Ȩͼ��
	UG,	//����ͼ
	UN,	//����������Ȩͼ��
}GraphKind;

typedef struct OtherInfo	//���ڵ�
{
	int AdjType;	//����Ȩֵ����Ϣ
}OtherInfo;

typedef struct ArcNode	//���ڵ�
{
	int adjvex;
	ArcNode* nextarc;
	OtherInfo* info;
}ArcNode, *PArcNode;

typedef char VertexData;

typedef struct VertexNode
{
	VertexData data;
	ArcNode* firstarc;
}VertexNode, *PVertexNode;

//Adjoining Matrix
typedef struct AdjList
{
	VertexNode vertex[GRAPH_MAX_VERTEX_NUM];
	int vexnum, arcnum;
	GraphKind kind;
}AdjList, *PAdjList;

void InitAdjList(PAdjList& pam, GraphKind kind);
void FreeAdjList(PAdjList& pam);
int FindVertex(PAdjList& pam, char vex);
void CreateDG(PAdjList& pam, char* sVertexs, char* sArcs);
void CreateUG(PAdjList& pam, char* sVertexs, char* sArcs);
void PrintAdjList(PAdjList& pam);
PArcNode InitArcNode(int adjvex);
bool HasArc(PAdjList& al, int adjvex_src, int adjvex_dst);
PArcNode GetArc(PAdjList& al, int adjvex_src, int adjvex_dst);
void PushArc(PAdjList& al, int adjvex_src, int adjvex_dst);
PArcNode GetNextArc(PArcNode& pan);
void Visit(PAdjList& pam, int index);
void VisitArc(PAdjList& pal, int src, int dst);
void DepthFirstSearch(PAdjList& pal, bool visited[], int index_src);
void TraverseGraph(PAdjList& pal);