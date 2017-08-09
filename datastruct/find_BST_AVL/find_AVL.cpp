#include "find_AVL.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue_ds.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <time.h>
using namespace std;
#define NODE_CHAR_WIDE		2
using namespace DS;

void InsertAVL(PAVL& pavl, KeyType& key)
{
	if (NULL == pavl)
	{
		MakeAVLNode(pavl, key);
	}
	else if (key == pavl->key)
	{
		return;
	}
	else if (key < pavl->key)
	{
		InsertAVL(pavl->LChild, key);
		if (2 == HighAVL(pavl->LChild)-HighAVL(pavl->RChild))
		{
			if (key < pavl->LChild->key)
				SingRotateLeft(&pavl);
			else
				DoubleRotateLR(&pavl);
		}
	}
	else
	{
		InsertAVL(pavl->RChild, key);
		if (2 == HighAVL(pavl->RChild) - HighAVL(pavl->LChild))
		{
			if (key < pavl->RChild->key)
				DoubleRotateRL(&pavl);
			else
				SingRotateRight(&pavl);
		}
	}

	pavl->high = max_int(HighAVL(pavl->LChild), HighAVL(pavl->RChild)) + 1;
}

void DestoryAVL(PAVL& pavl)
{
	if (NULL!=pavl)
	{
		DestoryAVL(pavl->LChild);
		DestoryAVL(pavl->RChild);
		free(pavl);
		pavl = NULL;
	}
}

void CreateAVL(PAVL& pavl, char* str)
{
	for (size_t i = 0; i < strlen(str); i++)
	{
		if (str[i] != ',' && str[i] != ' ')
		{
			KeyType key = atoi(&str[i]);
			InsertAVL(pavl, key);
			while (i < strlen(str) && str[++i] != ',');
		}
	}
}

void VisitAVL(PAVL& pavl)
{
	printf("%d ", pavl->key);
}

void InOrderAVL(PAVL& pavl)
{
	if (NULL != pavl)
	{
		InOrderAVL(pavl->LChild);
		VisitAVL(pavl);
		InOrderAVL(pavl->RChild);
	}
}

AVLNode* SearchAVL_Recursion(PAVL& pavl, KeyType key)
{
	if (NULL!=pavl)
	{
		if (pavl->key == key)
			return pavl;

		if (key < pavl->key)
			return SearchAVL_Recursion(pavl->LChild, key);
		else
			return SearchAVL_Recursion(pavl->RChild, key);
	}

	return NULL;
}

