#include "swap_sort.h"
#include <time.h>

void bubble_sort(RecType& rt)
{
	int temp;
	int count = 0;
	for (int i = 0; i < rt.size(); i++)
	{
		//�Ӻ���ǰ���ν���Сֵ�ƶ���iλ��
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

	printf("�Ƚϴ���:%d\n", count);
}

//�Ľ�����������ڲ���������Ϻõ�����������Ч������
void bubble_sort_improve(RecType& rt)
{
	int temp;
	int count = 0;
	//��¼ĳ��ð�ݹ����Ƿ���й����ݽ��������һ�ζ�û�н��У�˵��i��������ݾ�Ϊ�������ݣ�i�������������ٽ���ð��
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

	printf("�Ƚϴ���:%d\n", count);
}

void quick_sort(RecType& rt, const int& low, const int& high)
{
	if (low<high)
	{
		//�ҵ��ֽ�λ�ã����ֽ�Ԫ�ط���ֽ�λ��
		int partition = partition_quick(rt, low, high);
		//���ݷֽ�λ�ý������ҷ���
		quick_sort(rt, low, partition - 1);
		quick_sort(rt, partition + 1, high);
	}
}

/*  ����Ԫ����Ϊ�ֽ�Ԫ�أ������λ�����λ��ʼ���ڲ��ҷֽ�λ��*/
int partition_quick(RecType& rt, const int& low, const int& high)
{
	int l = low;
	int h = high;
	//��ͷԪ������Ϊ�ֽ�Ԫ�أ������棬��λ�������ˣ������λ�ÿ���������βԪ���з��ֵ�С�ڷֽ�Ԫ�ص�Ԫ��
	int temp = rt[l];
	//*** �м�ÿһ����Ҫ�ж�l<h	***
	while (l<h)
	{
		//���βԪ�ش��ڵ��ڷֽ�Ԫ�أ�β���������ƶ���
		//����ͺ����whileѭ������Ӧ���ٰ���һ�����ڱȽϣ�������ڶ����ֽ�Ԫ�ص�ֵԪ��ʱ��l��h�����޷����
		//�������û�е��ڱȽϣ������βԪ����ֽ�Ԫ����ȣ�--h����ִ�У���һ��Ҳ��ִ��rt[l++] = rt[h]����βԪ���ƶ�������λ�ã�βλ������
		while (rt[h]>=temp && l<h)
			--h;
		//����С�ڷֽ�Ԫ�ص�Ԫ�أ��������ͷ������λ�ã�������βλ����Ϊ����λ��
		if (l<h)
			rt[l++] = rt[h];	//��Ϊ����λ�ñ�д����С�ڷֽ�Ԫ�ص�ֵ����Ԫ���������ƣ����ƺ�λ�õ�Ԫ�ز������Ƚ�

		//���ڴ���Ԫ�ؿ�ʼ���ң������Ԫ��С�ڵ��ڷֽ�Ԫ�أ������������ƶ�
		while (rt[l] <= temp&& l<h)
			++l;
		//���ִ��ڷֽ�Ԫ�ص�Ԫ�أ��������β������λ�ã��������ײ�λ����Ϊ����λ��
		if (l<h)
			rt[h--] = rt[l];		//��Ϊ����λ�ñ�д���˴��ڷֽ�Ԫ�ص�ֵ��βԪ���������ƣ����ƺ�λ�õ�Ԫ�ز������Ƚ�
	}

	//�ҵ��˷ֽ�λ�ã��ֽ�λ��ǰ��Ԫ�ص�ֵ���ܾ����ڷֽ�Ԫ�أ����Կ����������ڲ��ȶ�����
	rt[l] = temp;
	return l;
}

int GetRandomInt(const int min, const int max)
{
	return (rand() % (max - min + 1)) + min;
}

void TestSwapSort()
{
	//��ʼ���������
	srand((unsigned)time(NULL));

	RecType rt;
	int minkey = 1;
	int maxkey = 99;
	int count = 25;
	printf("����������У�\n");
	for (int i = 0; i < count; i++)
	{
		int key = GetRandomInt(minkey, maxkey);
		printf("%d ", key);
		rt.push_back(key);
	}

	RecType rtcopy = rt;
	printf("\n����ð������\n");
	bubble_sort(rt);
	for (size_t i = 0; i < rt.size(); i++)
	{
		printf("%d ", rt[i]);
	}

	printf("\n����ð������Ľ��棺\n");
	bubble_sort_improve(rtcopy);
	for (size_t i = 0; i < rtcopy.size(); i++)
	{
		printf("%d ", rtcopy[i]);
	}

	printf("\n\n---\n����������У�\n");
	rt.clear();
	for (int i = 0; i < count; i++)
	{
		int key = GetRandomInt(minkey, maxkey);
		printf("%d ", key);
		rt.push_back(key);
	}

	printf("\n��������\n");
	quick_sort(rt, 0, rt.size()-1);
	for (size_t i = 0; i < rt.size(); i++)
	{
		printf("%d ", rt[i]);
	}
}
