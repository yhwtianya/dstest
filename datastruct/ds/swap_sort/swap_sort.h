#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
using namespace std;

/*	交换排序法是一类通过交换逆序元素进行排序的方法
	
	冒泡排序
	冒泡排序是一种简单的交换类排序方法，它是通过相邻的数据元素的交换，逐步将待排
	序序列变成有序序列的过程
	冒泡排序的时间复杂度为 O(n 2 )
	冒泡排序属于稳定排序

	快速排序 又名划分交换排序
	冒泡排序中，由于扫描过程中只对相邻的两个元素进行比较，因此在互换两个相邻元素时只能消除一个逆序。
	冒泡排序中比较过的数据在下一遍比较时可能还会进行比较，造成总比较和移动次数较多。
	快速排序是典型的分治思想，将数据分组，组内无序组间有序，子组再应用快速排序，减少比较次数和移动次数
	快速排序的最好时间复杂度为 O(nlog2 n )
	快速排序的平均时间复杂度为 O(nlog2 n )
	快速排序的最坏时间复杂度为 O(n2 )
	快速排序属于不稳定排序
	*/
 
typedef vector<int>	RecType;
//获取随机数
int GetRandomInt(const int min, const int max);
//冒泡排序（上升法）
void bubble_sort(RecType& rt);
//冒泡排序改进
void bubble_sort_improve(RecType& rt);
//快速排序
void quick_sort(RecType& rt);
//快速排序查找并设置分界
int partition_quick(RecType& rt,const int& low, const int& high);


void TestSwapSort();