/*  由于AVL树查找时是一个一直往下层遍历的树，不用记录父节点轨迹，
*	所以可以很方便的使用循环进行查找*/
AVLNode* SearchAVL(PAVL& pavl, KeyType key)
{
	PAVL p = pavl;
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

void DeleteAVL(PAVL* ppavl, KeyType key)
{
	if (NULL == (*ppavl))
		return ;

	/* 由于删除节点后，每个父节点层都需要调整平衡，不能使用循环结构完成删除，因此使用递归 */
	if (key < (*ppavl)->key)
	{
		DeleteAVL(&(*ppavl)->LChild, key);
		//左子树删除节点，只可能造成左子树降低
		if (2 == HighAVL((*ppavl)->RChild) - HighAVL((*ppavl)->LChild))
		{
			//判断是不是RL结构
			if ((*ppavl)->RChild->LChild && HighAVL((*ppavl)->RChild->LChild) > HighAVL((*ppavl)->RChild->RChild))
				DoubleRotateRL(ppavl);
			else
				SingRotateRight(ppavl);
		}
	}
	else if (key > (*ppavl)->key)
	{
		DeleteAVL(&(*ppavl)->RChild, key);
		//右子树删除节点，只可能造成右子树降低
		if (2 == HighAVL((*ppavl)->LChild) - HighAVL((*ppavl)->RChild))
		{
			//判断是不是LR结构
			if ((*ppavl)->LChild->RChild && HighAVL((*ppavl)->LChild->RChild) > HighAVL((*ppavl)->LChild->LChild))
				DoubleRotateLR(ppavl);
			else
				SingRotateLeft(ppavl);
		}
	}
	else
	{
		//递归出口
		DeleteAVL(ppavl);
	}
}

void DeleteAVL(PAVL* ppavl)
{
	//前部分删除节点部分和BSTree删除节点完全一样，主要是后面增加了调整平衡操作
	PAVL q;
	if (NULL == (*ppavl)->LChild && NULL == (*ppavl)->RChild)
	{
		//被删除节点左右儿子均为NULL，则直接删除此节点
		free((*ppavl));
		(*ppavl) = NULL;
	}
	else if (NULL == (*ppavl)->LChild)
	{
		//被删除节点左儿子为NULL，则指向被删除节点的指针指向其右儿子
		q = (*ppavl);
		*ppavl = ((*ppavl)->RChild);
		free(q);
	}
	else if (NULL == (*ppavl)->RChild)
	{
		//被删除节点右儿子为NULL，则指向被删除节点的指针指向其左儿子
		q = (*ppavl);
		*ppavl = ((*ppavl)->LChild);
		free(q);
	}
	else
	{
		//被删除节点的左右儿子均不为NULL，则找被删除节点的前驱节点
		PAVL pre = (*ppavl);		//记录前驱结点的父节点
		q = (*ppavl)->LChild;
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
			(*ppavl)->key = q->key;
			(*ppavl)->LChild = NULL;
			free(q);
		}
		else
		{
			//前驱节点存在左儿子
			if (pre == (*ppavl))
			{
				//前驱节点是被删节点的直接左儿子
				//被删除节点被其前驱节点替代
				(*ppavl)->LChild = q->LChild;
				(*ppavl)->key = q->key;
				free(q);
			}
			else
			{
				//前驱节点是被删节点的直接左儿子的某个绝对右儿孙
				//被删除节点被其前驱节点替代，将前驱节点的左孩子挂接到新节点的右孩子上
				(*ppavl)->key = q->key;
				pre->RChild = q->LChild;
				free(q);
			}
		}
	}

	/*  调整平衡的操作
	*	1 如果被删除节点左右子树为空，删除后此节点平衡
	*	2 如果只有一个子树为空，其实非空子树只能有一层，进行删除后，树的平衡度其实为0了，此节点平衡
	*	3 如果左右子树均不为空，则会选择位于左子树上前驱节点进行替换删除，这时可能造成左子树高度降低，可能需要调节平衡	*/
	if (NULL!=(*ppavl))
	{
		if (2 == HighAVL((*ppavl)->RChild) - HighAVL((*ppavl)->LChild))
		{
			//判断是不是RL结构
			if ((*ppavl)->RChild->LChild && HighAVL((*ppavl)->RChild->LChild) > HighAVL((*ppavl)->RChild->RChild))
				DoubleRotateRL(ppavl);
			else
				SingRotateRight(ppavl);
		}

		//更新最小不平衡子树，调整平衡后顶点高度，方便更高层树进行平衡调整
		(*ppavl)->high = max_int(HighAVL((*ppavl)->LChild), HighAVL((*ppavl)->RChild)) + 1;
	}
}

void CopyTreeAVL(PAVL& pavl_s, PAVL& pavl_d)
{
	if (NULL != pavl_s)
	{
		MakeAVLNode(pavl_d, pavl_s->key);
		pavl_d->high = pavl_s->high;
		CopyTreeAVL(pavl_s->LChild, pavl_d->LChild);
		CopyTreeAVL(pavl_s->RChild, pavl_d->RChild);
	}
}

void MakeAVLNode(PAVL& pavl, KeyType key)
{
	pavl = (PAVL)malloc(sizeof(AVLNode));
	pavl->key = key;
	pavl->high = 0;
	pavl->LChild = pavl->RChild = NULL;
}

void PrintAVL(PAVL& pavl)
{
	if (NULL == pavl)
		return;
	int depth = AVLNodeDepth(pavl);
	//每层分配一个Queue，记录该层的所有节点指针，包含NULL节点
	PQueue* ppq = (PQueue*)malloc(sizeof(PQueue)*(depth+1));
	for (size_t level = 0; level < depth+1; level++)
	{
		ppq[level] = NULL;
		InitQueue(ppq[level]);
	}

	int level = 0;
	//先序遍历，得到每层的节点指针
	PreOrderGetAVLLevelNode(pavl, level, depth, ppq);
	for (size_t level = 0; level < depth; level++)
	{
		//每层后面输出树枝，表示归属关系
		if(0!=level)
			PrintBranchAVL(level, depth);
		//当前层的高度
		int high = depth - level;
		//当前层元素直接的空格数量
		int spacecount = pow(2, high) - 1;
		//输出当前层第一个元素前的空格
		PrintfSpaceAVL(spacecount/2);
		while (!IsEmptyQueue(ppq[level]))
		{
			PAVL pavl = (PAVL)DelQueue(ppq[level]);
			if (NULL == pavl)
				PrintfAVLNullNode();
			else
				PrintfAVLNode(pavl);
			//输出元素间的空格
			PrintfSpaceAVL(spacecount);

		}
		//该层输出结束，进行换行
		PrintEndLineAVL();
	}
}

