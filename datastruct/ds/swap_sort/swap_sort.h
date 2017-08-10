#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
using namespace std;

/*	����������һ��ͨ����������Ԫ�ؽ�������ķ���
	
	ð������
	ð��������һ�ּ򵥵Ľ��������򷽷�������ͨ�����ڵ�����Ԫ�صĽ������𲽽�����
	�����б���������еĹ���
	ð�������ʱ�临�Ӷ�Ϊ O(n 2 )
	ð�����������ȶ�����

	�������� �������ֽ�������
	ð�������У�����ɨ�������ֻ�����ڵ�����Ԫ�ؽ��бȽϣ�����ڻ�����������Ԫ��ʱֻ������һ������
	ð�������бȽϹ�����������һ��Ƚ�ʱ���ܻ�����бȽϣ�����ܱȽϺ��ƶ������϶ࡣ
	���������ǵ��͵ķ���˼�룬�����ݷ��飬���������������������Ӧ�ÿ������򣬼��ٱȽϴ������ƶ�����
	������������ʱ�临�Ӷ�Ϊ O(nlog2 n )
	���������ƽ��ʱ�临�Ӷ�Ϊ O(nlog2 n )
	����������ʱ�临�Ӷ�Ϊ O(n2 )
	�����������ڲ��ȶ�����
	*/
 
typedef vector<int>	RecType;
//��ȡ�����
int GetRandomInt(const int min, const int max);
//ð��������������
void bubble_sort(RecType& rt);
//ð������Ľ�
void bubble_sort_improve(RecType& rt);
//��������
void quick_sort(RecType& rt);
//����������Ҳ����÷ֽ�
int partition_quick(RecType& rt,const int& low, const int& high);


void TestSwapSort();