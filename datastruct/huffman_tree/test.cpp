#include "test.h"
#include <string.h>

void DspHMTree(HMNode node[], int n)
{
	printf("index\tweight\tparent\tLChild\tRChild\n");
	for (size_t i=0; i<n; ++i)
	{
		printf("%d\t%d\t%d\t%d\t%d\n", i, node[i].weight, node[i].parent, node[i].LChild, node[i].RChild);
	}
}

void CreateHuffmanTree(HMTree& hmt, int w[], int leafcount)
{
	//leafcount为叶子节点数，nodecount为总节点数
	int nodecount = 2 * leafcount - 1;
	//采用顺序存储结构
	hmt = (HMTree)malloc(nodecount * sizeof(HMNode));
	for (int i = 0; i < leafcount; i++)
	{
		//初始化叶子
		hmt[i] = { w[i], 0, 0, 0 };
	}
	for (int i = leafcount; i < nodecount; ++i)
	{
		//初始化非叶子节点
		hmt[i] = { 0, 0, 0, 0 };
	}

	for (size_t i = leafcount; i < nodecount; i++)
	{
		int nMin1 = 0;
		int nMin2 = 0;
		/*	选择权值最小的两个节点合成子树，将新合成的子树保存并参与后序最小权值选择，
		*	将被合并的元素父指针赋值，以后不再参与最小权值选择*/
		SelectMin2Item(hmt, i, nMin1, nMin2);
		hmt[nMin1].parent = i;
		hmt[nMin2].parent = i;
		hmt[i].LChild = nMin1;
		hmt[i].RChild = nMin2;
		hmt[i].weight = hmt[nMin1].weight + hmt[nMin2].weight;
	}
}

void SelectMin2Item(HMTree& hmt, unsigned int cursize, int &min1, int &min2)
{
	int firstfindpos = 0;
	for (; firstfindpos < cursize; firstfindpos++)
	{
		if (hmt[firstfindpos].parent==0)
		{
			min1 = firstfindpos;
			break;
		}
	}

	for (size_t i = firstfindpos+1; i < cursize; i++)
	{
		if (hmt[i].parent==0)
		{
			if (hmt[i].weight<hmt[min1].weight)
				min1 = i;
		}
	}

	for (; firstfindpos < cursize; firstfindpos++)
	{
		if (hmt[firstfindpos].parent == 0 && firstfindpos != min1)
		{
			min2 = firstfindpos;
			break;
		}
	}

	for (size_t i = firstfindpos + 1; i < cursize; i++)
	{
		if (hmt[i].parent == 0 && i!=min1)
		{
			if (hmt[i].weight < hmt[min2].weight)
				min2 = i;
		}
	}
	/*	这里min1权值一般小于min2权值，可以决定让min1做左孩子，让min2做有孩子。但是如果遇到两个权值相同，
	*	即两个节点形成的子树A和某节点B权值相同，
	*	就会有AB这两个节点谁做新子树的左孩子还是右孩子的考虑，其实这里不用考虑，
	*	因为无论左右孩子，他们所处的树的深度相同，树的带权路径长度也相同，这棵树依然是最优二叉树。*/
}

/*	leafcount是叶子节点数量，当然也可以是总节点数，不过非叶子节点编码是无效的,
*	因为非叶子节点是其他节点合成的子树，其是被合成节点的前缀码。
*	这里采用从叶子节点，逆序查找到根节点，形成哈夫曼编码。
*	如果采用从根节点查找到叶子节点形成哈夫曼编码，用栈应该可以实现。 */
void CreateHuffmanCode(HMTree& hmt, unsigned int leafcount, huffmancode& hmc)
{
	//保存所有哈夫曼编码的头指针
	hmc = (huffmancode)malloc(leafcount * sizeof(char*));
	//保存一个哈夫曼编码，编码长度最长不会超过leafcount-1，因为即使两个叶子节点，
	//也得合并一下才能形成哈夫曼树，他们的编码长度变为了1
	char* code = (char*)malloc(leafcount*sizeof(char));
	code[leafcount-1] = '\0';
	for (size_t i = 0; i < leafcount; i++)
	{
		int start = leafcount - 1;
		int parent = hmt[i].parent;
		int curnode = i;
		for (; 0!=parent; curnode=parent,parent=hmt[parent].parent)
		{
			/*	哈夫曼编码人为规定的左孩子路径标记0，右孩子路径标记1，也可自己标记，编码也是最优前缀码 */
			//编码逆序入数组，顺序取出，类似栈
			if (hmt[parent].LChild == curnode)
				code[--start] = '0';
			else
				code[--start] = '1';
		}
		int codelen = leafcount - start;
		hmc[i] = (char*)malloc((codelen) * sizeof(char));
		strcpy_s(hmc[i], codelen, &code[start]);
	}
	free(code);
}

void DspHuffmanCode(HMTree& hmt, huffmancode& hmc, unsigned int leafcount)
{
	printf("index\tweight\thuffmancode\n");
	for (size_t i = 0; i < leafcount; i++)
	{
		printf("%d\t%d\t", i, hmt[i].weight);
		char* pcode = (hmc[i]);
		while ('\0' != *pcode)
		{
			printf("%c ", *pcode);
			++pcode;
		}
		printf("\n");
	}
}