//参考PrintAVL注释
void PrintAVLNoNull(PAVL& pavl)
{
	if (NULL == pavl)
		return;
	PrintEndLineAVL();
	int depth = AVLNodeDepth(pavl);
	PQueue* ppq = (PQueue*)malloc(sizeof(PQueue)*(depth + 1));
	for (size_t level = 0; level < depth + 1; level++)
	{
		ppq[level] = NULL;
		InitQueue(ppq[level]);
	}

	int level = 0;
	PreOrderGetAVLLevelNode(pavl, level, depth, ppq);
	for (size_t level = 0; level < depth; level++)
	{
		if (0 != level)
			PrintBranchAVL(level, depth, ppq);
		int high = depth - level;
		int spacecount = pow(2, high) - 1;
		PrintfSpaceAVL(spacecount / 2);
		while (!IsEmptyQueue(ppq[level]))
		{
			PAVL pavl = (PAVL)DelQueue(ppq[level]);
			if (NULL == pavl)
				PrintfAVLNullNodeNoNull();
			else
				PrintfAVLNode(pavl);
			PrintfSpaceAVL(spacecount);

		}

		PrintEndLineAVL();
	}
	PrintEndLineAVL();
}

void PrintBranchAVL(int level, int depth)
{
	int n = depth - level;
	int spacecount = pow(2, n) - 1;
	//cout << endl;
	PrintfSpaceAVL(spacecount / 2);
	int kuohao_count = pow(2, level-1);
	while (kuohao_count-- > 0)
	{
		cout << setw(NODE_CHAR_WIDE) << "/";
		PrintfSpaceAVL(spacecount);
		cout << setw(NODE_CHAR_WIDE) << "\\ ";
		PrintfSpaceAVL(spacecount);
	}
	PrintEndLineAVL();
}

void PrintBranchAVL(int level, int depth, PQueue* ppq)
{
	int high = depth - level;
	int spacecount = pow(2, high) - 1;
	cout << endl;
	PrintfSpaceAVL(spacecount / 2);
	int kuohao_count = pow(2, level - 1);
	PQueue pqnew = NULL;
	CopyQueue(ppq[level], pqnew);
	while (!IsEmptyQueue(pqnew))
	{
		PAVL pavl = (PAVL)DelQueue(pqnew);
		if (NULL==pavl)
			cout << setw(NODE_CHAR_WIDE) << " ";
		else
			cout << setw(NODE_CHAR_WIDE) << "/";
		PrintfSpaceAVL(spacecount);

		pavl = (PAVL)DelQueue(pqnew);
		if (NULL == pavl)
			cout << setw(NODE_CHAR_WIDE) << " ";
		else
			cout << setw(NODE_CHAR_WIDE) << "\\ ";
		PrintfSpaceAVL(spacecount);
	}

	cout << endl;
}

void PrintEndLineAVL()
{
	cout << endl;
}

void PrintfSpaceAVL(int count)
{
	for (size_t i = 0; i < count; i++)
	{
		for (size_t i = 0; i < NODE_CHAR_WIDE; i++)
		{
			cout << " ";
		}
	}
}

void PrintfAVLNullNode()
{
	cout << setw(NODE_CHAR_WIDE) << "- ";
}

void PrintfAVLNullNodeNoNull()
{
	cout << setw(NODE_CHAR_WIDE) << " ";
}

void PrintfAVLNode(PAVL& pavl)
{
	cout << setw(NODE_CHAR_WIDE) << pavl->key;
}
void PreOrderGetAVLLevelNode(PAVL pavl, int level, int depth, PQueue* ppq)
{
	//超过树的深度，后面都为NULL叶子节点了
	if (level > depth)
		return;

	//每层分别记录节点信息
	EnQueue(ppq[level], pavl);
	if (pavl)
	{
		PreOrderGetAVLLevelNode(pavl->LChild, level + 1, depth, ppq);
		PreOrderGetAVLLevelNode(pavl->RChild, level + 1, depth, ppq);
	}
	else
	{
		//如果没有到最深层，则NULL指针也要记录，因为记录的是NULL指针在该层的位置
		PreOrderGetAVLLevelNode(NULL, level + 1, depth, ppq);
		PreOrderGetAVLLevelNode(NULL, level + 1, depth, ppq);
	}
}

int AVLNodeDepth(PAVL& pavl)
{
	if (NULL == pavl)
	{
		return 0;
	}

	return max_int(AVLNodeDepth(pavl->LChild), AVLNodeDepth(pavl->RChild)) + 1;
}

int HighAVL(PAVL& pavl)
{
	if (NULL == pavl)
		return -1;

	return pavl->high;
}

