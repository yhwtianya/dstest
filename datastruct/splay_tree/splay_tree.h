#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue_ds.h"
using namespace DS;

/*  伸展树的出发点是这样的：考虑到局部性原理（刚被访问的内容下次可能仍会被访问，
查找次数多的内容可能下一次会被访问），为了使整个查找时间更小，被查频率高的那些节点应当经常处于靠近树根的位置。
这样，很容易得想到以下这个方案：每次查找节点之后对树进行重构，把被查找的节点搬移到树根，
这种自调整形式的二叉查找树就是伸展树。每次对伸展树进行操作后，它均会通过旋转的方法把被访问节点旋转到树根的位置。
为了将当前被访问节点旋转到树根，我们通常将节点自底向上旋转，直至该节点成为树根为止。
“旋转”的巧妙之处就是在不打乱数列中数据大小关系（指中序遍历结果是全序的）情况下，所有基本操作的平摊复杂度仍为O（log n）
*/

/*
对于没有局部性的随机数据来说，Splay能达到AVL的速度，对于有局部性的访问来说，效率会大幅度提升。
比如总数据有m个，经常访问的数据为k个，k远小于m，利用伸展树
那么访问的时间复杂度就和k非常紧密，即O（log k）。
磁盘的文件访问就是基于数据访问的局部性，根据用户习惯，经常访问的文件会更快速的找到。
输入法中，经常输入的词汇会被优先提示

Splay不能保证树的平衡，也不保证单次最坏情况的出现，不适合单次访问效率敏感的场合
*/

typedef int KeyType;
typedef void* POtherInfo;
typedef struct SplayTreeNode
{
	KeyType key;
	struct SplayTreeNode* parent;		//相对于AVL增加了父节点，取消了平衡因子
	struct SplayTreeNode* LChild;
	struct SplayTreeNode* RChild;
}SplayTreeNode, *PSplayTree;

//插入、创建SplayTree树节点
void InsertSplayTree(PSplayTree& pST, KeyType& key);
//销毁SplayTree树
void DestorySplayTree(PSplayTree& pST);
//根据字符串表达式创建SplayTree树
void CreateSplayTree(PSplayTree& pST, char* str);
//访问节点
void VisitSplayTree(PSplayTree& pST);
//中序遍历SplayTree树
//SplayTree树中序遍历总是得到SplayTree树节点从小到达的输出序列，这是BST树的特性
void InOrderSplayTree(PSplayTree& pST);
//SplayTree树的查找，查找过程会对查找节点或最匹配的节点进行翻转，将其翻转到根
//需要再次判断根节点的key是否与key相同
void SearchSplayTree(PSplayTree& pST, KeyType key);
//SplayTree树的查找，仅查找，不翻转。返回值为查找节点或最匹配的节点
//需要再次判断返回值节点的key是否与key相同
SplayTreeNode* SearchSplayTreeNoSplay(PSplayTree& pTree, KeyType key);
//内部查找，不会进行翻转，返回值为查找节点或最匹配的节点
PSplayTree SearchSplayTreeIn(PSplayTree& pST, KeyType key, PSplayTree& hot);
//将节点附加到左儿子
void AttachAsLChild(SplayTreeNode* pBase, SplayTreeNode* pNode);
//将节点附加到右儿子
void AttachAsRChild(SplayTreeNode* pBase, SplayTreeNode* pNode);
//翻转节点到根部，这是最重要的步骤
void Splay(PSplayTree& pNode);
//根据key删除节点，同时将最匹配节点翻转到根部
void DeleteSplayTree(PSplayTree& pST, KeyType key);
//复制伸展树
void CopyTreeSplayTree(PSplayTree& pST_s, PSplayTree& pST_d);
//生成新节点
void MakeSplayTreeNode(PSplayTree& pST, KeyType key);

//以树状结构输出树，NULL也进行输出
void PrintSplayTree(PSplayTree& pST);
//以树状结构输出树，NULL不进行输出
void PrintSplayTreeNoNull(PSplayTree& pST);
//根据层数，输出所有树枝标记，无论是否是NULL节点
void PrintBranchSplayTree(int level, int depth);
//根据层数，输出非NULL节点的树枝标记
void PrintBranchSplayTree(int level, int depth, PQueue* ppq);
//输出换行
void PrintEndLineSplayTree();
//输出空格，其实是按每个节点占用的宽度输出空格，即NODE_CHAR_WIDE
//总空格数为count*NODE_CHAR_WIDE
void PrintfSpaceSplayTree(int count);
//NULL节点输出标记
void PrintfSplayTreeNullNode();
//NULL节点仅输出空格
void PrintfSplayTreeNullNodeNoNull();
//输出节点信息
void PrintfSplayTreeNode(PSplayTree& pST);
int max_int(int x, int y);
//先序遍历，获得每层节点信息
void PreOrderGetSplayTreeLevelNode(PSplayTree pST, int level, int depth, PQueue* ppq);
//节点在整个树中的深度
int SplayTreeNodeDepth(PSplayTree& pST);

void TestSplayTree();