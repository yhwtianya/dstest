#include "splay_tree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue_ds.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <time.h>
#include <vector>
using namespace std;
#define NODE_CHAR_WIDE		2
using namespace DS;

void InsertSplayTree(PSplayTree& pST, KeyType& key)
{
	if (NULL == pST)
	{
		MakeSplayTreeNode(pST, key);
		return;
	}

	//����ʱ���Ѿ������˷�ת�����ڵ�ҪôΪkey��ƥ��ڵ㣬ҪôΪ�����ƥ��ڵ�
	SearchSplayTree(pST, key);
	if (pST)
	{
		if (pST->key != key)
		{
			//���ڵ�Ϊ�����ƥ��ڵ㣬���ݴ�����ڵ�����ڵ��С��ϵ���в��룬�����¸��ڵ�
			SplayTreeNode* pNew = NULL;
			MakeSplayTreeNode(pNew, key);
			if (key < pST->key)
			{
				AttachAsLChild(pNew, pST->LChild);	//�����ӽڵ��ƶ����½ڵ�
				pST->LChild = NULL;		//���ӽڵ�ļ��и�ֵһ��Ҫ�ɶԳ��֣���ʹ��ֵΪNULL
				AttachAsRChild(pNew, pST);
			}
			else
			{
				AttachAsRChild(pNew, pST->RChild);	//�����ӽڵ��ƶ����½ڵ�
				pST->RChild = NULL;		//���ӽڵ�ļ��и�ֵһ��Ҫ�ɶԳ��֣���ʹ��ֵΪNULL
				AttachAsLChild(pNew, pST);
			}

			pST = pNew;
		}
	}
}

void DestorySplayTree(PSplayTree& pST)
{
	if (NULL!=pST)
	{
		DestorySplayTree(pST->LChild);
		DestorySplayTree(pST->RChild);
		free(pST);
		pST = NULL;
	}
}

void CreateSplayTree(PSplayTree& pST, char* str)
{
	for (size_t i = 0; i < strlen(str); i++)
	{
		if (str[i] != ',' && str[i] != ' ')
		{
			KeyType key = atoi(&str[i]);
			InsertSplayTree(pST, key);
			//PrintSplayTreeNoNull(pST);
			while (i < strlen(str) && str[++i] != ',');
		}
	}
}

void VisitSplayTree(PSplayTree& pST)
{
	printf("%d ", pST->key);
}

void InOrderSplayTree(PSplayTree& pST)
{
	if (NULL != pST)
	{
		InOrderSplayTree(pST->LChild);
		VisitSplayTree(pST);
		InOrderSplayTree(pST->RChild);
	}
}

void SearchSplayTree(PSplayTree& pTree, KeyType key)
{
	if (NULL!=pTree)
	{
		SplayTreeNode* pHot = NULL;
		//ע�⣺���ﷵ�ص���ƥ��ڵ�������ƥ��ڵ�
		SplayTreeNode* pNode = SearchSplayTreeIn(pTree, key, pHot);
		pTree = pNode ? pNode : pHot;
		Splay(pTree);
	}
}

SplayTreeNode* SearchSplayTreeNoSplay(PSplayTree& pTree, KeyType key)
{
	if (NULL != pTree)
	{
		SplayTreeNode* pHot = NULL;
		SplayTreeNode* pNode = SearchSplayTreeIn(pTree, key, pHot);
		return pNode;
	}

	return NULL;
}

//������
void AttachAsLChild(SplayTreeNode* pBase, SplayTreeNode* pNode)
{
	//���жϽڵ㲻Ϊ���ٽ��в���
	if (pBase)
	{
		pBase->LChild = pNode;
		//���жϽڵ㲻Ϊ���ٽ��в���
		if (pNode)
			pNode->parent = pBase;
	}
}

//���Һ���
void AttachAsRChild(SplayTreeNode* pBase, SplayTreeNode* pNode)
{
	//���жϽڵ㲻Ϊ���ٽ��в���
	if (pBase)
	{
		pBase->RChild = pNode;
		//���жϽڵ㲻Ϊ���ٽ��в���
		if (pNode)
			pNode->parent = pBase;
	}
}

