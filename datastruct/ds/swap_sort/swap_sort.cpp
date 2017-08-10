#include "swap_sort.h"
#include <time.h>

void bubble_sort(RecType& rt)
{
	int temp;
	int count = 0;
	for (int i = 0; i < rt.size(); i++)
	{
		//从后向前依次将最小值移动到i位置
		for (int j = rt.size()-2; j>=i; j--)
		{
			if (rt[j+1] < rt[j])
			{
				temp = rt[j];
				rt[j] = rt[j+1];
				rt[j+1] = temp;
			}
			++count;
		}
	}

	printf("比较次数:%d\n", count);
}

//改进版对于数据内部升序排序较好的序列有提升效率作用
void bubble_sort_improve(RecType& rt)
{
	int temp;
	int count = 0;
	//记录某次冒泡过程是否进行过数据交换，如果一次都没有进行，说明i后面的数据均为有序数据，i后面数据无需再进行冒泡
	bool IsChange;	
	for (int i = 0; i < rt.size(); i++)
	{
		IsChange = false;
		for (int j = rt.size() - 2; j >= i; j--)
		{
			if (rt[j + 1] < rt[j])
			{
				temp = rt[j];
				rt[j] = rt[j + 1];
				rt[j + 1] = temp;
				IsChange = true;
			}
			++count;
		}

		if (!IsChange)
			break;
	}

	printf("比较次数:%d\n", count);
}

void quick_sort(RecType& rt, const int& low, const int& high)
{
	if (low<high)
	{
		//找到分界位置，将分界元素放入分界位置
		int partition = partition_quick(rt, low, high);
		//根据分界位置进行左右分治
		quick_sort(rt, low, partition - 1);
		quick_sort(rt, partition + 1, high);
	}
}

/*  以首元素作为分界元素，从最低位和最高位开始向内查找分界位置*/
int partition_quick(RecType& rt, const int& low, const int& high)
{
	int l = low;
	int h = high;
	//将头元素设置为分界元素，并保存，此位置留空了，后序此位置可用来保存尾元素中发现的小于分界元素的元素
	int temp = rt[l];
	//*** 切记每一步都要判断l<h	***
	while (l<h)
	{
		//如果尾元素大于等于分界元素，尾座标向左移动。
		//这里和后面的while循环条件应至少包含一个等于比较，否则存在多个与分界元素等值元素时，l和h可能无法汇合
		//如果这里没有等于比较，且如果尾元素与分界元素相等，--h不会执行，下一句也会执行rt[l++] = rt[h]，将尾元素移动到了首位置，尾位置留空
		while (rt[h]>=temp && l<h)
			--h;
		//发现小于分界元素的元素，将其放入头部留空位置，并将此尾位置作为留空位置
		if (l<h)
			rt[l++] = rt[h];	//因为留空位置被写入了小于分界元素的值，首元素坐标右移，右移后位置的元素参与后序比较

		//现在从首元素开始查找，如果首元素小于等于分界元素，首坐标向右移动
		while (rt[l] <= temp&& l<h)
			++l;
		//发现大于分界元素的元素，将其放入尾部留空位置，并将此首部位置作为留空位置
		if (l<h)
			rt[h--] = rt[l];		//因为留空位置被写入了大于分界元素的值，尾元素坐标左移，左移后位置的元素参与后序比较
	}

	//找到了分界位置，分界位置前后元素的值可能均等于分界元素，所以快速排序属于不稳定排序
	rt[l] = temp;
	return l;
}

int GetRandomInt(const int min, const int max)
{
	return (rand() % (max - min + 1)) + min;
}

void TestSwapSort()
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

	RecType rtcopy = rt;
	printf("\n上升冒泡排序：\n");
	bubble_sort(rt);
	for (size_t i = 0; i < rt.size(); i++)
	{
		printf("%d ", rt[i]);
	}

	printf("\n上升冒泡排序改进版：\n");
	bubble_sort_improve(rtcopy);
	for (size_t i = 0; i < rtcopy.size(); i++)
	{
		printf("%d ", rtcopy[i]);
	}

	printf("\n\n---\n随机生成序列：\n");
	rt.clear();
	for (int i = 0; i < count; i++)
	{
		int key = GetRandomInt(minkey, maxkey);
		printf("%d ", key);
		rt.push_back(key);
	}

	printf("\n快速排序：\n");
	quick_sort(rt, 0, rt.size()-1);
	for (size_t i = 0; i < rt.size(); i++)
	{
		printf("%d ", rt[i]);
	}
}
