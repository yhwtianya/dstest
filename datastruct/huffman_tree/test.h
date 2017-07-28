#pragma once
#include <stdio.h>
#include <stdlib.h>
/*	���ڹ���������û�ж�û1�Ľڵ㣬��һ������n��Ҷ�ӵĹ�����������2*n-1���ڵ�
*	����һ����СΪ2n-1��һά��������Ÿ����ڵ㡣
*	ÿ���ڵ�ͬ�»�������˫����Ϣ�ͺ��ӽڵ���Ϣ������һ����̬��������*/

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