//���LL�ͽ�����չ
SplayTreeNode* ZigZig(SplayTreeNode* v, SplayTreeNode* p, SplayTreeNode* g)
{
	//���ﲻͬ��AVL��LL��ת�����﷭ת�Ƚ����游�͸��ڵ㷭ת���Ž��б��ڵ�͸��ڵ㷭ת
	//�����ͬ��SplayTree��Ч�Ĺؼ�������ʹ���������ĸ߶ȼ���
	AttachAsLChild(g, p->RChild);	//�����ӽڵ����
	AttachAsRChild(p, g);			//���ӽڵ����¸�ֵ�����ӽڵ�ļ��и�ֵһ��Ҫ�ɶԳ���
	AttachAsLChild(p, v->RChild);
	AttachAsRChild(v, p);
	return v;
}

//���LR�ͽ�����չ
SplayTreeNode* ZigZag(SplayTreeNode* v, SplayTreeNode* p, SplayTreeNode* g)
{
	//��AVL��LR��ת����
	AttachAsRChild(p, v->LChild);	//�����ӽڵ����
	AttachAsLChild(v, p);			//���ӽڵ����¸�ֵ�����ӽڵ�ļ��и�ֵһ��Ҫ�ɶԳ��֣���ʹ��ֵΪNULL
	AttachAsLChild(g, v->RChild);
	AttachAsRChild(v, g);
	return v;
}

//���RL�ͽ�����չ
SplayTreeNode* ZagZig(SplayTreeNode* v, SplayTreeNode* p, SplayTreeNode* g)
{
	//��AVL��RL��ת����
	AttachAsLChild(p, v->RChild);
	AttachAsRChild(v, p);
	AttachAsRChild(g, v->LChild);
	AttachAsLChild(v, g);
	return v;
}

//���RR�ͽ�����չ
SplayTreeNode* ZagZag(SplayTreeNode* v, SplayTreeNode* p, SplayTreeNode* g)
{
	//���ﲻͬ��AVL��RR��ת�����﷭ת�Ƚ����游�͸��ڵ㷭ת���Ž��б��ڵ�͸��ڵ㷭ת
	//�����ͬ��SplayTree��Ч�Ĺؼ�������ʹ���������ĸ߶ȼ���
	AttachAsRChild(g, p->LChild);
	AttachAsLChild(p, g);
	AttachAsRChild(p, v->LChild);
	AttachAsLChild(v, p);
	return v;
}

//���L�ͽ�����չ
void Zig(SplayTreeNode* v, SplayTreeNode* p)
{
	AttachAsLChild(p, v->RChild);	//�����ӽڵ����
	AttachAsRChild(v, p);			//���ӽڵ�ļ��и�ֵһ��Ҫ�ɶԳ��֣���ʹ��ֵΪNULL
}

//���R�ͽ�����չ
void Zag(SplayTreeNode* v, SplayTreeNode* p)
{
	AttachAsRChild(p, v->LChild);
	AttachAsLChild(v, p);
}

void Splay(PSplayTree& pNode)
{
	SplayTreeNode* p = NULL;	//���ڵ�
	SplayTreeNode* g = NULL;	//�游�ڵ�

	//�游�͸��ڵ㶼����ʱ��������չ����˫�㷭ת��ֱ�����ڵ���游�ڵ�ΪNULLֹͣ˫�㷭ת
	while ((p=pNode->parent) && (g=p->parent))
	{
		SplayTreeNode* gg = g->parent;	//���游�ڵ�
		//�����游�����������ڵ�Ķ�Ӧ��ϵ���в�ͬ�ķ�ת
		if (p == g->LChild)
		{
			if (pNode==p->LChild)
			{
				pNode = ZigZig(pNode, p, g);
			}
			else
			{
				pNode = ZigZag(pNode, p, g);
			}
		}
		else
		{
			if (pNode == p->LChild)
			{
				pNode = ZagZig(pNode, p, g);
			}
			else
			{
				pNode = ZagZag(pNode, p, g);
			}
		}

		//�������游���游�Ĺ�ϵ�������¸��ĸ��ڵ�
		if (!gg)
			pNode->parent = NULL;
		else if (g == gg->LChild)
		{
			gg->LChild = pNode;
			pNode->parent = gg;
		}
		else
		{
			gg->RChild = pNode;
			pNode->parent = gg;
		}
	}

	//���ڵ�������游�ڵ�ΪNULL������е��㷭ת�������ڵ㷭ת������
	if (p=pNode->parent)
	{
		//���ݱ��ڵ�͸��ڵ��ϵ���е��㷭ת
		if (pNode==p->LChild)
		{
			Zig(pNode, p);
		}
		else
		{
			Zag(pNode, p);
		}
	}

	//���ڵ�ĸ���ΪNULL
	pNode->parent = NULL;
}

