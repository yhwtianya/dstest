#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue_ds.h"
using namespace DS;

/*  AVL树简称二叉平衡树，定义：
	1 可以是空树。
	2 假如不是空树，任何一个结点的左子树与右子树都是平衡二叉树，并且高度之差的绝对值不超过1
	AVL树通过在插入或删除节点时，由底向上层层调整最小不平衡树的平衡来维护平衡二叉树的平衡。
	调整最小不平衡树，是通过旋转节点来完成的。不平衡的状态：LL、LR、RR、RL。
	LL和RR仅需旋转一次即可调整本层最小不平衡树的平衡，LR、RL需要先对最小不平衡树的子树旋转一次，
	使最小不平衡树形成LL或RR，然后再进行一次旋转，即进行两次旋转可调整本层最小不平衡树的平衡。
	调节完本层最小不平衡树，需要更新最小不平衡树的平衡因子，方便上层进行平衡判断和平衡调节。
*/

typedef int KeyType;
typedef void* POtherInfo;
typedef struct AVLNode
{
	KeyType key;
	struct AVLNode* LChild;
	struct AVLNode* RChild;
	int high;				//节点在其最小子树的高度，用于计算平衡因子
}AVLNode, *PAVL;

//插入、创建AVL树节点
void InsertAVL(PAVL& pavl, KeyType& key);
//销毁AVL树
void DestoryAVL(PAVL& pavl);
//根据字符串表达式创建AVL树
void CreateAVL(PAVL& pavl, char* str);
//访问节点
void VisitAVL(PAVL& pavl);
//中序遍历AVL树
//AVL树中序遍历总是得到AVL树节点从小到达的输出序列，这是AVL树的特性
void InOrderAVL(PAVL& pavl);
//基于递归实现AVL树的查找
AVLNode* SearchAVL_Recursion(PAVL& pavl, KeyType key);
//基于循环实现AVL树的查找
AVLNode* SearchAVL(PAVL& pavl, KeyType key);
//删除AVL树节点
void DeleteAVL(PAVL* ppavl);
//bool DeleteAVL(PAVL* pavl);
//根据key查找并删除对应节点
void DeleteAVL(PAVL* ppavl, KeyType key);
//bool DeleteAVL(PAVL& pavl, KeyType key);
void CopyTreeAVL(PAVL& pavl_s, PAVL& pavl_d);
void MakeAVLNode(PAVL& pavl, KeyType key);

//以树状结构输出树，NULL也进行输出
void PrintAVL(PAVL& pavl);
//以树状结构输出树，NULL不进行输出
void PrintAVLNoNull(PAVL& pavl);
//根据层数，输出所有树枝标记，无论是否是NULL节点
void PrintBranchAVL(int level, int depth);
//根据层数，输出非NULL节点的树枝标记
void PrintBranchAVL(int level, int depth, PQueue* ppq);
//输出换行
void PrintEndLineAVL();
//输出空格，其实是按每个节点占用的宽度输出空格，即NODE_CHAR_WIDE
//总空格数为count*NODE_CHAR_WIDE
void PrintfSpaceAVL(int count);
//NULL节点输出标记
void PrintfAVLNullNode();
//NULL节点仅输出空格
void PrintfAVLNullNodeNoNull();
//输出节点信息
void PrintfAVLNode(PAVL& pavl);
int max_int(int x, int y);
//先序遍历，获得每层节点信息
void PreOrderGetAVLLevelNode(PAVL pavl, int level, int depth, PQueue* ppq);
//节点在整个树中的深度
int AVLNodeDepth(PAVL& pavl);
/*节点在其最小子树的高度
	节点在其最小子树的高度是由节点所属最小子树的叶子节点开始计算的，而节点深度是参考节点在整个树的层次来进行计算的。 */
int HighAVL(PAVL& pavl);
//左旋，仅进行一次旋转，适合调整LL型不平衡状态
void SingRotateLeft(PAVL* pavl);
//右旋，仅进行一次旋转，适合调整RR型不平衡状态
void SingRotateRight(PAVL* pavl);
//双旋，适合LR型不平衡状态，左子树先RR旋转使整体形成LL型状态，再进行LL旋转
void DoubleRotateLR(PAVL* pavl);
//双旋，适合RL型不平衡状态，右子树先LL旋转使整体形成RR型状态，再进行RR旋转
void DoubleRotateRL(PAVL* pavl);

void TestSearchAVL();