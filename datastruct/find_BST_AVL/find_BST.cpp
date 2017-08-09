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

/*  由于二叉排序树是一个一直往下层遍历的树，不用记录父节点轨迹，
*	所以可以很方便的使用循环进行查找*/
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
		//查找节点，并将指向此节点的父的儿子指针的地址传入到删除函数
		//不能传递节点指针地址，那样不能更改父的儿子指针指向的内容。
		if ((*p)->key == key)
			return DeleteBST(p);
		if (key < (*p)->key)
			p = &((*p)->LChild);
		else
			p = &((*p)->RChild);
	}

	//以下写法错误
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
		//被删除节点左右儿子均为NULL，则直接删除此节点
		free((*ppbst));
		(*ppbst) = NULL;
	}
	else if (NULL == (*ppbst)->LChild)
	{
		//被删除节点左儿子为NULL，则指向被删除节点的指针指向其右儿子
		q = (*ppbst);
		*ppbst = ((*ppbst)->RChild);
		free(q);
	}
	else if (NULL == (*ppbst)->RChild)
	{
		//被删除节点右儿子为NULL，则指向被删除节点的指针指向其左儿子
		q = (*ppbst);
		*ppbst = ((*ppbst)->LChild);
		free(q);
	}
	else
	{
		//被删除节点的左右儿子均不为NULL，则找被删除节点的前驱节点
		PBSTree pre = (*ppbst);		//记录前驱结点的父节点
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
			//根据中序前驱节点的性质，前驱节点必定没有右儿子。
			//如果前驱节点无左儿子，则直接用前驱节点替换被删除节点，删除前驱节点即可。
			(*ppbst)->key = q->key;
			(*ppbst)->LChild = NULL;
			free(q);
		}
		else
		{
			//前驱节点存在左儿子
			if (pre == (*ppbst))
			{
				//前驱节点是被删节点的直接左儿子
				//被删除节点被其前驱节点替代
				(*ppbst)->LChild = q->LChild;
				(*ppbst)->key = q->key;
				free(q);
			}
			else
			{
				//前驱节点是被删节点的直接左儿子的某个绝对右儿孙
				//被删除节点被其前驱节点替代，将前驱节点的左孩子挂接到新节点的右孩子上
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
	//每层分配一个Queue，记录该层的所有节点指针，包含NULL节点
	PQueue* ppq = (PQueue*)malloc(sizeof(PQueue)*(depth+1));
	for (size_t level = 0; level < depth+1; level++)
	{
		ppq[level] = NULL;
		InitQueue(ppq[level]);
	}

	int level = 0;
	//先序遍历，得到每层的节点指针
	PreOrderGetBSTLevelNode(bt, level, depth, ppq);
	for (size_t level = 0; level < depth; level++)
	{
		//每层后面输出树枝，表示归属关系
		if(0!=level)
			PrintBranchBST(level, depth);
		//当前层的高度
		int high = depth - level;
		//当前层元素直接的空格数量
		int spacecount = pow(2, high) - 1;
		//输出当前层第一个元素前的空格
		PrintfSpaceBST(spacecount/2);
		while (!IsEmptyQueue(ppq[level]))
		{
			PBSTree pbst = (PBSTree)DelQueue(ppq[level]);
			if (NULL == pbst)
				PrintfBSTNullNode();
			else
				PrintfBSTNode(pbst);
			//输出元素间的空格
			PrintfSpaceBST(spacecount);

		}
		//该层输出结束，进行换行
		PrintEndLineBST();
	}
}

//参考PrintBST注释
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
		//普通方式输出
		PrintfSpaceBST(spacecount);
		cout << setw(NODE_CHAR_WIDE) << "\\ ";

		//箭头方式输出
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
	//超过树的深度，后面都为NULL叶子节点了
	if (level > depth)
		return;

	//每层分别记录节点信息
	EnQueue(ppq[level], bt);
	if (bt)
	{
		PreOrderGetBSTLevelNode(bt->LChild, level + 1, depth, ppq);
		PreOrderGetBSTLevelNode(bt->RChild, level + 1, depth, ppq);
	}
	else
	{
		//如果没有到最深层，则NULL指针也要记录，因为记录的是NULL指针在该层的位置
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
	//创建二叉排序树，并中序输出
	PBSTree root = NULL;
	char * pstr = "45, 24, 53, 12, 90, 28, 25";
	CreateBST(root, pstr);
	printf("InOrder BST:\n");
	InOrderBST(root);
	printf("\n");
	PrintBST(root);
	PrintBSTNoNull(root);

	//递归查找算法
	KeyType key = 28;
	PBSTree pfind = SearchBST_Recursion(root, key);
	if (NULL != pfind)
		printf("find key %d, %d\n", key, pfind->key);
	else
		printf("find no key %d\n", key);

	//循环查找算法
	key = 24;
	pfind = SearchBST(root, key);
	if (NULL != pfind)
		printf("find key %d, %d\n", key, pfind->key);
	else
		printf("find no key %d\n", key);

	//删除24节点
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

	//删除53节点
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

	//删除45节点
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