#include "test.h"
#include <stack>
#include <queue>
using namespace std;
queue<BiTree> g_layerqueue;

void CreateBiTree(BiTree& bt, const char* s)
{
	stack<BiTree> mystack;
	BiTree pR = NULL;
	BiTree pTemp = NULL;
	int op = 1;
	for (int i = 0; i < strlen(s); i++)
	{
		char c = s[i];
		switch (c)
		{
		case '(':
			op = 1;
			mystack.push(pTemp);
			break;
		case ',':
			op = 2;
			break;
		case ')':     

			mystack.pop();
			break;;
		case ' ':
			break;

		default:
			pTemp = (BiTree)malloc(sizeof(BiTNode));
			pTemp->data = c;
			pTemp->LChild = NULL;
			pTemp->RChild = NULL;
			if (bt == NULL)
			{
				bt = pTemp;
			}
			else
			{
				switch (op)
				{
				case 1:
					mystack.top()->LChild = pTemp;
					break;
				case 2:
					mystack.top()->RChild = pTemp;
					break;

				default:
					break;
				}
			}

			break;
		}
	}
}

void Visit(BiTree root)
{
	printf("%c ", root->data);
}

void DspTree(BiTree root)
{
	if (root!=NULL)
	{
		Visit(root);
		if (root->LChild==NULL && root->RChild==NULL)
			return;

		printf("(");
		DspTree(root->LChild);

		if (root->RChild == NULL)
		{
			printf(")");
			return;
		}
		printf(",");
		DspTree(root->RChild);
		printf(")");
	}
}

void DestoryBiTree(BiTree& bt)
{
	if (NULL != bt)
	{
		DestoryBiTree(bt->LChild);
		DestoryBiTree(bt->RChild);
		free(bt);
	}
}

BiTree FindNode(BiTree& bt, char x)
{
	if (bt->data == x)
	{
		return bt;
	}

	if (FindNode(bt->LChild, x))
	{
		return bt->LChild;
	}

	return FindNode(bt->RChild, x);
}

BiTree LchildNode(BiTree& bt)
{
	return bt->LChild;
}

BiTree RchildNode(BiTree& bt)
{
	return bt->RChild;
}

int maxtree(int x, int y)
{
	return (x > y) ? x : y;
}

int BTNodeDepth(BiTree& bt)
{
	if (NULL == bt)
	{
		return 0;
	}

	return maxtree(BTNodeDepth(bt->LChild), BTNodeDepth(bt->RChild)) + 1;
}

void PreOrder(BiTree& bt)
{
	if (NULL != bt)
	{
		Visit(bt);
		PreOrder(bt->LChild);
		PreOrder(bt->RChild);
	}
}

void InOrder(BiTree& bt)
{
	if (NULL != bt)
	{
		InOrder(bt->LChild);
		Visit(bt);
		InOrder(bt->RChild);
	}
}

void PostOrder(BiTree& bt)
{
	if (NULL != bt)
	{
		PostOrder(bt->LChild);
		PostOrder(bt->RChild);
		Visit(bt);
	}
}

void LayerOrder(BiTree& bt)
{
	if (NULL!=bt)
	{
		g_layerqueue.push(bt);
		while (!g_layerqueue.empty())
		{
			BiTree temp = g_layerqueue.front();
			g_layerqueue.pop();
			if (NULL!=temp)
			{
				Visit(temp);
				if (NULL != temp->LChild)
					g_layerqueue.push(temp->LChild);
				if (NULL != temp->RChild)
					g_layerqueue.push(temp->RChild);
			}
		}
	}
}

int NodeCount(BiTree& bt)
{
	if (NULL==bt)
	{
		return 0;
	}
	else
	{
		return NodeCount(bt->LChild) + NodeCount(bt->RChild) + 1;
	}
}

int LeafCount(BiTree& bt)
{
	if (NULL!=bt)
	{
		if (NULL == bt->LChild && NULL == bt->RChild)
		{
			return 1;
		}
		else
		{
			return LeafCount(bt->LChild) + LeafCount(bt->RChild);
		}
	}
	else
		return 0;
}

void MakeBiNode(BiTree& bt, char c)
{
	bt = (BiTree)malloc(sizeof(BiTNode));
	bt->data = c;
	bt->LChild = bt->RChild = NULL;
}

void CopyTree(BiTree& bt_s, BiTree& bt_d)
{
	if (NULL!=bt_s)
	{
		MakeBiNode(bt_d, bt_s->data);
		CopyTree(bt_s->LChild, bt_d->LChild);
		CopyTree(bt_s->RChild, bt_d->RChild);
	}
}

int GetLevel(BiTree& bt, char c, int n)
{
	if (NULL == bt)
	{
		return 0;
	}

	if (c == bt->data)
		return n;

	int ll = GetLevel(bt->LChild, c, n + 1);
	if (0!=ll)
	{
		return ll;
	}

	return GetLevel(bt->RChild, c, n + 1);
}

my_stack* g_Mystack = (my_stack*)malloc(sizeof(my_stack));

void DspLeafInverted()
{
	printf("%c: ", g_Mystack->node[g_Mystack->top-1]->data);
	for (int i = g_Mystack->top-1; i >=0; i--)
	{
		printf("%c ", g_Mystack->node[i]->data);
	}
	printf("\n");
}


void popParentOfRchild(BiTree& bt)
{
	/*	������������Ҷ�ӽڵ���ջ�����Һ��ӣ���ջ����Ҫ��������Ϊ����������ұ�Ҷ��
	*	�Ѿ������ˣ��丸�ڵ㲻������һ��Ҷ�ӽڵ��е�·���ڵ��ˡ�����丸�ڵ����游�ڵ���Һ��ӣ�
	*	�����游�ڵ�Ҳ��������һ��Ҷ�ӽڵ��е�·���ڵ��ˣ���Ҫ��˵ݹ鵯��
	*/
	if (g_Mystack->top==0)
	{
		//�Ѿ���ջ
		return;
	}

	BiTree topitem = g_Mystack->node[g_Mystack->top - 1];
	if (topitem->RChild == bt)
	{
		g_Mystack->top--;
		popParentOfRchild(topitem);
	}
}

void LeafInverted(BiTree& bt)
{
	if (NULL!=bt)
	{
		g_Mystack->node[g_Mystack->top] = bt;
		g_Mystack->top++;
		if (bt->LChild==NULL &&bt->RChild==NULL)
		{
			DspLeafInverted();
			g_Mystack->top--;	//��������
			//����������ұ�Ҷ�ӵĴ���·�����ڵ㲻���������һ��Ҷ�ӵĽڵ�·����
			popParentOfRchild(bt);
			return;
		}
		LeafInverted(bt->LChild);
		LeafInverted(bt->RChild);
	}
}

