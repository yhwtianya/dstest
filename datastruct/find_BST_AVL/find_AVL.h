#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue_ds.h"
using namespace DS;

/*  AVL����ƶ���ƽ���������壺
	1 �����ǿ�����
	2 ���粻�ǿ������κ�һ������������������������ƽ������������Ҹ߶�֮��ľ���ֵ������1
	AVL��ͨ���ڲ����ɾ���ڵ�ʱ���ɵ����ϲ�������С��ƽ������ƽ����ά��ƽ���������ƽ�⡣
	������С��ƽ��������ͨ����ת�ڵ�����ɵġ���ƽ���״̬��LL��LR��RR��RL��
	LL��RR������תһ�μ��ɵ���������С��ƽ������ƽ�⣬LR��RL��Ҫ�ȶ���С��ƽ������������תһ�Σ�
	ʹ��С��ƽ�����γ�LL��RR��Ȼ���ٽ���һ����ת��������������ת�ɵ���������С��ƽ������ƽ�⡣
	�����걾����С��ƽ��������Ҫ������С��ƽ������ƽ�����ӣ������ϲ����ƽ���жϺ�ƽ����ڡ�
*/

typedef int KeyType;
typedef void* POtherInfo;
typedef struct AVLNode
{
	KeyType key;
	struct AVLNode* LChild;
	struct AVLNode* RChild;
	int high;				//�ڵ�������С�����ĸ߶ȣ����ڼ���ƽ������
}AVLNode, *PAVL;

//���롢����AVL���ڵ�
void InsertAVL(PAVL& pavl, KeyType& key);
//����AVL��
void DestoryAVL(PAVL& pavl);
//�����ַ������ʽ����AVL��
void CreateAVL(PAVL& pavl, char* str);
//���ʽڵ�
void VisitAVL(PAVL& pavl);
//�������AVL��
//AVL������������ǵõ�AVL���ڵ��С�����������У�����AVL��������
void InOrderAVL(PAVL& pavl);
//���ڵݹ�ʵ��AVL���Ĳ���
AVLNode* SearchAVL_Recursion(PAVL& pavl, KeyType key);
//����ѭ��ʵ��AVL���Ĳ���
AVLNode* SearchAVL(PAVL& pavl, KeyType key);
//ɾ��AVL���ڵ�
void DeleteAVL(PAVL* ppavl);
//bool DeleteAVL(PAVL* pavl);
//����key���Ҳ�ɾ����Ӧ�ڵ�
void DeleteAVL(PAVL* ppavl, KeyType key);
//bool DeleteAVL(PAVL& pavl, KeyType key);
void CopyTreeAVL(PAVL& pavl_s, PAVL& pavl_d);
void MakeAVLNode(PAVL& pavl, KeyType key);

//����״�ṹ�������NULLҲ�������
void PrintAVL(PAVL& pavl);
//����״�ṹ�������NULL���������
void PrintAVLNoNull(PAVL& pavl);
//���ݲ��������������֦��ǣ������Ƿ���NULL�ڵ�
void PrintBranchAVL(int level, int depth);
//���ݲ����������NULL�ڵ����֦���
void PrintBranchAVL(int level, int depth, PQueue* ppq);
//�������
void PrintEndLineAVL();
//����ո���ʵ�ǰ�ÿ���ڵ�ռ�õĿ������ո񣬼�NODE_CHAR_WIDE
//�ܿո���Ϊcount*NODE_CHAR_WIDE
void PrintfSpaceAVL(int count);
//NULL�ڵ�������
void PrintfAVLNullNode();
//NULL�ڵ������ո�
void PrintfAVLNullNodeNoNull();
//����ڵ���Ϣ
void PrintfAVLNode(PAVL& pavl);
int max_int(int x, int y);
//������������ÿ��ڵ���Ϣ
void PreOrderGetAVLLevelNode(PAVL pavl, int level, int depth, PQueue* ppq);
//�ڵ����������е����
int AVLNodeDepth(PAVL& pavl);
/*�ڵ�������С�����ĸ߶�
	�ڵ�������С�����ĸ߶����ɽڵ�������С������Ҷ�ӽڵ㿪ʼ����ģ����ڵ�����ǲο��ڵ����������Ĳ�������м���ġ� */
int HighAVL(PAVL& pavl);
//������������һ����ת���ʺϵ���LL�Ͳ�ƽ��״̬
void SingRotateLeft(PAVL* pavl);
//������������һ����ת���ʺϵ���RR�Ͳ�ƽ��״̬
void SingRotateRight(PAVL* pavl);
//˫�����ʺ�LR�Ͳ�ƽ��״̬����������RR��תʹ�����γ�LL��״̬���ٽ���LL��ת
void DoubleRotateLR(PAVL* pavl);
//˫�����ʺ�RL�Ͳ�ƽ��״̬����������LL��תʹ�����γ�RR��״̬���ٽ���RR��ת
void DoubleRotateRL(PAVL* pavl);

void TestSearchAVL();