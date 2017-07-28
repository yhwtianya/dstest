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
	//leafcountΪҶ�ӽڵ�����nodecountΪ�ܽڵ���
	int nodecount = 2 * leafcount - 1;
	//����˳��洢�ṹ
	hmt = (HMTree)malloc(nodecount * sizeof(HMNode));
	for (int i = 0; i < leafcount; i++)
	{
		//��ʼ��Ҷ��
		hmt[i] = { w[i], 0, 0, 0 };
	}
	for (int i = leafcount; i < nodecount; ++i)
	{
		//��ʼ����Ҷ�ӽڵ�
		hmt[i] = { 0, 0, 0, 0 };
	}

	for (size_t i = leafcount; i < nodecount; i++)
	{
		int nMin1 = 0;
		int nMin2 = 0;
		/*	ѡ��Ȩֵ��С�������ڵ�ϳ����������ºϳɵ��������沢���������СȨֵѡ��
		*	�����ϲ���Ԫ�ظ�ָ�븳ֵ���Ժ��ٲ�����СȨֵѡ��*/
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
	/*	����min1Ȩֵһ��С��min2Ȩֵ�����Ծ�����min1�����ӣ���min2���к��ӡ����������������Ȩֵ��ͬ��
	*	�������ڵ��γɵ�����A��ĳ�ڵ�BȨֵ��ͬ��
	*	�ͻ���AB�������ڵ�˭�������������ӻ����Һ��ӵĿ��ǣ���ʵ���ﲻ�ÿ��ǣ�
	*	��Ϊ�������Һ��ӣ��������������������ͬ�����Ĵ�Ȩ·������Ҳ��ͬ���������Ȼ�����Ŷ�������*/
}

/*	leafcount��Ҷ�ӽڵ���������ȻҲ�������ܽڵ�����������Ҷ�ӽڵ��������Ч��,
*	��Ϊ��Ҷ�ӽڵ��������ڵ�ϳɵ����������Ǳ��ϳɽڵ��ǰ׺�롣
*	������ô�Ҷ�ӽڵ㣬������ҵ����ڵ㣬�γɹ��������롣
*	������ôӸ��ڵ���ҵ�Ҷ�ӽڵ��γɹ��������룬��ջӦ�ÿ���ʵ�֡� */
void CreateHuffmanCode(HMTree& hmt, unsigned int leafcount, huffmancode& hmc)
{
	//�������й����������ͷָ��
	hmc = (huffmancode)malloc(leafcount * sizeof(char*));
	//����һ�����������룬���볤������ᳬ��leafcount-1����Ϊ��ʹ����Ҷ�ӽڵ㣬
	//Ҳ�úϲ�һ�²����γɹ������������ǵı��볤�ȱ�Ϊ��1
	char* code = (char*)malloc(leafcount*sizeof(char));
	code[leafcount-1] = '\0';
	for (size_t i = 0; i < leafcount; i++)
	{
		int start = leafcount - 1;
		int parent = hmt[i].parent;
		int curnode = i;
		for (; 0!=parent; curnode=parent,parent=hmt[parent].parent)
		{
			/*	������������Ϊ�涨������·�����0���Һ���·�����1��Ҳ���Լ���ǣ�����Ҳ������ǰ׺�� */
			//�������������飬˳��ȡ��������ջ
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
