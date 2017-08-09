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

/*  ����AVL������ʱ��һ��һֱ���²�������������ü�¼���ڵ�켣��
*	���Կ��Ժܷ����ʹ��ѭ�����в���*/
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

	/* ����ɾ���ڵ��ÿ�����ڵ�㶼��Ҫ����ƽ�⣬����ʹ��ѭ���ṹ���ɾ�������ʹ�õݹ� */
	if (key < (*ppavl)->key)
	{
		DeleteAVL(&(*ppavl)->LChild, key);
		//������ɾ���ڵ㣬ֻ�����������������
		if (2 == HighAVL((*ppavl)->RChild) - HighAVL((*ppavl)->LChild))
		{
			//�ж��ǲ���RL�ṹ
			if ((*ppavl)->RChild->LChild && HighAVL((*ppavl)->RChild->LChild) > HighAVL((*ppavl)->RChild->RChild))
				DoubleRotateRL(ppavl);
			else
				SingRotateRight(ppavl);
		}
	}
	else if (key > (*ppavl)->key)
	{
		DeleteAVL(&(*ppavl)->RChild, key);
		//������ɾ���ڵ㣬ֻ�����������������
		if (2 == HighAVL((*ppavl)->LChild) - HighAVL((*ppavl)->RChild))
		{
			//�ж��ǲ���LR�ṹ
			if ((*ppavl)->LChild->RChild && HighAVL((*ppavl)->LChild->RChild) > HighAVL((*ppavl)->LChild->LChild))
				DoubleRotateLR(ppavl);
			else
				SingRotateLeft(ppavl);
		}
	}
	else
	{
		//�ݹ����
		DeleteAVL(ppavl);
	}
}

