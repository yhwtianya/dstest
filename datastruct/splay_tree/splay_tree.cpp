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

	//查找时，已经进行了翻转，根节点要么为key的匹配节点，要么为最相近匹配节点
	SearchSplayTree(pST, key);
	if (pST)
	{
		if (pST->key != key)
		{
			//根节点为最相近匹配节点，根据待插入节点与根节点大小关系进行插入，并更新根节点
			SplayTreeNode* pNew = NULL;
			MakeSplayTreeNode(pNew, key);
			if (key < pST->key)
			{
				AttachAsLChild(pNew, pST->LChild);	//将儿子节点移动到新节点
				pST->LChild = NULL;		//儿子节点的剪切赋值一定要成对出现，即使赋值为NULL
				AttachAsRChild(pNew, pST);
			}
			else
			{
				AttachAsRChild(pNew, pST->RChild);	//将儿子节点移动到新节点
				pST->RChild = NULL;		//儿子节点的剪切赋值一定要成对出现，即使赋值为NULL
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
		//注意：这里返回的是匹配节点或最相近匹配节点
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

//绑定左孩子
void AttachAsLChild(SplayTreeNode* pBase, SplayTreeNode* pNode)
{
	//先判断节点不为空再进行操作
	if (pBase)
	{
		pBase->LChild = pNode;
		//先判断节点不为空再进行操作
		if (pNode)
			pNode->parent = pBase;
	}
}

//绑定右孩子
void AttachAsRChild(SplayTreeNode* pBase, SplayTreeNode* pNode)
{
	//先判断节点不为空再进行操作
	if (pBase)
	{
		pBase->RChild = pNode;
		//先判断节点不为空再进行操作
		if (pNode)
			pNode->parent = pBase;
	}
}

//针对LL型进行伸展
SplayTreeNode* ZigZig(SplayTreeNode* v, SplayTreeNode* p, SplayTreeNode* g)
{
	//这里不同于AVL的LL翻转，这里翻转先进行祖父和父节点翻转，才进行本节点和父节点翻转
	//这个不同是SplayTree高效的关键，它会使最坏情况下树的高度减少
	AttachAsLChild(g, p->RChild);	//将儿子节点剪切
	AttachAsRChild(p, g);			//儿子节点重新赋值，儿子节点的剪切赋值一定要成对出现
	AttachAsLChild(p, v->RChild);
	AttachAsRChild(v, p);
	return v;
}

//针对LR型进行伸展
SplayTreeNode* ZigZag(SplayTreeNode* v, SplayTreeNode* p, SplayTreeNode* g)
{
	//和AVL的LR翻转类似
	AttachAsRChild(p, v->LChild);	//将儿子节点剪切
	AttachAsLChild(v, p);			//儿子节点重新赋值，儿子节点的剪切赋值一定要成对出现，即使赋值为NULL
	AttachAsLChild(g, v->RChild);
	AttachAsRChild(v, g);
	return v;
}

//针对RL型进行伸展
SplayTreeNode* ZagZig(SplayTreeNode* v, SplayTreeNode* p, SplayTreeNode* g)
{
	//和AVL的RL翻转类似
	AttachAsLChild(p, v->RChild);
	AttachAsRChild(v, p);
	AttachAsRChild(g, v->LChild);
	AttachAsLChild(v, g);
	return v;
}

//针对RR型进行伸展
SplayTreeNode* ZagZag(SplayTreeNode* v, SplayTreeNode* p, SplayTreeNode* g)
{
	//这里不同于AVL的RR翻转，这里翻转先进行祖父和父节点翻转，才进行本节点和父节点翻转
	//这个不同是SplayTree高效的关键，它会使最坏情况下树的高度减少
	AttachAsRChild(g, p->LChild);
	AttachAsLChild(p, g);
	AttachAsRChild(p, v->LChild);
	AttachAsLChild(v, p);
	return v;
}

//针对L型进行伸展
void Zig(SplayTreeNode* v, SplayTreeNode* p)
{
	AttachAsLChild(p, v->RChild);	//将儿子节点剪切
	AttachAsRChild(v, p);			//儿子节点的剪切赋值一定要成对出现，即使赋值为NULL
}

//针对R型进行伸展
void Zag(SplayTreeNode* v, SplayTreeNode* p)
{
	AttachAsRChild(p, v->LChild);
	AttachAsLChild(v, p);
}

void Splay(PSplayTree& pNode)
{
	SplayTreeNode* p = NULL;	//父节点
	SplayTreeNode* g = NULL;	//祖父节点

	//祖父和父节点都存在时，进行伸展树的双层翻转，直到父节点或祖父节点为NULL停止双层翻转
	while ((p=pNode->parent) && (g=p->parent))
	{
		SplayTreeNode* gg = g->parent;	//曾祖父节点
		//根据祖父、父、操作节点的对应关系进行不同的翻转
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

		//根据曾祖父与祖父的关系，更新新根的父节点
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

	//父节点存在且祖父节点为NULL，则进行单层翻转，将本节点翻转到根部
	if (p=pNode->parent)
	{
		//根据本节点和父节点关系进行单层翻转
		if (pNode==p->LChild)
		{
			Zig(pNode, p);
		}
		else
		{
			Zag(pNode, p);
		}
	}

	//根节点的父置为NULL
	pNode->parent = NULL;
}

SplayTreeNode* GetMinInRight(PSplayTree pTree)
{
	//获取右子树的最小节点
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
		//查找时，已经进行了翻转，根节点要么为待删除节点，要么为最相近匹配节点
		SearchSplayTree(pST, key);
		if (pST && (key==pST->key))
		{
			if (pST->RChild)
			{
				//存在右子树时，找到右子树最小节点
				//pMin肯定没有左子树，这是由GetMin决定的
				SplayTreeNode* pMin = GetMinInRight(pST->RChild);
				if (pMin == pST->RChild)
				{
					//右子树根节点为最小节点时，将待删除节点左子树移动到右根节点的左孩子，右根节点为新根
					pMin->LChild = pST->LChild;
					if(pMin->LChild)	//先判断节点不为空再进行操作
						pMin->LChild->parent = pMin;
					free(pST);
					pST = pMin;
				}
				else
				{
					//右子树根节点不是最小节点时，使用最小节点的值替换根节点的值，然后删除最小节点
					pST->key = pMin->key;
					pMin->parent->LChild = pMin->RChild;
					if (pMin->RChild)	//先判断节点不为空再进行操作
						pMin->RChild->parent = pMin->parent;
					free(pMin);
				}
			}
			else
			{
				//不存在右子树时，直接将根置空，根更新为原来根的左孩子
				SplayTreeNode* ptemp = pST->LChild;
				if(pST->LChild)	//先判断节点不为空再进行操作
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
	//每层分配一个Queue，记录该层的所有节点指针，包含NULL节点
	PQueue* ppq = (PQueue*)malloc(sizeof(PQueue)*(depth+1));
	for (size_t level = 0; level < depth+1; level++)
	{
		ppq[level] = NULL;
		InitQueue(ppq[level]);
	}

	int level = 0;
	//先序遍历，得到每层的节点指针
	PreOrderGetSplayTreeLevelNode(pST, level, depth, ppq);
	for (size_t level = 0; level < depth; level++)
	{
		//每层后面输出树枝，表示归属关系
		if(0!=level)
			PrintBranchSplayTree(level, depth);
		//当前层的高度
		int high = depth - level;
		//当前层元素直接的空格数量
		int spacecount = pow(2, high) - 1;
		//输出当前层第一个元素前的空格
		PrintfSpaceSplayTree(spacecount/2);
		while (!IsEmptyQueue(ppq[level]))
		{
			PSplayTree pST = (PSplayTree)DelQueue(ppq[level]);
			if (NULL == pST)
				PrintfSplayTreeNullNode();
			else
				PrintfSplayTreeNode(pST);
			//输出元素间的空格
			PrintfSpaceSplayTree(spacecount);

		}
		//该层输出结束，进行换行
		PrintEndLineSplayTree();
	}
}

//参考PrintSplayTree注释
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
	//超过树的深度，后面都为NULL叶子节点了
	if (level > depth)
		return;

	//每层分别记录节点信息
	EnQueue(ppq[level], pST);
	if (pST)
	{
		PreOrderGetSplayTreeLevelNode(pST->LChild, level + 1, depth, ppq);
		PreOrderGetSplayTreeLevelNode(pST->RChild, level + 1, depth, ppq);
	}
	else
	{
		//如果没有到最深层，则NULL指针也要记录，因为记录的是NULL指针在该层的位置
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
	printf("------SplayTree 测试-----\n");
	//根据字符表达式，创建SplayTree树，并中序输出
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

	//随机插入数据测试，包含重复插入
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
		printf("插入%d\n", key);
		InsertSplayTree(root, key);
		record.push_back(key);
		PrintSplayTreeNoNull(root);
	}
	printf("\n--------------------\n\n");

	//查找所有数据，包含重复查找
	it = record.begin();
	for (; it != record.end(); ++it)
	{
		key = *it;
		printf("查找%d\n", key);
		SearchSplayTree(root, key);
		if (key == root->key)
			printf("find key %d, %d\n", key, root->key);
		else
			printf("find no key %d\n", key);
		PrintSplayTreeNoNull(root);
	}
	printf("\n--------------------\n\n");

	//删除所有数据测试，包含重复删除
	it = record.begin();
	for (; it != record.end(); ++it)
	{
		printf("删除%d\n", *it);
		DeleteSplayTree(root, *it);
		PrintSplayTreeNoNull(root);
	}
	printf("\n--------------------\n\n");

	//构造最坏情况测试
	DestorySplayTree(root);
	record.clear();
	for (int i=1; i<7; ++i)
	{
		InsertSplayTree(root, i);
		record.push_back(i);
	}
	printf("最坏情况：\n");
	PrintSplayTreeNoNull(root);

	//最坏情况查找所有数据
	it = record.begin();
	for (; it != record.end(); ++it)
	{
		key = *it;
		printf("查找%d\n", key);
		SearchSplayTree(root, key);
		if (key == root->key)
			printf("find key %d, %d\n", key, root->key);
		else
			printf("find no key %d\n", key);
		PrintSplayTreeNoNull(root);
	}

}