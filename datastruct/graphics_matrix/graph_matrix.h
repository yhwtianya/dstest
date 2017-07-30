#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define GRAPH_MAX_VERTEX_NUM	128
#define CMPR_MATRIX_ARC_MAX_NUM		(GRAPH_MAX_VERTEX_NUM*(GRAPH_MAX_VERTEX_NUM-1)/2+1)
typedef enum
{
	DG,	//����ͼ
	DN,	//����������Ȩͼ��
	UG,	//����ͼ
	UN,	//����������Ȩͼ��
}GraphKind;

typedef char VertexData;
typedef struct ArcNode	//���ڵ�
{
	int AdjType;	//�����Ƿ��ڽӻ�Ȩֵ
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
int GetNextArc(PAdjMatrix& pam, int row, int startcol);
int GetFirstArc(PAdjMatrix& pam, int row);
void Visit(PAdjMatrix& pam, int index);
void VisitArc(PAdjMatrix& pam, int src, int dst);
void DepthFirstSearch(PAdjMatrix& pam, bool visited[], int row);
void TraverseGraph(PAdjMatrix& pam);

/*	����ͼ�������Ļ��ھ������жԳ��У����õ�����ѹ������洢����ͼ������
*	���ڲ�������������Ļ���������ĶԽ��߲���Ҫ�洢,
*	ѹ���������˳��洢,����b�У����� n*(n-1)/2 �����������满�ڵ�,
*	�������У���i�б���i��Ԫ�أ�i=0, 1, ... n-1����
*	����Ԫ������m=1+2+...+(n-1) = n*(n-1)/2		
*	���������i��j��1<=i<=n-1, 0<=j<i��,��i�е�j��Ԫ��a(ij)�洢��b[k]��
*	����k=1+2+...+(i-1) + j = i*(i-1)/2 + j		
*	�������һ���ռ���������Խ���Ĭ��ֵ��ת������Խ��ʱ����Ĭ��ֵ����,
*	�������ܳ���Ϊ n*(n-1)/2 + 1		*/

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
int GetFirstArc(PAdjMatrixCmpr& pam, int row);
int GetNextArc(PAdjMatrixCmpr& pam, int row, int startcol);
void DepthFirstSearch(PAdjMatrixCmpr& pam, bool visited[], int index_src);
void TraverseGraph(PAdjMatrixCmpr& pam);
void Visit(PAdjMatrixCmpr& pam, int index);
void VisitArc(PAdjMatrixCmpr& pam, int src, int dst);
