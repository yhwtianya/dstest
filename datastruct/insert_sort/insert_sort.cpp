#include "insert_sort.h"
#include <time.h>

void sinsert_sort(RecType& rt)
{
	int temp;
	for (size_t i = 1; i < rt.size(); i++)
	{
		temp = rt[i];
		int j = i - 1;
		for (; j >= 0 && temp < rt[j]; j--)
		{
			//将前面有序序列大于i元素的值从后向前依次向后移动
			rt[j + 1] = rt[j];
		}
		rt[j + 1] = temp;
	}
}

void binsert_sort(RecType& rt)
{
	int temp;
	int low, high, mid;
	for (size_t i = 1; i < rt.size(); i++)
	{
		temp = rt[i];
		low = 0;
		high = i-1;
		/*	二分查找法找到前面有序序列第一个大于i的元素,注意不是找等于i的元素，
			这样是为了保证排序稳定性。 */
		while (low<=high)
		{
			mid = (low + high) / 2;
			if (temp < rt[mid])
				high = mid - 1;
			else
				low = mid + 1;
		}

		for (int j=i-1; j >=low; j--)
		{
			//将前面有序序列大于i元素的值从后向前依次向后移动
			rt[j+1] = rt[j];
		}
		rt[low] = temp;
	}
}

void shell_sort(RecType& rt)
{
	//
	int d = rt.size() / 2;
	int temp;
	//增量元素最小为1，即步幅为1，即进行直接插入排序，步幅为1保证了最后全部元素执行了一次直接插入排序
	while (d>=1)	
	{
		//这部分和直接插入排序的代码完全一致，仅仅将移动步幅由1改成了d
		for (size_t i = d; i < rt.size(); i++)
		{
			/*  元素分成d组，0到d-1个元素分别为其所在组的第一个元素，
			从i=d到2d-1每个元素属于前面分组的第2个元素，从i=2d到3d-1......			*/
			temp = rt[i];
			int j = i - d;	//该组上一个元素
			for (; j >= 0 && temp < rt[j]; j-=d)
			{
				//将该组前面有序序列大于i元素的值从后向前依次向后移动d
				rt[j + d] = rt[j];
			}
			rt[j + d] = temp;
		}

		//可以根据经验值更改增量元素递减算法
		//除法去余取整
		d = d / 2;
	}
}

int GetRandomInt(const int min, const int max)
{
	return (rand() % (max - min + 1)) + min;
}

void TestInsertSort()
{
	//初始化随机种子
	srand((unsigned)time(NULL));

	RecType rt;
	int minkey = 1;
	int maxkey = 99;
	int count = 25;
	printf("随机生成序列：\n");
	for (int i = 0; i < count; i++)
	{
		int key = GetRandomInt(minkey, maxkey);
		printf("%d ", key);
		rt.push_back(key);
	}

	sinsert_sort(rt);
	printf("\n直接插入排序：\n");
	for (size_t i = 0; i < rt.size(); i++)
	{
		printf("%d ", rt[i]);
	}

	printf("\n\n---\n随机生成序列：\n");
	rt.clear();
	for (int i = 0; i < count; i++)
	{
		int key = GetRandomInt(minkey, maxkey);
		printf("%d ", key);
		rt.push_back(key);
	}

	binsert_sort(rt);
	printf("\n折半插入排序：\n");
	for (size_t i = 0; i < rt.size(); i++)
	{
		printf("%d ", rt[i]);
	}

	printf("\n\n---\n随机生成序列：\n");
	rt.clear();
	for (int i = 0; i < count; i++)
	{
		int key = GetRandomInt(minkey, maxkey);
		printf("%d ", key);
		rt.push_back(key);
	}

	shell_sort(rt);
	printf("\n希尔排序：\n");
	for (size_t i = 0; i < rt.size(); i++)
	{
		printf("%d ", rt[i]);
	}
}
