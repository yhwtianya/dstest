#include "find_BST.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue_ds.h"
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;
#define NODE_CHAR_WIDE		2
using namespace DS;

void InsertBST(PBSTree& pbst, KeyType& key)
{
	if (NULL == pbst)
	{
		pbst = (PBSTree)malloc(sizeof(BSTreeNode));
		pbst->key = key;
		pbst->LChild = pbst->RChild = NULL;
	}
	else if (key < pbst->key)
	{
		InsertBST(pbst->LChild, key);
	}
	else
	{
		InsertBST(pbst->RChild, key);
	}
}

void CreateBST(PBSTree& pbst, char* str)
{
	for (size_t i = 0; i < strlen(str); i++)
	{
		if (str[i] != ',' && str[i] != ' ')
		{
			KeyType key = atoi(&str[i]);
			InsertBST(pbst, key);
			while (i < strlen(str) && str[++i] != ',');
		}
	}
}

void VisitBST(PBSTree& pbst)
{
	printf("%d ", pbst->key);
}

void InOrderBST(PBSTree& pbst)
{
	if (NULL != pbst)
	{
		InOrderBST(pbst->LChild);
		VisitBST(pbst);
		InOrderBST(pbst->RChild);
	}
}


BSTreeNode* SearchBST_Recursion(PBSTree& pbst, KeyType key)
{
	if (NULL!=pbst)
	{
		if (pbst->key == key)
			return pbst;

		if (key < pbst->key)
			return SearchBST_Recursion(pbst->LChild, key);
		else
			return SearchBST_Recursion(pbst->RChild, key);
	}

	return NULL;
}

/*  ���ڶ�����������һ��һֱ���²�������������ü�¼���ڵ�켣��
*	���Կ��Ժܷ����ʹ��ѭ�����в���*/
BSTreeNode* SearchBST(PBSTree& pbst, KeyType key)
{
	PBSTree p = pbst;
	while (NULL!=p)
	{
		if (p->key == key)
			return p;
		if (key < p->key)
			p = p->LChild;
		else
			p = p->RChild;
	}

	return p;
}

bool DeleteBST(PBSTree& pbst, KeyType key)
{
	if (NULL==pbst)
		return false;

	PBSTree* p = &pbst;
	while (NULL != p)
	{
		//���ҽڵ㣬����ָ��˽ڵ�ĸ��Ķ���ָ��ĵ�ַ���뵽ɾ������
		//���ܴ��ݽڵ�ָ���ַ���������ܸ��ĸ��Ķ���ָ��ָ������ݡ�
		if ((*p)->key == key)
			return DeleteBST(p);
		if (key < (*p)->key)
			p = &((*p)->LChild);
		else
			p = &((*p)->RChild);
	}

	//����д������
/*	PBSTree p = pbst;
	while (NULL != p)
	{
		if (p->key == key)
			return DeleteBST(&p);
		if (key < p->key)
			p = p->LChild;
		else
			p = p->RChild;
	}	*/

	return false;
}

bool DeleteBST(PBSTree* ppbst)
{
	PBSTree q;
	if (NULL == (*ppbst)->LChild && NULL == (*ppbst)->RChild)
	{
		//��ɾ���ڵ����Ҷ��Ӿ�ΪNULL����ֱ��ɾ���˽ڵ�
		free((*ppbst));
		(*ppbst) = NULL;
	}
	else if (NULL == (*ppbst)->LChild)
	{
		//��ɾ���ڵ������ΪNULL����ָ��ɾ���ڵ��ָ��ָ�����Ҷ���
		q = (*ppbst);
		*ppbst = ((*ppbst)->RChild);
		free(q);
	}
	else if (NULL == (*ppbst)->RChild)
	{
		//��ɾ���ڵ��Ҷ���ΪNULL����ָ��ɾ���ڵ��ָ��ָ���������
		q = (*ppbst);
		*ppbst = ((*ppbst)->LChild);
		free(q);
	}
	else
	{
		//��ɾ���ڵ�����Ҷ��Ӿ���ΪNULL�����ұ�ɾ���ڵ��ǰ���ڵ�
		PBSTree pre = (*ppbst);		//��¼ǰ�����ĸ��ڵ�
		q = (*ppbst)->LChild;
		while (NULL != q)
		{
			if (q->RChild)
			{
				pre = q;
				q = q->RChild;
			}
			else
				break;
		}

		if (NULL == q->LChild)
		{
			//��������ǰ���ڵ�����ʣ�ǰ���ڵ�ض�û���Ҷ��ӡ�
			//���ǰ���ڵ�������ӣ���ֱ����ǰ���ڵ��滻��ɾ���ڵ㣬ɾ��ǰ���ڵ㼴�ɡ�
			(*ppbst)->key = q->key;
			(*ppbst)->LChild = NULL;
			free(q);
		}
		else
		{
			//ǰ���ڵ���������
			if (pre == (*ppbst))
			{
				//ǰ���ڵ��Ǳ�ɾ�ڵ��ֱ�������
				//��ɾ���ڵ㱻��ǰ���ڵ����
				(*ppbst)->LChild = q->LChild;
				(*ppbst)->key = q->key;
				free(q);
			}
			else
			{
				//ǰ���ڵ��Ǳ�ɾ�ڵ��ֱ������ӵ�ĳ�������Ҷ���
				//��ɾ���ڵ㱻��ǰ���ڵ��������ǰ���ڵ�����ӹҽӵ��½ڵ���Һ�����
				(*ppbst)->key = q->key;
				pre->RChild = q->LChild;
				free(q);
			}
		}
	}

	return true;
}

