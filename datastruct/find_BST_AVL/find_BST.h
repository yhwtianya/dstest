#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue_ds.h"
using namespace DS;
/*  �������Ĳ��ҷ��Ĳ��ҷ���Ҳ���������ҷ���
*	���������֯���ض����������ṹ��ʵ�ֲ��� */

/*	������������������һ�ÿ����������Ǿ����������ʵĶ�������
	1 �������������ǿգ��������������н���ֵ��С�ڸ�����ֵ��
	2 �������������ǿգ��������������н���ֵ�����ڣ�����ڵ��ڣ�������ֵ��
	3 ������������Ҳ�ֱ�Ϊ����������	*/

typedef int KeyType;
typedef void* POtherInfo;
typedef struct BSTreeNode
{
	KeyType key;
	struct BSTreeNode* LChild;
	struct BSTreeNode* RChild;
}BSTreeNode, *PBSTree;

//��������������ڵ�
void InsertBST(PBSTree& pbst, KeyType& key);
//��������������
void CreateBST(PBSTree& pbst, char* str);
//���ʽڵ�
void VisitBST(PBSTree& pbst);
//�����������������
//��������������������ǵõ������������ڵ��С�����������У����Ƕ���������������
void InOrderBST(PBSTree& pbst);
//���ڵݹ�ʵ�ֶ����������Ĳ���
BSTreeNode* SearchBST_Recursion(PBSTree& pbst, KeyType key);
//����ѭ��ʵ�ֶ����������Ĳ���
BSTreeNode* SearchBST(PBSTree& pbst, KeyType key);
//ɾ�������������ڵ�
bool DeleteBST(PBSTree* pbst);
//����key���Ҳ�ɾ����Ӧ�ڵ�
bool DeleteBST(PBSTree& pbst, KeyType key);
void CopyTreeBST(PBSTree& bt_s, PBSTree& bt_d);
void MakeBSTNode(PBSTree& bt, KeyType key);

//����״�ṹ�������NULLҲ�������
void PrintBST(PBSTree& bt);
//����״�ṹ�������NULL���������
void PrintBSTNoNull(PBSTree& bt);
//���ݲ��������������֦��ǣ������Ƿ���NULL�ڵ�
void PrintBranchBST(int level, int depth);
//���ݲ����������NULL�ڵ����֦���
void PrintBranchBST(int level, int depth, PQueue* ppq);
//�������
void PrintEndLineBST();
//����ո���ʵ�ǰ�ÿ���ڵ�ռ�õĿ������ո񣬼�NODE_CHAR_WIDE
//�ܿո���Ϊcount*NODE_CHAR_WIDE
void PrintfSpaceBST(int count);
//NULL�ڵ�������
void PrintfBSTNullNode();
//NULL�ڵ������ո�
void PrintfBSTNullNodeNoNull();
//����ڵ���Ϣ
void PrintfBSTNode(PBSTree& pbst);
int maxtreeBST(int x, int y);
//������������ÿ��ڵ���Ϣ
void PreOrderGetBSTLevelNode(PBSTree bt, int level, int depth, PQueue* ppq);
int BSTNodeDepth(PBSTree& bt);
void TestSearchBST();