#include "test.h"
#include <stack>
#include <queue>
using namespace std;
queue<BiTree> g_layerqueue;

BiTree PreTreeNode = NULL;
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
			MakeBiNode(pTemp, c);
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
	if (root != NULL)
	{
		Visit(root);
		if (root->LChild == NULL && root->RChild == NULL)
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
	if (NULL != bt)
	{
		g_layerqueue.push(bt);
		while (!g_layerqueue.empty())
		{
			BiTree temp = g_layerqueue.front();
			g_layerqueue.pop();
			if (NULL != temp)
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
	if (NULL == bt)
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
	if (NULL != bt)
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
	bt->Ltag = bt->Rtag = 0;
}

void CopyTree(BiTree& bt_s, BiTree& bt_d)
{
	if (NULL != bt_s)
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
	if (0 != ll)
	{
		return ll;
	}

	return GetLevel(bt->RChild, c, n + 1);
}

my_stack* g_Mystack = (my_stack*)malloc(sizeof(my_stack));

void DspLeafInverted()
{
	printf("%c: ", g_Mystack->node[g_Mystack->top - 1]->data);
	for (int i = g_Mystack->top - 1; i >= 0; i--)
	{
		printf("%c ", g_Mystack->node[i]->data);
	}
	printf("\n");
}


void popParentOfRchild(BiTree& bt)
{
	/*	先序遍历，如果叶子节点是栈顶的右孩子，则栈顶需要弹出，因为先序遍历的右边叶子
	*	已经遍历了，其父节点不会是下一个叶子节点中的路径节点了。如果其父节点是祖父节点的右孩子，
	*	则其祖父节点也不会是下一个叶子节点中的路径节点了，需要如此递归弹出
	*/
	if (g_Mystack->top == 0)
	{
		//已经空栈
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
	if (NULL != bt)
	{
		g_Mystack->node[g_Mystack->top] = bt;
		g_Mystack->top++;
		if (bt->LChild == NULL &&bt->RChild == NULL)
		{
			DspLeafInverted();
			g_Mystack->top--;	//将自身弹出
								//先序遍历，右边叶子的纯右路径父节点不会出现在下一个叶子的节点路径中
			popParentOfRchild(bt);
			return;
		}
		LeafInverted(bt->LChild);
		LeafInverted(bt->RChild);
	}
}

void CreateTreeUsePreInString(string szPre, string szIn, BiTree& bt)
{
	if (!szPre.empty())
	{
		//生成根节点
		MakeBiNode(bt, szPre[0]);
		int npos = szIn.find(szPre[0]);
		if (npos != -1)
		{
			//获得左右孩子中序字串
			string szInLchild = szIn.substr(0, npos);
			string szInRchild = szIn.substr(npos + 1);
			//获得左右孩子前序字串
			string szPreLChild, szPreRChild;
			GetChildPreString(npos, szPre, szPreLChild, szPreRChild);
			//生成左右孩子节点
			CreateTreeUsePreInString(szPreLChild, szInLchild, bt->LChild);
			CreateTreeUsePreInString(szPreRChild, szInRchild, bt->RChild);
		}
		else
		{
			printf("error: PreString or InString is not match\n");
			bt = NULL;
		}
	}
	else
		bt = NULL;
}

void GetChildPreString(int npos, string szPre, string& szPreLeft, string& szPreRight)
{
	//前序的首位是中序的根，中序的根位置将左右树划分，这个位置
	//其实也是前序里左右树划分的地方。因为树根的左右节点数量是固定的。
	//使用前序和中序，去掉根节点，左子树节点后才能是右子树节点
	szPreLeft = szPre.substr(1, npos);
	szPreRight = szPre.substr(npos + 1);
}
void InThreadTree(BiTree& bt)
{
	if (NULL != bt)
	{
		//线索化左子树
		InThreadTree(bt->LChild);

		//线索化自身节点
		if (NULL == bt->LChild)
		{
			bt->Ltag = 1;
			bt->LChild = PreTreeNode;
		}

		if (NULL != PreTreeNode && NULL == PreTreeNode->RChild)
		{
			PreTreeNode->Rtag = 1;
			PreTreeNode->RChild = bt;
		}
		PreTreeNode = bt;

		//线索化右子树
		InThreadTree(bt->RChild);
	}
}

void MakeThreadHead(ThreadHead& th)
{
	th = (ThreadHead)malloc(sizeof(ThreadNode));
	th->data = 'z';
	th->LChild = th->RChild = NULL;
	th->Ltag = th->Rtag = 0;
}
void CreateThreadTree(BiTree& bt, ThreadHead& th, int threadtype/*=InThread*/)
{
	if (NULL == bt)
		return;
	MakeThreadHead(th);
	BiTree btnew = NULL;
	CopyTree(bt, btnew);
	th->threadtype = threadtype;
	PreTreeNode = (BiTree)th;
	switch (th->threadtype)
	{
	case InThread:
		InThreadTree(btnew);
		break;
	case PreThread:
		PreThreadTree(btnew);
		break;
	case PostThread:
		PostThreadTree(btnew);
		break;
	default:
		printf("invalid threadtype.\n");
		break;
	}
}

void DspThreadTree(ThreadHead& th)
{
	if (NULL!=th)
	{
		BiTree pHead = (BiTree)th->RChild;
		while (NULL != pHead)
		{
			Visit(pHead);
			switch (th->threadtype)
			{
			case InThread:
				pHead = FindPostNodeInThread(pHead);
				break;
			case PreThread:
				pHead = FindPostNodePreThread(pHead);
				break;
			case PostThread:
				pHead = FindPostNodePostThread(pHead);
				break;
			default:
				printf("invalid threadtype.\n");
				return;
			}
		}
	}
}

BiTree FindPreNodeInThread(BiTree& bt)
{
	/*中序线索二叉树向前向后遍历均比较方便*/
	if (NULL != bt)
	{
		if (bt->Ltag == 1)
		{
			return bt->LChild;
		}
		else
		{
			BiTree p = bt->LChild;
			while (p&&p->Rtag != 1)
			{
				p = p->RChild;
			}

			return p;
		}
	}

	return NULL;
}

BiTree FindPostNodeInThread(BiTree& bt)
{
	/*中序线索二叉树向前向后遍历均比较方便*/
	if (NULL != bt)
	{
		if (bt->Rtag == 1)
		{
			return bt->RChild;
		}
		else
		{
			BiTree p = bt->RChild;
			while (p&&p->Ltag != 1)
			{
				p = p->LChild;
			}

			return p;
		}
	}

	return NULL;
}

void PreThreadTree(BiTree& bt)
{
	if (NULL != bt)
	{
		//线索化自身节点
		if (NULL == bt->LChild)
		{
			bt->Ltag = 1;
			bt->LChild = PreTreeNode;
		}

		if (NULL != PreTreeNode && NULL == PreTreeNode->RChild)
		{
			PreTreeNode->Rtag = 1;
			PreTreeNode->RChild = bt;
		}
		PreTreeNode = bt;

		//线索化左子树
		InThreadTree(bt->LChild);

		//线索化右子树
		InThreadTree(bt->RChild);
	}
}

BiTree FindPreNodePreThread(BiTree& bt)
{
	/*	先序二叉树获取前驱节点比较复杂，因为要先获取其父节点，
	*	根据此节点是父节点的左右孩子来判断。因为要获取父节点比较复杂，
	*	如果使用三叉链表则比较方便找到父节点，使用先序线索二叉树找
	*	前驱节点代价比较大。先序线索二叉树适合向后遍历*/
	return NULL;
}

BiTree FindPostNodePreThread(BiTree& bt)
{
	if (NULL!=bt)
	{
		if (bt->Ltag==0)
			return bt->LChild;
		else
			return bt->RChild;
	}
	return NULL;
}

void PostThreadTree(BiTree& bt)
{
	if (NULL != bt)
	{
		//线索化左子树
		InThreadTree(bt->LChild);

		//线索化右子树
		InThreadTree(bt->RChild);

		//线索化自身节点
		if (NULL == bt->LChild)
		{
			bt->Ltag = 1;
			bt->LChild = PreTreeNode;
		}

		if (NULL != PreTreeNode && NULL == PreTreeNode->RChild)
		{
			PreTreeNode->Rtag = 1;
			PreTreeNode->RChild = bt;
		}
		PreTreeNode = bt;
	}
}

BiTree FindPreNodePostThread(BiTree& bt)
{
	if (NULL!=bt)
	{
		if (bt->Rtag==0)
			return bt->RChild;
		else
			return bt->LChild;
	}
	return NULL;
}

BiTree FindPostNodePostThread(BiTree& bt)
{
	/*	后序二叉树获取后驱节点比较复杂，因为要先获取其父节点，
	*	根据此节点是父节点的左右孩子来判断。因为要获取父节点比较复杂，
	*	如果使用三叉链表则比较方便找到父节点，使用后序线索二叉树找后
	*	驱节点代价比较大。后序线索二叉树适合向前遍历*/
	return NULL;
}