void CopyTreeBST(PBSTree& bt_s, PBSTree& bt_d)
{
	if (NULL != bt_s)
	{
		MakeBSTNode(bt_d, bt_s->key);
		CopyTreeBST(bt_s->LChild, bt_d->LChild);
		CopyTreeBST(bt_s->RChild, bt_d->RChild);
	}
}

void MakeBSTNode(PBSTree& bt, KeyType key)
{
	bt = (PBSTree)malloc(sizeof(BSTreeNode));
	bt->key = key;
	bt->LChild = bt->RChild = NULL;
}

void PrintBST(PBSTree& bt)
{
	if (NULL == bt)
		return;
	int depth = BSTNodeDepth(bt);
	//ÿ�����һ��Queue����¼�ò�����нڵ�ָ�룬����NULL�ڵ�
	PQueue* ppq = (PQueue*)malloc(sizeof(PQueue)*(depth+1));
	for (size_t level = 0; level < depth+1; level++)
	{
		ppq[level] = NULL;
		InitQueue(ppq[level]);
	}

	int level = 0;
	//����������õ�ÿ��Ľڵ�ָ��
	PreOrderGetBSTLevelNode(bt, level, depth, ppq);
	for (size_t level = 0; level < depth; level++)
	{
		//ÿ����������֦����ʾ������ϵ
		if(0!=level)
			PrintBranchBST(level, depth);
		//��ǰ��ĸ߶�
		int high = depth - level;
		//��ǰ��Ԫ��ֱ�ӵĿո�����
		int spacecount = pow(2, high) - 1;
		//�����ǰ���һ��Ԫ��ǰ�Ŀո�
		PrintfSpaceBST(spacecount/2);
		while (!IsEmptyQueue(ppq[level]))
		{
			PBSTree pbst = (PBSTree)DelQueue(ppq[level]);
			if (NULL == pbst)
				PrintfBSTNullNode();
			else
				PrintfBSTNode(pbst);
			//���Ԫ�ؼ�Ŀո�
			PrintfSpaceBST(spacecount);

		}
		//�ò�������������л���
		PrintEndLineBST();
	}
}

//�ο�PrintBSTע��
void PrintBSTNoNull(PBSTree& bt)
{
	if (NULL == bt)
		return;
	int depth = BSTNodeDepth(bt);
	PQueue* ppq = (PQueue*)malloc(sizeof(PQueue)*(depth + 1));
	for (size_t level = 0; level < depth + 1; level++)
	{
		ppq[level] = NULL;
		InitQueue(ppq[level]);
	}

	int level = 0;
	PreOrderGetBSTLevelNode(bt, level, depth, ppq);
	for (size_t level = 0; level < depth; level++)
	{
		if (0 != level)
			PrintBranchBST(level, depth, ppq);
		int high = depth - level;
		int spacecount = pow(2, high) - 1;
		PrintfSpaceBST(spacecount / 2);
		while (!IsEmptyQueue(ppq[level]))
		{
			PBSTree pbst = (PBSTree)DelQueue(ppq[level]);
			if (NULL == pbst)
				PrintfBSTNullNodeNoNull();
			else
				PrintfBSTNode(pbst);
			PrintfSpaceBST(spacecount);

		}

		PrintEndLineBST();
	}
}

void PrintfHengXian(int count)
{
	for (size_t i = 0; i < count; i++)
	{
		for (size_t i = 0; i < NODE_CHAR_WIDE; i++)
		{
			cout << "-";
		}
	}
}

void PrintfJianTou(int spacecount)
{
	int half = (spacecount - 1) / 2;
	PrintfHengXian(half);
	cout << "^";
	PrintfHengXian(half);
	int n = 0;
	while (++n < NODE_CHAR_WIDE)
	{
		cout << "-";
	}
	if (spacecount%2 == 1)
	{
		cout << "-";
	}
}

void PrintBranchBST(int level, int depth)
{
	int n = depth - level;
	int spacecount = pow(2, n) - 1;
	//cout << endl;
	PrintfSpaceBST(spacecount / 2);
	int kuohao_count = pow(2, level-1);
	while (kuohao_count-- > 0)
	{
		cout << setw(NODE_CHAR_WIDE) << "/";
		//��ͨ��ʽ���
		PrintfSpaceBST(spacecount);
		cout << setw(NODE_CHAR_WIDE) << "\\ ";

		//��ͷ��ʽ���
		//PrintfJianTou(spacecount);
		//cout << "\\";
		PrintfSpaceBST(spacecount);
	}
	cout << endl;
}

