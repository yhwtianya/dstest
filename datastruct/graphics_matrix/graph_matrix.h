#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define GRAPH_MAX_VERTEX_NUM	128
#define CMPR_MATRIX_ARC_MAX_NUM		(GRAPH_MAX_VERTEX_NUM*(GRAPH_MAX_VERTEX_NUM-1)/2+1)
typedef enum
{
	DG,	//有向图
	DN,	//有向网（有权图）
	UG,	//无向图
	UN,	//无向网（有权图）
}GraphKind;

typedef char VertexData;
typedef struct ArcNode	//弧节点
{
	int AdjType;	//代表是否邻接或权值
}ArcNode, *PArcNode;

//Adjoining Matrix
typedef struct AdjMatrix
{
	VertexData vexs[GRAPH_MAX_VERTEX_NUM];
	ArcNode arcs[GRAPH_MAX_VERTEX_NUM][GRAPH_MAX_VERTEX_NUM];
	int vexnum, arcnum;
	GraphKind kind;
}AdjMatrix, *PAdjMatrix;

void InitAdjMatrix(PAdjMatrix& pam, GraphKind kind);
void FreeAdjMatrix(PAdjMatrix& pam);
int FindVertex(PAdjMatrix& pam, char vex);
void CreateDG(PAdjMatrix& pam, char* sVertexs, char* sArcs);
void CreateUG(PAdjMatrix& pam, char* sVertexs, char* sArcs);
void PrintAdjMatrix(PAdjMatrix& pam);

/*	无向图（网）的弧在矩阵中有对称行，采用倒三角压缩矩阵存储无向图（网）
*	由于不存在自身到自身的弧，弧矩阵的对角线不需要存储,
*	压缩矩阵采用顺序存储,数组b中，共需 n*(n-1)/2 个数据来保存弧节点,
*	倒三角中，第i行保存i个元素（i=0, 1, ... n-1）。
*	数组元素总数m=1+2+...+(n-1) = n*(n-1)/2		
*	对于任意的i和j（1<=i<=n-1, 0<=j<i）,第i行第j列元素a(ij)存储于b[k]，
*	这里k=1+2+...+(i-1) + j = i*(i-1)/2 + j		
*	这里多留一个空间用来保存对角线默认值，转换坐标越界时返回默认值索引,
*	即数组总长度为 n*(n-1)/2 + 1		*/

typedef struct AdjMatrixCmpr
{
	VertexData vexs[GRAPH_MAX_VERTEX_NUM];
	ArcNode arcs[CMPR_MATRIX_ARC_MAX_NUM];
	int vexnum, arcnum;
	GraphKind kind;
}AdjMatrixCmpr, *PAdjMatrixCmpr;
void InitAdjMatrix(PAdjMatrixCmpr& pam, GraphKind kind);
void FreeAdjMatrix(PAdjMatrixCmpr& pam);
int FindVertex(PAdjMatrixCmpr& pam, char vex);
int GetArcIndex(int row, int col);
void CreateUG(PAdjMatrixCmpr& pam, char* sVertexs, char* sArcs);
void PrintAdjMatrix(PAdjMatrixCmpr& pam);
ArcNode InitArcNode(int adjtype);
ArcNode GetArc(PAdjMatrixCmpr& pam, int row, int col, ArcNode def=InitArcNode(0));