void DeleteAVL(PAVL* ppavl)
{
	//ǰ����ɾ���ڵ㲿�ֺ�BSTreeɾ���ڵ���ȫһ������Ҫ�Ǻ��������˵���ƽ�����
	PAVL q;
	if (NULL == (*ppavl)->LChild && NULL == (*ppavl)->RChild)
	{
		//��ɾ���ڵ����Ҷ��Ӿ�ΪNULL����ֱ��ɾ���˽ڵ�
		free((*ppavl));
		(*ppavl) = NULL;
	}
	else if (NULL == (*ppavl)->LChild)
	{
		//��ɾ���ڵ������ΪNULL����ָ��ɾ���ڵ��ָ��ָ�����Ҷ���
		q = (*ppavl);
		*ppavl = ((*ppavl)->RChild);
		free(q);
	}
	else if (NULL == (*ppavl)->RChild)
	{
		//��ɾ���ڵ��Ҷ���ΪNULL����ָ��ɾ���ڵ��ָ��ָ���������
		q = (*ppavl);
		*ppavl = ((*ppavl)->LChild);
		free(q);
	}
	else
	{
		//��ɾ���ڵ�����Ҷ��Ӿ���ΪNULL�����ұ�ɾ���ڵ��ǰ���ڵ�
		PAVL pre = (*ppavl);		//��¼ǰ�����ĸ��ڵ�
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
			//��������ǰ���ڵ�����ʣ�ǰ���ڵ�ض�û���Ҷ��ӡ�
			//���ǰ���ڵ�������ӣ���ֱ����ǰ���ڵ��滻��ɾ���ڵ㣬ɾ��ǰ���ڵ㼴�ɡ�
			(*ppavl)->key = q->key;
			(*ppavl)->LChild = NULL;
			free(q);
		}
		else
		{
			//ǰ���ڵ���������
			if (pre == (*ppavl))
			{
				//ǰ���ڵ��Ǳ�ɾ�ڵ��ֱ�������
				//��ɾ���ڵ㱻��ǰ���ڵ����
				(*ppavl)->LChild = q->LChild;
				(*ppavl)->key = q->key;
				free(q);
			}
			else
			{
				//ǰ���ڵ��Ǳ�ɾ�ڵ��ֱ������ӵ�ĳ�������Ҷ���
				//��ɾ���ڵ㱻��ǰ���ڵ��������ǰ���ڵ�����ӹҽӵ��½ڵ���Һ�����
				(*ppavl)->key = q->key;
				pre->RChild = q->LChild;
				free(q);
			}
		}
	}

	/*  ����ƽ��Ĳ���
	*	1 �����ɾ���ڵ���������Ϊ�գ�ɾ����˽ڵ�ƽ��
	*	2 ���ֻ��һ������Ϊ�գ���ʵ�ǿ�����ֻ����һ�㣬����ɾ��������ƽ�����ʵΪ0�ˣ��˽ڵ�ƽ��
	*	3 ���������������Ϊ�գ����ѡ��λ����������ǰ���ڵ�����滻ɾ������ʱ��������������߶Ƚ��ͣ�������Ҫ����ƽ��	*/
	if (NULL!=(*ppavl))
	{
		if (2 == HighAVL((*ppavl)->RChild) - HighAVL((*ppavl)->LChild))
		{
			//�ж��ǲ���RL�ṹ
			if ((*ppavl)->RChild->LChild && HighAVL((*ppavl)->RChild->LChild) > HighAVL((*ppavl)->RChild->RChild))
				DoubleRotateRL(ppavl);
			else
				SingRotateRight(ppavl);
		}

		//������С��ƽ������������ƽ��󶥵�߶ȣ�������߲�������ƽ�����
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
	//ÿ�����һ��Queue����¼�ò�����нڵ�ָ�룬����NULL�ڵ�
	PQueue* ppq = (PQueue*)malloc(sizeof(PQueue)*(depth+1));
	for (size_t level = 0; level < depth+1; level++)
	{
		ppq[level] = NULL;
		InitQueue(ppq[level]);
	}

	int level = 0;
	//����������õ�ÿ��Ľڵ�ָ��
	PreOrderGetAVLLevelNode(pavl, level, depth, ppq);
	for (size_t level = 0; level < depth; level++)
	{
		//ÿ����������֦����ʾ������ϵ
		if(0!=level)
			PrintBranchAVL(level, depth);
		//��ǰ��ĸ߶�
		int high = depth - level;
		//��ǰ��Ԫ��ֱ�ӵĿո�����
		int spacecount = pow(2, high) - 1;
		//�����ǰ���һ��Ԫ��ǰ�Ŀո�
		PrintfSpaceAVL(spacecount/2);
		while (!IsEmptyQueue(ppq[level]))
		{
			PAVL pavl = (PAVL)DelQueue(ppq[level]);
			if (NULL == pavl)
				PrintfAVLNullNode();
			else
				PrintfAVLNode(pavl);
			//���Ԫ�ؼ�Ŀո�
			PrintfSpaceAVL(spacecount);

		}
		//�ò�������������л���
		PrintEndLineAVL();
	}
}