void PrintBranchBST(int level, int depth, PQueue* ppq)
{
	int high = depth - level;
	int spacecount = pow(2, high) - 1;
	//cout << endl;
	PrintfSpaceBST(spacecount / 2);
	int kuohao_count = pow(2, level - 1);
	PQueue pqnew = NULL;
	CopyQueue(ppq[level], pqnew);
	while (!IsEmptyQueue(pqnew))
	{
		PBSTree pbst = (PBSTree)DelQueue(pqnew);
		if (NULL==pbst)
			cout << setw(NODE_CHAR_WIDE) << " ";
		else
			cout << setw(NODE_CHAR_WIDE) << "/";
		PrintfSpaceBST(spacecount);

		pbst = (PBSTree)DelQueue(pqnew);
		if (NULL == pbst)
			cout << setw(NODE_CHAR_WIDE) << " ";
		else
			cout << setw(NODE_CHAR_WIDE) << "\\ ";
		PrintfSpaceBST(spacecount);
	}

	cout << endl;
}

void PrintEndLineBST()
{
	cout << endl;
}

void PrintfSpaceBST(int count)
{
	for (size_t i = 0; i < count; i++)
	{
		for (size_t i = 0; i < NODE_CHAR_WIDE; i++)
		{
			cout << " ";
		}
	}
}

void PrintfBSTNullNode()
{
	cout << setw(NODE_CHAR_WIDE) << "- ";
}

void PrintfBSTNullNodeNoNull()
{
	cout << setw(NODE_CHAR_WIDE) << " ";
}

void PrintfBSTNode(PBSTree& pbst)
{
	cout << setw(NODE_CHAR_WIDE) << pbst->key;
}
void PreOrderGetBSTLevelNode(PBSTree bt, int level, int depth, PQueue* ppq)
{
	//����������ȣ����涼ΪNULLҶ�ӽڵ���
	if (level > depth)
		return;

	//ÿ��ֱ��¼�ڵ���Ϣ
	EnQueue(ppq[level], bt);
	if (bt)
	{
		PreOrderGetBSTLevelNode(bt->LChild, level + 1, depth, ppq);
		PreOrderGetBSTLevelNode(bt->RChild, level + 1, depth, ppq);
	}
	else
	{
		//���û�е�����㣬��NULLָ��ҲҪ��¼����Ϊ��¼����NULLָ���ڸò��λ��
		PreOrderGetBSTLevelNode(NULL, level + 1, depth, ppq);
		PreOrderGetBSTLevelNode(NULL, level + 1, depth, ppq);
	}
}

int BSTNodeDepth(PBSTree& bt)
{
	if (NULL == bt)
	{
		return 0;
	}

	return maxtreeBST(BSTNodeDepth(bt->LChild), BSTNodeDepth(bt->RChild)) + 1;
}

int maxtreeBST(int x, int y)
{
	return (x > y) ? x : y;
}

void TestSearchBST()
{
	//�������������������������
	PBSTree root = NULL;
	char * pstr = "45, 24, 53, 12, 90, 28, 25";
	CreateBST(root, pstr);
	printf("InOrder BST:\n");
	InOrderBST(root);
	printf("\n");
	PrintBST(root);
	PrintBSTNoNull(root);

	//�ݹ�����㷨
	KeyType key = 28;
	PBSTree pfind = SearchBST_Recursion(root, key);
	if (NULL != pfind)
		printf("find key %d, %d\n", key, pfind->key);
	else
		printf("find no key %d\n", key);

	//ѭ�������㷨
	key = 24;
	pfind = SearchBST(root, key);
	if (NULL != pfind)
		printf("find key %d, %d\n", key, pfind->key);
	else
		printf("find no key %d\n", key);

	//ɾ��24�ڵ�
	key = 24;
	PBSTree root1 = NULL;
	CopyTreeBST(root, root1);
	printf("delete %d, after delete:\n", key);
	DeleteBST(root1, key);
	InOrderBST(root1);
	printf("\n");
	pfind = SearchBST(root1, key);
	if (NULL != pfind)
		printf("find key %d, %d\n", key, pfind->key);
	else
		printf("find no key %d\n", key);
	PrintBSTNoNull(root1);

	//ɾ��53�ڵ�
	key = 53;
	PBSTree root2 = NULL;
	CopyTreeBST(root, root2);
	printf("delete %d, after delete:\n", key);
	DeleteBST(root2, key);
	InOrderBST(root2);
	printf("\n");
	pfind = SearchBST(root2, key);
	if (NULL != pfind)
		printf("find key %d, %d\n", key, pfind->key);
	else
		printf("find no key %d\n", key);
	PrintBSTNoNull(root2);

	//ɾ��45�ڵ�
	key = 45;
	PBSTree root3 = NULL;
	CopyTreeBST(root, root3);
	printf("delete %d, after delete:\n", key);
	DeleteBST(root3, key);
	InOrderBST(root3);
	printf("\n");
	pfind = SearchBST(root3, key);
	if (NULL != pfind)
		printf("find key %d, %d\n", key, pfind->key);
	else
		printf("find no key %d\n", key);
	PrintBSTNoNull(root3);
	printf("\n");
	PrintBST(root3);

}