//左旋，仅进行一次旋转，适合调整LL型不平衡状态
void SingRotateLeft(PAVL* ppavl)
{
	//旋转使左节点作为根
	PAVL p = (*ppavl)->LChild;
	(*ppavl)->LChild = p->RChild;
	p->RChild = (*ppavl);

	//先更新子树高，再更新父树高
	(*ppavl)->high = max_int(HighAVL((*ppavl)->LChild), HighAVL((*ppavl)->RChild)) + 1;
	p->high = max_int(HighAVL(p->LChild), HighAVL(p->RChild)) + 1;
	
	//ppavl是指针的地址，这里使最小不平衡树的父节点的儿子指针指向最新最小平衡树的根
	*ppavl = p;
}

//右旋，仅进行一次旋转，适合调整RR型不平衡状态
void SingRotateRight(PAVL* ppavl)
{
	//旋转使右节点作为根
	PAVL p = (*ppavl)->RChild;
	(*ppavl)->RChild = p->LChild;
	p->LChild = (*ppavl);

	//先更新子树高，再更新父树高
	(*ppavl)->high = max_int(HighAVL((*ppavl)->LChild), HighAVL((*ppavl)->RChild)) + 1;
	p->high = max_int(HighAVL(p->LChild), HighAVL(p->RChild)) + 1;

	//ppavl是指针的地址，这里使最小不平衡树的父节点的儿子指针指向最新最小平衡树的根
	*ppavl = p;
}

//双旋，适合LR型不平衡状态，左子树先RR旋转使整体形成LL型状态，再进行LL旋转
void DoubleRotateLR(PAVL* ppavl)
{
	SingRotateRight(&(*ppavl)->LChild);
	SingRotateLeft(ppavl);
}

//双旋，适合RL型不平衡状态，右子树先LL旋转使整体形成RR型状态，再进行RR旋转
void DoubleRotateRL(PAVL* ppavl)
{
	SingRotateLeft(&(*ppavl)->RChild);
	SingRotateRight(ppavl);
}

int max_int(int x, int y)
{
	return (x > y) ? x : y;
}

void TestSearchAVL()
{
	printf("\n\n\n------AVL 测试-----\n");
	//创建AVL树，并中序输出
	PAVL root = NULL;
	char * pstr = "45, 24, 53, 12, 90, 28, 25";
	CreateAVL(root, pstr);
	printf("InOrder AVL:\n");
	InOrderAVL(root);
	printf("\n");
	PrintAVLNoNull(root);

	//循环查找算法
	KeyType key = 53;
	PAVL pfind = SearchAVL(root, key);
	if (NULL != pfind)
		printf("find key %d, %d\n", key, pfind->key);
	else
		printf("find no key %d\n", key);

	//删除key节点
	printf("\ndelete %d, after delete:\n", key);
	DeleteAVL(&root, key);
	PrintAVLNoNull(root);
	//再次删除key节点
	printf("\ndelete %d again, after delete:\n", key);
	DeleteAVL(&root, key);
	PrintAVLNoNull(root);

	//再次查找key节点
	pfind = SearchAVL(root, key);
	if (NULL != pfind)
		printf("find key %d, %d\n", key, pfind->key);
	else
		printf("find no key %d\n", key);

	//插入key节点
	InsertAVL(root, key);
	printf("\ninsert %d, after insert:\n", key);
	PrintAVLNoNull(root);
	//再次插入key节点
	InsertAVL(root, key);
	printf("insert %d again, after insert:\n", key);
	PrintAVLNoNull(root);

	//从小到大插入测试
	int maxkey = 6;
	printf("Insert Node form 1 to %d:\n", maxkey);
	DestoryAVL(root);
	for (int i = 0; i < maxkey; i++)
	{
		key = i + 1;
		InsertAVL(root, key);
	}
	PrintAVLNoNull(root);

	//从大到小依次删除
	for (int i = maxkey; i > 0; i--)
	{
		key = i ;
		printf("del key:%d, tree:\n", key);
		DeleteAVL(&root, key);
		PrintAVLNoNull(root);
	}
	PrintAVLNoNull(root);
	printf("\n");

	//从大到小插入测试
	printf("Insert Node form %d to 1:\n", maxkey);
	DestoryAVL(root);
	for (int i = maxkey; i > 0; i--)
	{
		key = i;
		InsertAVL(root, key);
	}
	PrintAVLNoNull(root);

	//随机插入数据测试
	printf("Random Insert Node :", maxkey);
	srand((unsigned)time(NULL));
	maxkey = 49;
	int minkey = 1;
	DestoryAVL(root);
	for (int i = 0; i < 6; i++)
	{
		key = (rand() % (maxkey - minkey + 1)) + minkey;
		printf("%d ", key);
		InsertAVL(root, key);
	}
	printf("\n");
	PrintAVLNoNull(root);

}