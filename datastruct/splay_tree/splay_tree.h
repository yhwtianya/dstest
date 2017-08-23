#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue_ds.h"
using namespace DS;

/*  ��չ���ĳ������������ģ����ǵ��ֲ���ԭ���ձ����ʵ������´ο����Իᱻ���ʣ�
���Ҵ���������ݿ�����һ�λᱻ���ʣ���Ϊ��ʹ��������ʱ���С������Ƶ�ʸߵ���Щ�ڵ�Ӧ���������ڿ���������λ�á�
�����������׵��뵽�������������ÿ�β��ҽڵ�֮����������ع����ѱ����ҵĽڵ���Ƶ�������
�����Ե�����ʽ�Ķ��������������չ����ÿ�ζ���չ�����в�����������ͨ����ת�ķ����ѱ����ʽڵ���ת��������λ�á�
Ϊ�˽���ǰ�����ʽڵ���ת������������ͨ�����ڵ��Ե�������ת��ֱ���ýڵ��Ϊ����Ϊֹ��
����ת��������֮�������ڲ��������������ݴ�С��ϵ��ָ������������ȫ��ģ�����£����л���������ƽ̯���Ӷ���ΪO��log n��
*/

/*
����û�оֲ��Ե����������˵��Splay�ܴﵽAVL���ٶȣ������оֲ��Եķ�����˵��Ч�ʻ�����������
������������m�����������ʵ�����Ϊk����kԶС��m��������չ��
��ô���ʵ�ʱ�临�ӶȾͺ�k�ǳ����ܣ���O��log k����
���̵��ļ����ʾ��ǻ������ݷ��ʵľֲ��ԣ������û�ϰ�ߣ��������ʵ��ļ�������ٵ��ҵ���
���뷨�У���������Ĵʻ�ᱻ������ʾ

Splay���ܱ�֤����ƽ�⣬Ҳ����֤���������ĳ��֣����ʺϵ��η���Ч�����еĳ���
*/

typedef int KeyType;
typedef void* POtherInfo;
typedef struct SplayTreeNode
{
	KeyType key;
	struct SplayTreeNode* parent;		//�����AVL�����˸��ڵ㣬ȡ����ƽ������
	struct SplayTreeNode* LChild;
	struct SplayTreeNode* RChild;
}SplayTreeNode, *PSplayTree;

//���롢����SplayTree���ڵ�
void InsertSplayTree(PSplayTree& pST, KeyType& key);
//����SplayTree��
void DestorySplayTree(PSplayTree& pST);
//�����ַ������ʽ����SplayTree��
void CreateSplayTree(PSplayTree& pST, char* str);
//���ʽڵ�
void VisitSplayTree(PSplayTree& pST);
//�������SplayTree��
//SplayTree������������ǵõ�SplayTree���ڵ��С�����������У�����BST��������
void InOrderSplayTree(PSplayTree& pST);
//SplayTree���Ĳ��ң����ҹ��̻�Բ��ҽڵ����ƥ��Ľڵ���з�ת�����䷭ת����
//��Ҫ�ٴ��жϸ��ڵ��key�Ƿ���key��ͬ
void SearchSplayTree(PSplayTree& pST, KeyType key);
//SplayTree���Ĳ��ң������ң�����ת������ֵΪ���ҽڵ����ƥ��Ľڵ�
//��Ҫ�ٴ��жϷ���ֵ�ڵ��key�Ƿ���key��ͬ
SplayTreeNode* SearchSplayTreeNoSplay(PSplayTree& pTree, KeyType key);
//�ڲ����ң�������з�ת������ֵΪ���ҽڵ����ƥ��Ľڵ�
PSplayTree SearchSplayTreeIn(PSplayTree& pST, KeyType key, PSplayTree& hot);
//���ڵ㸽�ӵ������
void AttachAsLChild(SplayTreeNode* pBase, SplayTreeNode* pNode);
//���ڵ㸽�ӵ��Ҷ���
void AttachAsRChild(SplayTreeNode* pBase, SplayTreeNode* pNode);
//��ת�ڵ㵽��������������Ҫ�Ĳ���
void Splay(PSplayTree& pNode);
//����keyɾ���ڵ㣬ͬʱ����ƥ��ڵ㷭ת������
void DeleteSplayTree(PSplayTree& pST, KeyType key);
//������չ��
void CopyTreeSplayTree(PSplayTree& pST_s, PSplayTree& pST_d);
//�����½ڵ�
void MakeSplayTreeNode(PSplayTree& pST, KeyType key);

//����״�ṹ�������NULLҲ�������
void PrintSplayTree(PSplayTree& pST);
//����״�ṹ�������NULL���������
void PrintSplayTreeNoNull(PSplayTree& pST);
//���ݲ��������������֦��ǣ������Ƿ���NULL�ڵ�
void PrintBranchSplayTree(int level, int depth);
//���ݲ����������NULL�ڵ����֦���
void PrintBranchSplayTree(int level, int depth, PQueue* ppq);
//�������
void PrintEndLineSplayTree();
//����ո���ʵ�ǰ�ÿ���ڵ�ռ�õĿ������ո񣬼�NODE_CHAR_WIDE
//�ܿո���Ϊcount*NODE_CHAR_WIDE
void PrintfSpaceSplayTree(int count);
//NULL�ڵ�������
void PrintfSplayTreeNullNode();
//NULL�ڵ������ո�
void PrintfSplayTreeNullNodeNoNull();
//����ڵ���Ϣ
void PrintfSplayTreeNode(PSplayTree& pST);
int max_int(int x, int y);
//������������ÿ��ڵ���Ϣ
void PreOrderGetSplayTreeLevelNode(PSplayTree pST, int level, int depth, PQueue* ppq);
//�ڵ����������е����
int SplayTreeNodeDepth(PSplayTree& pST);

void TestSplayTree();