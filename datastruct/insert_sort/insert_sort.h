#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
using namespace std;

/*	插入排序法
	在一个已排好序的记录子集的基础上，每一步将下一个待排序
	的记录有序插入到已排好序的记录子集中，直到将所有待排记录全部插入为止

	直接插入排序
	直接插入排序是一种最基本的插入排序方法，其基本操作是将第 i 个记录插入
	到前面 i-1 个已排好序的记录中。具体过程为：将第 i 个记录的关键字 K i ，顺次与其前面记
	录的关键字 K i-1 ，K i-2 ，…, K 1 进行比较，将所有关键字大于 K i 的记录依次向后移动一个位
	置，直到遇见一个关键字小于或者等于 K i 的记录 K j ，此时 K j 后面必为空位置，将第 i 个记
	录插入空位置即可。完整的直接插入排序是从 i=2 开始，也就是说，将第 1 个记录视为已排
	好序的单元素子集合，然后将第二个记录插入到单元素子集合中。i 从 2 循环到 n，即可实
	现完整的直接插入排序。
	直接插入排序算法时间复杂度是O(n 2 )
	直接插入排序算法属于稳定排序

	折半排序法
	采用折半插入排序法，可减少关键字的比较次数。每插入一个元素，需要比较
	的次数最大为折半判定树的深度。虽然折半插入排序法与直接插入排序法相比较，改善了算法中比较次数的数量级为
	O(nlog 2 n)，但其并未改变移动元素的时间耗费，所以折半插入排序的总的时间复杂度仍然是
	O(n 2 )
	折半排序算法属于稳定排序

	希尔排序又称缩小增量排序法
	直接插入排序，折半排序，移动数据时每次仅移动一位，希尔排序，根据增量元素序列对元素跳跃分组，
	增量元素有多个，每次根据一个增量元素的值n进行分组，分成n组，然后分组内进行直接插入排序。
	增量元素序列内元素数值递减，最后增量元素必须为1
	分组内部排序时相当于增加了移动数据时每次移动的步幅，增加排序效率。
	增量元素序列的选择是一个经验值，根据排序元素个数，不同的增量元素序列对排序效率的影响不同。
	希尔排序算法时间复杂度是O(n 1.5 )
	希尔排序算法属于不稳定排序
	*/
 
typedef vector<int>	RecType;
//获取随机数
int GetRandomInt(const int min, const int max);
//直接插入排序
void sinsert_sort(RecType& rt);
//折半插入排序
void binsert_sort(RecType& rt);
//希尔排序
void shell_sort(RecType& rt);


void TestInsertSort();