SplayTreeNode* GetMinInRight(PSplayTree pTree)
{
	//��ȡ����������С�ڵ�
	while (pTree->LChild != NULL)
	{
		pTree = pTree->LChild;
	}

	return pTree;
}

void DeleteSplayTree(PSplayTree& pST, KeyType key)
{
	if (NULL!=pST)
	{
		//����ʱ���Ѿ������˷�ת�����ڵ�ҪôΪ��ɾ���ڵ㣬ҪôΪ�����ƥ��ڵ�
		SearchSplayTree(pST, key);
		if (pST && (key==pST->key))
		{
			if (pST->RChild)
			{
				//����������ʱ���ҵ���������С�ڵ�
				//pMin�϶�û����������������GetMin������
				SplayTreeNode* pMin = GetMinInRight(pST->RChild);
				if (pMin == pST->RChild)
				{
					//���������ڵ�Ϊ��С�ڵ�ʱ������ɾ���ڵ��������ƶ����Ҹ��ڵ�����ӣ��Ҹ��ڵ�Ϊ�¸�
					pMin->LChild = pST->LChild;
					if(pMin->LChild)	//���жϽڵ㲻Ϊ���ٽ��в���
						pMin->LChild->parent = pMin;
					free(pST);
					pST = pMin;
				}
				else
				{
					//���������ڵ㲻����С�ڵ�ʱ��ʹ����С�ڵ��ֵ�滻���ڵ��ֵ��Ȼ��ɾ����С�ڵ�
					pST->key = pMin->key;
					pMin->parent->LChild = pMin->RChild;
					if (pMin->RChild)	//���жϽڵ㲻Ϊ���ٽ��в���
						pMin->RChild->parent = pMin->parent;
					free(pMin);
				}
			}
			else
			{
				//������������ʱ��ֱ�ӽ����ÿգ�������Ϊԭ����������
				SplayTreeNode* ptemp = pST->LChild;
				if(pST->LChild)	//���жϽڵ㲻Ϊ���ٽ��в���
					pST->LChild->parent = NULL;
				free(pST);
				pST = ptemp;
			}
		}

		if (pST)
			pST->parent = NULL;
	}
}

void CopyTreeSplayTree(PSplayTree& pST_s, PSplayTree& pST_d)
{
	if (NULL != pST_s)
	{
		MakeSplayTreeNode(pST_d, pST_s->key);
		CopyTreeSplayTree(pST_s->LChild, pST_d->LChild);
		CopyTreeSplayTree(pST_s->RChild, pST_d->RChild);
		if (pST_d->LChild)
			pST_d->LChild->parent = pST_d;
		if (pST_d->RChild)
			pST_d->RChild->parent = pST_d;
	}
}

void MakeSplayTreeNode(PSplayTree& pST, KeyType key)
{
	pST = (PSplayTree)malloc(sizeof(SplayTreeNode));
	pST->key = key;
	pST->LChild = pST->RChild = pST->parent = NULL;
}

void PrintSplayTree(PSplayTree& pST)
{
	if (NULL == pST)
		return;
	int depth = SplayTreeNodeDepth(pST);
	//ÿ�����һ��Queue����¼�ò�����нڵ�ָ�룬����NULL�ڵ�
	PQueue* ppq = (PQueue*)malloc(sizeof(PQueue)*(depth+1));
	for (size_t level = 0; level < depth+1; level++)
	{
		ppq[level] = NULL;
		InitQueue(ppq[level]);
	}

	int level = 0;
	//����������õ�ÿ��Ľڵ�ָ��
	PreOrderGetSplayTreeLevelNode(pST, level, depth, ppq);
	for (size_t level = 0; level < depth; level++)
	{
		//ÿ����������֦����ʾ������ϵ
		if(0!=level)
			PrintBranchSplayTree(level, depth);
		//��ǰ��ĸ߶�
		int high = depth - level;
		//��ǰ��Ԫ��ֱ�ӵĿո�����
		int spacecount = pow(2, high) - 1;
		//�����ǰ���һ��Ԫ��ǰ�Ŀո�
		PrintfSpaceSplayTree(spacecount/2);
		while (!IsEmptyQueue(ppq[level]))
		{
			PSplayTree pST = (PSplayTree)DelQueue(ppq[level]);
			if (NULL == pST)
				PrintfSplayTreeNullNode();
			else
				PrintfSplayTreeNode(pST);
			//���Ԫ�ؼ�Ŀո�
			PrintfSpaceSplayTree(spacecount);

		}
		//�ò�������������л���
		PrintEndLineSplayTree();
	}
}