//�ο�PrintAVLע��
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
	//����������ȣ����涼ΪNULLҶ�ӽڵ���
	if (level > depth)
		return;

	//ÿ��ֱ��¼�ڵ���Ϣ
	EnQueue(ppq[level], pavl);
	if (pavl)
	{
		PreOrderGetAVLLevelNode(pavl->LChild, level + 1, depth, ppq);
		PreOrderGetAVLLevelNode(pavl->RChild, level + 1, depth, ppq);
	}
	else
	{
		//���û�е�����㣬��NULLָ��ҲҪ��¼����Ϊ��¼����NULLָ���ڸò��λ��
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

//������������һ����ת���ʺϵ���LL�Ͳ�ƽ��״̬
void SingRotateLeft(PAVL* ppavl)
{
	//��תʹ��ڵ���Ϊ��
	PAVL p = (*ppavl)->LChild;
	(*ppavl)->LChild = p->RChild;
	p->RChild = (*ppavl);

	//�ȸ��������ߣ��ٸ��¸�����
	(*ppavl)->high = max_int(HighAVL((*ppavl)->LChild), HighAVL((*ppavl)->RChild)) + 1;
	p->high = max_int(HighAVL(p->LChild), HighAVL(p->RChild)) + 1;
	
	//ppavl��ָ��ĵ�ַ������ʹ��С��ƽ�����ĸ��ڵ�Ķ���ָ��ָ��������Сƽ�����ĸ�
	*ppavl = p;
}

//������������һ����ת���ʺϵ���RR�Ͳ�ƽ��״̬
void SingRotateRight(PAVL* ppavl)
{
	//��תʹ�ҽڵ���Ϊ��
	PAVL p = (*ppavl)->RChild;
	(*ppavl)->RChild = p->LChild;
	p->LChild = (*ppavl);

	//�ȸ��������ߣ��ٸ��¸�����
	(*ppavl)->high = max_int(HighAVL((*ppavl)->LChild), HighAVL((*ppavl)->RChild)) + 1;
	p->high = max_int(HighAVL(p->LChild), HighAVL(p->RChild)) + 1;

	//ppavl��ָ��ĵ�ַ������ʹ��С��ƽ�����ĸ��ڵ�Ķ���ָ��ָ��������Сƽ�����ĸ�
	*ppavl = p;
}

//˫�����ʺ�LR�Ͳ�ƽ��״̬����������RR��תʹ�����γ�LL��״̬���ٽ���LL��ת
void DoubleRotateLR(PAVL* ppavl)
{
	SingRotateRight(&(*ppavl)->LChild);
	SingRotateLeft(ppavl);
}

//˫�����ʺ�RL�Ͳ�ƽ��״̬����������LL��תʹ�����γ�RR��״̬���ٽ���RR��ת
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
	printf("\n\n\n------AVL ����-----\n");
	//����AVL�������������
	PAVL root = NULL;
	char * pstr = "45, 24, 53, 12, 90, 28, 25";
	CreateAVL(root, pstr);
	printf("InOrder AVL:\n");
	InOrderAVL(root);
	printf("\n");
	PrintAVLNoNull(root);

	//ѭ�������㷨
	KeyType key = 53;
	PAVL pfind = SearchAVL(root, key);
	if (NULL != pfind)
		printf("find key %d, %d\n", key, pfind->key);
	else
		printf("find no key %d\n", key);

	//ɾ��key�ڵ�
	printf("\ndelete %d, after delete:\n", key);
	DeleteAVL(&root, key);
	PrintAVLNoNull(root);
	//�ٴ�ɾ��key�ڵ�
	printf("\ndelete %d again, after delete:\n", key);
	DeleteAVL(&root, key);
	PrintAVLNoNull(root);

	//�ٴβ���key�ڵ�
	pfind = SearchAVL(root, key);
	if (NULL != pfind)
		printf("find key %d, %d\n", key, pfind->key);
	else
		printf("find no key %d\n", key);

	//����key�ڵ�
	InsertAVL(root, key);
	printf("\ninsert %d, after insert:\n", key);
	PrintAVLNoNull(root);
	//�ٴβ���key�ڵ�
	InsertAVL(root, key);
	printf("insert %d again, after insert:\n", key);
	PrintAVLNoNull(root);

	//��С����������
	int maxkey = 6;
	printf("Insert Node form 1 to %d:\n", maxkey);
	DestoryAVL(root);
	for (int i = 0; i < maxkey; i++)
	{
		key = i + 1;
		InsertAVL(root, key);
	}
	PrintAVLNoNull(root);

	//�Ӵ�С����ɾ��
	for (int i = maxkey; i > 0; i--)
	{
		key = i ;
		printf("del key:%d, tree:\n", key);
		DeleteAVL(&root, key);
		PrintAVLNoNull(root);
	}
	PrintAVLNoNull(root);
	printf("\n");

	//�Ӵ�С�������
	printf("Insert Node form %d to 1:\n", maxkey);
	DestoryAVL(root);
	for (int i = maxkey; i > 0; i--)
	{
		key = i;
		InsertAVL(root, key);
	}
	PrintAVLNoNull(root);

	//����������ݲ���
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