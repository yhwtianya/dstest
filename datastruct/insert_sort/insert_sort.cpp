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
			//��ǰ���������д���iԪ�ص�ֵ�Ӻ���ǰ��������ƶ�
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
		/*	���ֲ��ҷ��ҵ�ǰ���������е�һ������i��Ԫ��,ע�ⲻ���ҵ���i��Ԫ�أ�
			������Ϊ�˱�֤�����ȶ��ԡ� */
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
			//��ǰ���������д���iԪ�ص�ֵ�Ӻ���ǰ��������ƶ�
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
	//����Ԫ����СΪ1��������Ϊ1��������ֱ�Ӳ������򣬲���Ϊ1��֤�����ȫ��Ԫ��ִ����һ��ֱ�Ӳ�������
	while (d>=1)	
	{
		//�ⲿ�ֺ�ֱ�Ӳ�������Ĵ�����ȫһ�£��������ƶ�������1�ĳ���d
		for (size_t i = d; i < rt.size(); i++)
		{
			/*  Ԫ�طֳ�d�飬0��d-1��Ԫ�طֱ�Ϊ��������ĵ�һ��Ԫ�أ�
			��i=d��2d-1ÿ��Ԫ������ǰ�����ĵ�2��Ԫ�أ���i=2d��3d-1......			*/
			temp = rt[i];
			int j = i - d;	//������һ��Ԫ��
			for (; j >= 0 && temp < rt[j]; j-=d)
			{
				//������ǰ���������д���iԪ�ص�ֵ�Ӻ���ǰ��������ƶ�d
				rt[j + d] = rt[j];
			}
			rt[j + d] = temp;
		}

		//���Ը��ݾ���ֵ��������Ԫ�صݼ��㷨
		//����ȥ��ȡ��
		d = d / 2;
	}
}

int GetRandomInt(const int min, const int max)
{
	return (rand() % (max - min + 1)) + min;
}

void TestInsertSort()
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

	sinsert_sort(rt);
	printf("\nֱ�Ӳ�������\n");
	for (size_t i = 0; i < rt.size(); i++)
	{
		printf("%d ", rt[i]);
	}

	printf("\n\n---\n����������У�\n");
	rt.clear();
	for (int i = 0; i < count; i++)
	{
		int key = GetRandomInt(minkey, maxkey);
		printf("%d ", key);
		rt.push_back(key);
	}

	binsert_sort(rt);
	printf("\n�۰��������\n");
	for (size_t i = 0; i < rt.size(); i++)
	{
		printf("%d ", rt[i]);
	}

	printf("\n\n---\n����������У�\n");
	rt.clear();
	for (int i = 0; i < count; i++)
	{
		int key = GetRandomInt(minkey, maxkey);
		printf("%d ", key);
		rt.push_back(key);
	}

	shell_sort(rt);
	printf("\nϣ������\n");
	for (size_t i = 0; i < rt.size(); i++)
	{
		printf("%d ", rt[i]);
	}
}
