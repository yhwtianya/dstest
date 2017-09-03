#pragma  once
#include <vector>
#include <iostream>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cmath>
#include "pq_if.h"
using namespace std;
#define NODE_CHAR_WIDE		2

#ifdef _DEBUG
//#define PQ_COMPLHEAP_DEBUG
#endif

/*
���ȶ��У���һ�ָ���һ�����ȼ��洢��ȡ�����ݵĶ��С�������˵�Ƕ��к��������������壬�������Դ洢���ݣ�
�����Խ���Щ���ݰ��������趨�Ĺ����������

�����ݽṹ��һ��������������Ա���Ϊһ����ȫ�������ṹ��
�����ص��Ǹ��ڵ��ֵ���ڣ�С�ڣ������ӽڵ��ֵ���ֱ��Ϊ�󶥶Ѻ�С���ѣ���
�������ڹ����㷨ִ�й����е���Ϣ��Ӧ�ó����������������ȶ��еȡ�

���ȶ��в��������ȶѣ�Ҳ���Ƕѡ�
���ȶ���ֻ�Ǵ洢���ݺ�����Ľ�ϣ���������ѧ����֪ʶ������������ʵ�ַ��������顢�����������������Ȼ���жѡ�
������������ɾ��Ч�ʲ��ߣ��ʲ����á�
����������Ǹ߼����ݽṹ�������ɾ��Ч�ʸߣ�����ɾ������ֱ�ӿ���ʵ��ȫ���򣬵����������ȶ�����Щ�˷�

ƫ��Ҫ�󸸱ش����ӣ�����֮��Ĵ�С��ϵû��ǿ��Ҫ��
�ѵĲ����ɾ��Ч�ʱȶ��������ߣ����ҽ���ʵ����ƫ��ƫ����Ѿ����������ȶ��е�Ҫ�󣬹�һ����ö�
*/

typedef int Rank;
template <class T>
class PQ_ComplHeap : public PQ<T>, public vector<T>
{
public:
	PQ_ComplHeap();
	//�����鹹��PQ
	PQ_ComplHeap(T* A, Rank n);
	//��ȡ����Ԫ�ز�ɾ����������СԪ�أ�
	T popPQ();
	//����Ԫ��
	void pushPQ(T item);
	//��ȡ����Ԫ�أ�������СԪ�أ�
	T topPQ();
	//���д�С
	int sizePQ();
	//�Ƿ�Ϊ��
	bool emptyPQ();
	//�������PQ����
	void printPQ(ostream& os=cout);
	//�����鹹��PQ
	void creatPQ(T* A, Rank n);
	//������
	void heapSort();
	//�����鹹��PQ��������
	void heapSort(T* A, Rank n);
	//˳�����Ԫ�ص�ָ����
	void printOrder(ostream& os = cout);
	//�ѻ�
	void heapify();

protected:
	//��iԪ������
	Rank percolateUp(Rank i);
	//��iԪ��ִ�����ʣ�nΪԪ��������
	Rank percolateDown(Rank n, Rank i);
	//������A������������[lo, hi)
	void copyFrom(T* A, Rank lo, Rank hi);
	//��ȡ������
	static Rank Parent(Rank i) { return (i - 1) >> 1; }
	//��ȡ��������
	static Rank LChild(Rank i) { return 1 + (i << 1); }
	//��ȡ�Һ�������
	static Rank RChild(Rank i) { return (i + 1) << 1; }
	//��ȡ�������ӡ����������Ԫ�ص�����
	Rank maxParentChilds(Rank n, Rank p);

protected:
	//��ȡ�����
	int getNodeDepth();
	//��ȡi�����Ĳ��
	int getLevel(Rank i);
	//���ָ�������Ŀո�
	void printSpace(float fcount, ostream& os = cout);
	//�ڵ����ռ�ÿ��
	int getItemWide();

protected:
	//�ڵ�ռ������Ŀ��
	int _item_wide;

};

template <class T>
void PQ_ComplHeap<T>::heapSort(T* A, Rank n)
{
	creatPQ(A, n);
	heapSort();
}

