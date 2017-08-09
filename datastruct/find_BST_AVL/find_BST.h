#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue_ds.h"
using namespace DS;
/*  基于树的查找法的查找法，也称树表达查找方法
*	将待查表组织成特定树，在树结构上实现查找 */

/*	二叉树排序树或者是一棵空树，或者是具有如下性质的二叉树：
	1 若它的左子树非空，则左子树上所有结点的值均小于根结点的值；
	2 若它的右子树非空，则右子树上所有结点的值均大于（或大于等于）根结点的值；
	3 它的左右子树也分别为二叉排序树	*/

typedef int KeyType;
typedef void* POtherInfo;
typedef struct BSTreeNode
{
	KeyType key;
	struct BSTreeNode* LChild;
	struct BSTreeNode* RChild;
}BSTreeNode, *PBSTree;

//插入二叉排序树节点
void InsertBST(PBSTree& pbst, KeyType& key);
//创建二叉排序树
void CreateBST(PBSTree& pbst, char* str);
//访问节点
void VisitBST(PBSTree& pbst);
//中序遍历二叉排序树
//二叉排序树中序遍历总是得到二叉排序树节点从小到达的输出序列，这是二叉排序树的特性
void InOrderBST(PBSTree& pbst);
//基于递归实现二叉排序树的查找
BSTreeNode* SearchBST_Recursion(PBSTree& pbst, KeyType key);
//基于循环实现二叉排序树的查找
BSTreeNode* SearchBST(PBSTree& pbst, KeyType key);
//删除二叉排序树节点
bool DeleteBST(PBSTree* pbst);
//根据key查找并删除对应节点
bool DeleteBST(PBSTree& pbst, KeyType key);
void CopyTreeBST(PBSTree& bt_s, PBSTree& bt_d);
void MakeBSTNode(PBSTree& bt, KeyType key);

//以树状结构输出树，NULL也进行输出
void PrintBST(PBSTree& bt);
//以树状结构输出树，NULL不进行输出
void PrintBSTNoNull(PBSTree& bt);
//根据层数，输出所有树枝标记，无论是否是NULL节点
void PrintBranchBST(int level, int depth);
//根据层数，输出非NULL节点的树枝标记
void PrintBranchBST(int level, int depth, PQueue* ppq);
//输出换行
void PrintEndLineBST();
//输出空格，其实是按每个节点占用的宽度输出空格，即NODE_CHAR_WIDE
//总空格数为count*NODE_CHAR_WIDE
void PrintfSpaceBST(int count);
//NULL节点输出标记
void PrintfBSTNullNode();
//NULL节点仅输出空格
void PrintfBSTNullNodeNoNull();
//输出节点信息
void PrintfBSTNode(PBSTree& pbst);
int maxtreeBST(int x, int y);
//先序遍历，获得每层节点信息
void PreOrderGetBSTLevelNode(PBSTree bt, int level, int depth, PQueue* ppq);
int BSTNodeDepth(PBSTree& bt);
void TestSearchBST();