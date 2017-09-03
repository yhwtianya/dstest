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
优先队列，是一种根据一定优先级存储和取出数据的队列。它可以说是队列和排序的完美结合体，不仅可以存储数据，
还可以将这些数据按照我们设定的规则进行排序。

堆数据结构是一种数组对象，它可以被视为一棵完全二叉树结构。
它的特点是父节点的值大于（小于）两个子节点的值（分别称为大顶堆和小顶堆）。
它常用于管理算法执行过程中的信息，应用场景包括堆排序，优先队列等。

优先队列并不是优先堆，也不是堆。
优先队列只是存储数据和排序的结合，根据我们学过的知识，可以有以下实现方法：数组、链表、二叉查找树，当然还有堆。
数组和链表插入删除效率不高，故不采用。
二叉查找树是高级数据结构，插入和删除效率高，插入删除过程直接可以实现全排序，但用来做优先队列有些浪费

偏序：要求父必大于子，但子之间的大小关系没有强制要求。
堆的插入和删除效率比二叉树还高，而且仅仅实现了偏序，偏序就已经满足了优先队列的要求，故一般采用堆
*/

typedef int Rank;
template <class T>
class PQ_ComplHeap : public PQ<T>, public vector<T>
{
public:
	PQ_ComplHeap();
	//从数组构造PQ
	PQ_ComplHeap(T* A, Rank n);
	//获取顶端元素并删除（最大或最小元素）
	T popPQ();
	//插入元素
	void pushPQ(T item);
	//获取顶端元素（最大或最小元素）
	T topPQ();
	//队列大小
	int sizePQ();
	//是否为空
	bool emptyPQ();
	//树形输出PQ到流
	void printPQ(ostream& os=cout);
	//从数组构建PQ
	void creatPQ(T* A, Rank n);
	//堆排序
	void heapSort();
	//从数组构建PQ并堆排序
	void heapSort(T* A, Rank n);
	//顺序输出元素到指定流
	void printOrder(ostream& os = cout);
	//堆化
	void heapify();

protected:
	//对i元素上滤
	Rank percolateUp(Rank i);
	//对i元素执行下率，n为元素总数量
	Rank percolateDown(Rank n, Rank i);
	//从数组A拷贝数据区间[lo, hi)
	void copyFrom(T* A, Rank lo, Rank hi);
	//获取父索引
	static Rank Parent(Rank i) { return (i - 1) >> 1; }
	//获取左孩子索引
	static Rank LChild(Rank i) { return 1 + (i << 1); }
	//获取右孩子索引
	static Rank RChild(Rank i) { return (i + 1) << 1; }
	//获取父、左子、右子中最大元素的索引
	Rank maxParentChilds(Rank n, Rank p);

protected:
	//获取堆深度
	int getNodeDepth();
	//获取i索引的层次
	int getLevel(Rank i);
	//输出指定数量的空格
	void printSpace(float fcount, ostream& os = cout);
	//节点输出占用宽度
	int getItemWide();

protected:
	//节点占用输出的宽度
	int _item_wide;

};

template <class T>
void PQ_ComplHeap<T>::heapSort(T* A, Rank n)
{
	creatPQ(A, n);
	heapSort();
}

/*	堆排序
	在选择排序中，利用优先队列的特性（高效堆化），快速选择最大（或最小）元素进行排序。
	堆排序重复利用堆数组空间，在获取最大（或最小）元素时，数组同时保存了排序结果。
	因此堆排序在时间和空间复杂度上都有很大优势。
*/
template <class T>
void PQ_ComplHeap<T>::heapSort()
{
	//这里重复利用vector的数组空间，依次交换PQ队首和队尾元素，队尾保存排序后的结果
	//然后对新队首元素执行下滤，维护PQ特性
	//这样从队尾到队首依次遍历，将PQ遍历完成时，vector内保存的就是排序后的结果。
	for (int i = 0; i < vector::size(); i++)
	{
		Rank posEnd = vector::size() - i - 1;
		T keyEnd = vector::at(posEnd);
		vector::at(posEnd) = vector::at(0);
		vector::at(0) = keyEnd;
		//对新队首执行下滤，维护PQ特性
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

//上滤：对i元素逐层向上进行比较，直到i元素到根的路径上全部满足父大于子的特性为止
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

//下滤：对i元素逐层向下进行比较，直到i元素到页节点的路径上全部满足父大于子的特性为止
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

	//获取最后节点的父节点，即最后一个非页节点
	Rank last_f_leaf = Parent(vector::size()-1);
	//从最后一个非页节点（这样遍历次数缩减了一半）到根依次进行下滤，由底向上逐步建立PQ特性
	//由底向上的次顺利用了上次已经建立的PQ特性，大大加快了算法效率
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
	//将队首元素保存
	T temp = vector::front();
	//将队尾元素移动队首
	*vector::begin() = *vector::rbegin();
	//删除此队尾元素
	vector::pop_back();
	//下滤：对新队首元素逐层向下进行比较，直到新队首元素到页节点的路径上全部满足父大于子的特性为止
	percolateDown(vector::size(), 0);
	return temp;
}

template <class T>
void PQ_ComplHeap<T>::pushPQ(T item)
{
	//将待插入元素先添加到末尾，这样保持了之前所有元素的PQ特性
	vector::push_back(item);
	//上滤：对尾元素逐层向上进行比较，直到尾元素到根的路径上全部满足父大于子的特性为止
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
	//由于fcount经常作为半个元素占用的空间输入，所以采用浮点类型，否则int无法表达0.5个元素占用的空间
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
	//树的深度
	int depth = getNodeDepth();
	//当前节点层级
	int level = 0;
	//输出每层节点信息
	for (int i = 0; i < vector::size(); i = pow(2, level + 1) - 1)
	{
		level = getLevel(i);
		//每层后面输出树枝，表示归属关系
		if (0 != level)
			os << endl;

		//当前层的高度
		int high = depth - level;

		//当前层元素之间的空元素数量
		int inter_space_item_count;

		if (high == 1)
		{
			//最低层元素间空格数为0，因为节点输出时自带一个空格
			inter_space_item_count = 0;
		}
		else
		{
			//由观察得出的这个通项公式,2代表阶数，即节点孩子数量
			inter_space_item_count = pow(2, (high - 1)) - 1;
		}

		/*当前层开始处的空白元素个数
		其实写代码时通过画图,设置阶为3，得出首位空元素个数数列为0,1,4,13,40,......
		元素间不构成等比，相邻元素差构成等比，得到通项公式为An = (pow(order,n) - 1) / 2,
		而分子部分正好与元素间空元素数量的通项公式相同，得到公式：首空元素数 = 元素间空元素数 / 2.
		*/
		float begin_space_item_count;
		begin_space_item_count = 1.0*inter_space_item_count / 2;
		//输出当前层第一个元素前的空格
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