/*	������
	��ѡ�������У��������ȶ��е����ԣ���Ч�ѻ���������ѡ����󣨻���С��Ԫ�ؽ�������
	�������ظ����ö�����ռ䣬�ڻ�ȡ��󣨻���С��Ԫ��ʱ������ͬʱ��������������
	��˶�������ʱ��Ϳռ临�Ӷ��϶��кܴ����ơ�
*/
template <class T>
void PQ_ComplHeap<T>::heapSort()
{
	//�����ظ�����vector������ռ䣬���ν���PQ���׺Ͷ�βԪ�أ���β���������Ľ��
	//Ȼ����¶���Ԫ��ִ�����ˣ�ά��PQ����
	//�����Ӷ�β���������α�������PQ�������ʱ��vector�ڱ���ľ��������Ľ����
	for (int i = 0; i < vector::size(); i++)
	{
		Rank posEnd = vector::size() - i - 1;
		T keyEnd = vector::at(posEnd);
		vector::at(posEnd) = vector::at(0);
		vector::at(0) = keyEnd;
		//���¶���ִ�����ˣ�ά��PQ����
		percolateDown(posEnd, 0);
	}
}

template <class T>
void PQ_ComplHeap<T>::creatPQ(T* A, Rank n)
{
	copyFrom(A, 0, n);
	heapify();
}

template <class T>
PQ_ComplHeap<T>::PQ_ComplHeap(T* A, Rank n)
{
	_item_wide = getItemWide();
	creatPQ(A, n);
}

template <class T>
Rank PQ_ComplHeap<T>::maxParentChilds(Rank n, Rank p)
{
	Rank lc = LChild(p);
	Rank rc = RChild(p);
	Rank maxInd = p;
	if (lc < n && rc < n)
	{
		if (vector::at(lc) > vector::at(rc))
			maxInd = lc;
		else
			maxInd = rc;
	}
	else if (lc < n)
	{
		maxInd = lc;
	}
	else if (rc < n)
	{
		maxInd = rc;
	}

	if (vector::at(maxInd) > vector::at(p))
		return maxInd;
	else
		return p;
}

//���ˣ���iԪ��������Ͻ��бȽϣ�ֱ��iԪ�ص�����·����ȫ�����㸸�����ӵ�����Ϊֹ
template <class T>
Rank PQ_ComplHeap<T>::percolateUp(Rank i)
{
	T temp = vector::at(i);
	int j = Parent(i);
	while (j>=0 && temp>vector::at(j))
	{
		vector::at(i) = vector::at(j);
		i = j;
		j = Parent(i);
	}
	vector::at(i) = temp;
	return i;
}

//���ˣ���iԪ��������½��бȽϣ�ֱ��iԪ�ص�ҳ�ڵ��·����ȫ�����㸸�����ӵ�����Ϊֹ
template <class T>
Rank PQ_ComplHeap<T>::percolateDown(Rank n, Rank i)
{
	if (0 == n)
		return -1;

	Rank j = maxParentChilds(n, i);
	while (j<n && j!=i)
	{
		T temp = vector::at(i);
		vector::at(i) = vector::at(j);
		vector::at(j) = temp;
		i = j;
		j = maxParentChilds(n, i);
	}

	return j;
}

template <class T>
void PQ_ComplHeap<T>::heapify()
{
	if (vector::size() < 2)
		return;

	//��ȡ���ڵ�ĸ��ڵ㣬�����һ����ҳ�ڵ�
	Rank last_f_leaf = Parent(vector::size()-1);
	//�����һ����ҳ�ڵ㣨������������������һ�룩�������ν������ˣ��ɵ������𲽽���PQ����
	//�ɵ����ϵĴ�˳�������ϴ��Ѿ�������PQ���ԣ����ӿ����㷨Ч��
	for (Rank i = last_f_leaf; i >=0; i--)
		percolateDown(vector::size(), i);
}

template <class T>
void PQ_ComplHeap<T>::copyFrom(T* A, Rank lo, Rank hi)
{
	vector::clear();
	vector::assign(A + lo, A + hi);
}

template <class T>
bool PQ_ComplHeap<T>::emptyPQ()
{
	return vector::empty();
}

template <class T>
int PQ_ComplHeap<T>::sizePQ()
{
	return vector::size();
}

template <class T>
T PQ_ComplHeap<T>::topPQ()
{
	return vector::front();
}

template <class T>
T PQ_ComplHeap<T>::popPQ()
{
	//������Ԫ�ر���
	T temp = vector::front();
	//����βԪ���ƶ�����
	*vector::begin() = *vector::rbegin();
	//ɾ���˶�βԪ��
	vector::pop_back();
	//���ˣ����¶���Ԫ��������½��бȽϣ�ֱ���¶���Ԫ�ص�ҳ�ڵ��·����ȫ�����㸸�����ӵ�����Ϊֹ
	percolateDown(vector::size(), 0);
	return temp;
}