//�ο�PrintSplayTreeע��
void PrintSplayTreeNoNull(PSplayTree& pST)
{
	if (NULL == pST)
		return;
	PrintEndLineSplayTree();
	int depth = SplayTreeNodeDepth(pST);
	PQueue* ppq = (PQueue*)malloc(sizeof(PQueue)*(depth + 1));
	for (size_t level = 0; level < depth + 1; level++)
	{
		ppq[level] = NULL;
		InitQueue(ppq[level]);
	}

	int level = 0;
	PreOrderGetSplayTreeLevelNode(pST, level, depth, ppq);
	for (size_t level = 0; level < depth; level++)
	{
		if (0 != level)
			PrintBranchSplayTree(level, depth, ppq);
		int high = depth - level;
		int spacecount = pow(2, high) - 1;
		PrintfSpaceSplayTree(spacecount / 2);
		while (!IsEmptyQueue(ppq[level]))
		{
			PSplayTree pST = (PSplayTree)DelQueue(ppq[level]);
			if (NULL == pST)
				PrintfSplayTreeNullNodeNoNull();
			else
				PrintfSplayTreeNode(pST);
			PrintfSpaceSplayTree(spacecount);

		}

		PrintEndLineSplayTree();
	}
	PrintEndLineSplayTree();
}

void PrintBranchSplayTree(int level, int depth)
{
	int n = depth - level;
	int spacecount = pow(2, n) - 1;
	//cout << endl;
	PrintfSpaceSplayTree(spacecount / 2);
	int kuohao_count = pow(2, level-1);
	while (kuohao_count-- > 0)
	{
		cout << setw(NODE_CHAR_WIDE) << "/";
		PrintfSpaceSplayTree(spacecount);
		cout << setw(NODE_CHAR_WIDE) << "\\ ";
		PrintfSpaceSplayTree(spacecount);
	}
	PrintEndLineSplayTree();
}

void PrintBranchSplayTree(int level, int depth, PQueue* ppq)
{
	int high = depth - level;
	int spacecount = pow(2, high) - 1;
	cout << endl;
	PrintfSpaceSplayTree(spacecount / 2);
	int kuohao_count = pow(2, level - 1);
	PQueue pqnew = NULL;
	CopyQueue(ppq[level], pqnew);
	while (!IsEmptyQueue(pqnew))
	{
		PSplayTree pST = (PSplayTree)DelQueue(pqnew);
		if (NULL==pST)
			cout << setw(NODE_CHAR_WIDE) << " ";
		else
			cout << setw(NODE_CHAR_WIDE) << "/";
		PrintfSpaceSplayTree(spacecount);

		pST = (PSplayTree)DelQueue(pqnew);
		if (NULL == pST)
			cout << setw(NODE_CHAR_WIDE) << " ";
		else
			cout << setw(NODE_CHAR_WIDE) << "\\ ";
		PrintfSpaceSplayTree(spacecount);
	}

	cout << endl;
}

void PrintEndLineSplayTree()
{
	cout << endl;
}

void PrintfSpaceSplayTree(int count)
{
	for (size_t i = 0; i < count; i++)
	{
		for (size_t i = 0; i < NODE_CHAR_WIDE; i++)
		{
			cout << " ";
		}
	}
}

void PrintfSplayTreeNullNode()
{
	cout << setw(NODE_CHAR_WIDE) << "- ";
}

void PrintfSplayTreeNullNodeNoNull()
{
	cout << setw(NODE_CHAR_WIDE) << " ";
}

