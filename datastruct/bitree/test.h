#pragma once
#include <stack>
using namespace std;
#define MAX_STACK_SIZE	128

typedef struct Node
{
	char data;
	struct Node *LChild;
	struct Node *RChild;
}BiTNode, *BiTree;


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
void CopyTree(BiTree& bt_s, BiTree& bt_d);
int GetLevel(BiTree& bt, char c, int n);

void LeafInverted(BiTree& bt);
#define MAX_STACK_LEN	128
struct my_stack
{
	int top;
	BiTNode* node[MAX_STACK_LEN];
};