template <class T>
void PQ_ComplHeap<T>::pushPQ(T item)
{
	//��������Ԫ������ӵ�ĩβ������������֮ǰ����Ԫ�ص�PQ����
	vector::push_back(item);
	//���ˣ���βԪ��������Ͻ��бȽϣ�ֱ��βԪ�ص�����·����ȫ�����㸸�����ӵ�����Ϊֹ
	percolateUp(vector::size()-1);
}

template <class T>
PQ_ComplHeap<T>::PQ_ComplHeap()
{
	_item_wide = getItemWide();
}

template <class T>
int PQ_ComplHeap<T>::getItemWide()
{
	return NODE_CHAR_WIDE + 1;
}

template <class T>
void PQ_ComplHeap<T>::printSpace(float fcount, ostream& os /*= cout*/)
{
	//����fcount������Ϊ���Ԫ��ռ�õĿռ����룬���Բ��ø������ͣ�����int�޷����0.5��Ԫ��ռ�õĿռ�
	for (int i = 0; i < int(fcount*_item_wide); i++)
		os << " ";
}

template <class T>
int PQ_ComplHeap<T>::getLevel(Rank i)
{
	if (i <= 0)
		return 0;

	return log2(i) / 1 + 1;
}

template <class T>
int PQ_ComplHeap<T>::getNodeDepth()
{
	return getLevel(vector::size());
}

template <class T>
void PQ_ComplHeap<T>::printPQ(ostream& os/*=cout*/)
{
	//�������
	int depth = getNodeDepth();
	//��ǰ�ڵ�㼶
	int level = 0;
	//���ÿ��ڵ���Ϣ
	for (int i = 0; i < vector::size(); i = pow(2, level + 1) - 1)
	{
		level = getLevel(i);
		//ÿ����������֦����ʾ������ϵ
		if (0 != level)
			os << endl;

		//��ǰ��ĸ߶�
		int high = depth - level;

		//��ǰ��Ԫ��֮��Ŀ�Ԫ������
		int inter_space_item_count;

		if (high == 1)
		{
			//��Ͳ�Ԫ�ؼ�ո���Ϊ0����Ϊ�ڵ����ʱ�Դ�һ���ո�
			inter_space_item_count = 0;
		}
		else
		{
			//�ɹ۲�ó������ͨ�ʽ,2������������ڵ㺢������
			inter_space_item_count = pow(2, (high - 1)) - 1;
		}

		/*��ǰ�㿪ʼ���Ŀհ�Ԫ�ظ���
		��ʵд����ʱͨ����ͼ,���ý�Ϊ3���ó���λ��Ԫ�ظ�������Ϊ0,1,4,13,40,......
		Ԫ�ؼ䲻���ɵȱȣ�����Ԫ�ز�ɵȱȣ��õ�ͨ�ʽΪAn = (pow(order,n) - 1) / 2,
		�����Ӳ���������Ԫ�ؼ��Ԫ��������ͨ�ʽ��ͬ���õ���ʽ���׿�Ԫ���� = Ԫ�ؼ��Ԫ���� / 2.
		*/
		float begin_space_item_count;
		begin_space_item_count = 1.0*inter_space_item_count / 2;
		//�����ǰ���һ��Ԫ��ǰ�Ŀո�
		printSpace(begin_space_item_count, os);
		int nStart = pow(2, level) - 1;
		int nEnd = pow(2, level+1) - 1;
		nEnd = vector::size() < nEnd ? vector::size() : nEnd;

		for (int i = nStart; i < nEnd; i++)
		{
			T key = vector::at(i);
			os << left << setw(_item_wide) << key << right;
			if(i+1 != nEnd)
				printSpace(inter_space_item_count, os);
		}
		os << endl;
	}
}

template <class T>
void PQ_ComplHeap<T>::printOrder(ostream& os /*= cout*/)
{
	vector::iterator it = begin();
	for (; it != end(); ++it)
	{
		os << *it;
		if ((it + 1) != end())
			os << ", ";
	}
	os << endl;
}

void testPQ_ComlHeap();
void reappear_auto();
void reappear_auto2();
void reappear_manual();
template<class T>
void printVector(const vector<T>& record, ostream& os=cout);
template<class T>
void saveDump(const vector<T>& record);


