#include <stdio.h>
#include <string>
#include "test.h"

int main()
{
	HMTree hmt = NULL;
	int weigth[] = { 5, 2, 7, 3, 8, 10, 15, 25, 11 };
	int leafcount = sizeof(weigth) / sizeof(int);
	printf("weight:\n");
	for (size_t i = 0; i < leafcount; i++)
	{
		printf("%d\t", weigth[i]);
	}
	CreateHuffmanTree(hmt, weigth, leafcount);
	int nodecount = 2 * leafcount - 1;
	printf("\nHuffman Tree:\n");
	DspHMTree(hmt, nodecount);
	huffmancode hmc = NULL;
	CreateHuffmanCode(hmt, leafcount, hmc);
	printf("HuffmanCode:\n");
	DspHuffmanCode(hmt, hmc, leafcount);
	return 0;
}