void PrintfSplayTreeNode(PSplayTree& pST)
{
	cout << setw(NODE_CHAR_WIDE) << pST->key;
}
void PreOrderGetSplayTreeLevelNode(PSplayTree pST, int level, int depth, PQueue* ppq)
{
	//����������ȣ����涼ΪNULLҶ�ӽڵ���
	if (level > depth)
		return;

	//ÿ��ֱ��¼�ڵ���Ϣ
	EnQueue(ppq[level], pST);
	if (pST)
	{
		PreOrderGetSplayTreeLevelNode(pST->LChild, level + 1, depth, ppq);
		PreOrderGetSplayTreeLevelNode(pST->RChild, level + 1, depth, ppq);
	}
	else
	{
		//���û�е�����㣬��NULLָ��ҲҪ��¼����Ϊ��¼����NULLָ���ڸò��λ��
		PreOrderGetSplayTreeLevelNode(NULL, level + 1, depth, ppq);
		PreOrderGetSplayTreeLevelNode(NULL, level + 1, depth, ppq);
	}
}

int SplayTreeNodeDepth(PSplayTree& pST)
{
	if (NULL == pST)
	{
		return 0;
	}

	return max_int(SplayTreeNodeDepth(pST->LChild), SplayTreeNodeDepth(pST->RChild)) + 1;
}

PSplayTree SearchSplayTreeIn(PSplayTree& pST, KeyType key, PSplayTree& hot)
{
	if (NULL == pST)
		return NULL;

	if (pST->key == key)
		return pST;
	
	hot = pST;
	if (key < pST->key)
		return SearchSplayTreeIn(pST->LChild, key, hot);
	else
		return SearchSplayTreeIn(pST->RChild, key, hot);
}

int max_int(int x, int y)
{
	return (x > y) ? x : y;
}

void TestSplayTree()
{
	printf("------SplayTree ����-----\n");
	//�����ַ����ʽ������SplayTree�������������
	PSplayTree root = NULL;
	char * pstr = "45, 24, 53, 12, 90, 28, 25";
	CreateSplayTree(root, pstr);
	printf("InOrder SplayTree:\n");
	InOrderSplayTree(root);
	printf("\n");
	PrintSplayTreeNoNull(root);

	KeyType key = 53;
	SearchSplayTree(root, key);
	if (key == root->key)
		printf("find key %d, %d\n", key, root->key);
	else
		printf("find no key %d\n", key);
	PrintSplayTreeNoNull(root);

	//����������ݲ��ԣ������ظ�����
	vector<int> record;
	vector<int>::iterator it;
	record.clear();
	int maxkey = 32;
	printf("Random Insert Node :\n", maxkey);
	srand((unsigned)time(NULL));
	maxkey = 49;
	int minkey = 1;
	DestorySplayTree(root);
	for (int i = 0; i < 6; i++)
	{
		key = (rand() % (maxkey - minkey + 1)) + minkey;
		printf("����%d\n", key);
		InsertSplayTree(root, key);
		record.push_back(key);
		PrintSplayTreeNoNull(root);
	}
	printf("\n--------------------\n\n");

	//�����������ݣ������ظ�����
	it = record.begin();
	for (; it != record.end(); ++it)
	{
		key = *it;
		printf("����%d\n", key);
		SearchSplayTree(root, key);
		if (key == root->key)
			printf("find key %d, %d\n", key, root->key);
		else
			printf("find no key %d\n", key);
		PrintSplayTreeNoNull(root);
	}
	printf("\n--------------------\n\n");

	//ɾ���������ݲ��ԣ������ظ�ɾ��
	it = record.begin();
	for (; it != record.end(); ++it)
	{
		printf("ɾ��%d\n", *it);
		DeleteSplayTree(root, *it);
		PrintSplayTreeNoNull(root);
	}
	printf("\n--------------------\n\n");

	//������������
	DestorySplayTree(root);
	record.clear();
	for (int i=1; i<7; ++i)
	{
		InsertSplayTree(root, i);
		record.push_back(i);
	}
	printf("������\n");
	PrintSplayTreeNoNull(root);

	//����������������
	it = record.begin();
	for (; it != record.end(); ++it)
	{
		key = *it;
		printf("����%d\n", key);
		SearchSplayTree(root, key);
		if (key == root->key)
			printf("find key %d, %d\n", key, root->key);
		else
			printf("find no key %d\n", key);
		PrintSplayTreeNoNull(root);
	}

}