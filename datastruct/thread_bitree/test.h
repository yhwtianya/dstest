#pragma once
#include <stack>
using namespace std;
#define MAX_STACK_SIZE	128
#define	PreThread	0
#define InThread	1
#define PostThread	2

typedef struct Node
{
	char data;
	struct Node *LChild;
	struct Node *RChild;
	int Ltag;
	int Rtag;
}BiTNode, *BiTree;

typedef struct ThreadNode
{
	char data;
	struct Node *LChild;
	struct Node *RChild;
	int Ltag;
	int Rtag;
	int threadtype;
}* ThreadHead;


void CreateBiTree(BiTree& bt, const char* s);
void Visit(BiTree root);
void DspTree(BiTree root);
void DestoryBiTree(BiTree& bt);
BiTree FindNode(BiTree& bt, char x);
BiTree LchildNode(BiTree& bt);
BiTree RchildNode(BiTree& bt);
int maxtree(int x, int y);
int BTNodeDepth(BiTree& bt);

void PreOrder(BiTree& bt);
void InOrder(BiTree& bt);
void PostOrder(BiTree& bt);
void LayerOrder(BiTree& bt);
int NodeCount(BiTree& bt);
int LeafCount(BiTree& bt);
void MakeBiNode(BiTree& bt, char c);
void CopyTree(BiTree& bt_s, BiTree& bt_d);
int GetLevel(BiTree& bt, char c, int n);

void LeafInverted(BiTree& bt);
void CreateTreeUsePreInString(string szPre, string szIn, BiTree& bt);
void GetChildPreString(int pos, string szPre, string& szPreLeft, string& szPreRight);
#define MAX_STACK_LEN	128
struct my_stack
{
	int top;
	BiTNode* node[MAX_STACK_LEN];
};

void DspThreadTree(ThreadHead& th);
void CreateThreadTree(BiTree& bt, ThreadHead& th, int threadtype = InThread);
//中序线索
void InThreadTree(BiTree& bt);
BiTree FindPreNodeInThread(BiTree& bt);
BiTree FindPostNodeInThread(BiTree& bt);
//先序线索
void PreThreadTree(BiTree& bt);
BiTree FindPreNodePreThread(BiTree& bt);
BiTree FindPostNodePreThread(BiTree& bt);
//后序线索
void PostThreadTree(BiTree& bt);
BiTree FindPreNodePostThread(BiTree& bt);
BiTree FindPostNodePostThread(BiTree& bt);
