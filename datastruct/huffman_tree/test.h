#pragma once
#include <stdio.h>
#include <stdlib.h>
/*	由于哈夫曼树中没有度没1的节点，则一棵树有n个叶子的哈夫曼树共有2*n-1个节点
*	可用一个大小为2n-1的一维数组来存放各个节点。
*	每个节点同事还包含其双亲信息和孩子节点信息，构成一个静态三叉链表*/

typedef struct  
{
	int weight;
	int parent, LChild, RChild;
}HMNode, *HMTree;

typedef char** huffmancode;
void DspHMTree(HMNode node[], int n);
void CreateHuffmanTree(HMTree& hmt, int w[], int leafcount);
void SelectMin2Item(HMTree& hmt, unsigned int cursize, int &min1, int &min2);
void CreateHuffmanCode(HMTree& hmt, unsigned int leafcount, huffmancode& hmc);
void DspHuffmanCode(HMTree& hmt, huffmancode& hmc, unsigned int leafcount);