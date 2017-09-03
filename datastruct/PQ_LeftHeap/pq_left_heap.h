#pragma  once
#include <vector>
#include <iostream>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cmath>
#include "../PQ_ComplHeap/pq_if.h"
#include "left_heap_node.h"
using namespace std;
#define NODE_CHAR_WIDE		2

#ifdef _DEBUG
//#define PQ_COMPLHEAP_DEBUG
#endif

/*左式堆（Leftist Heaps)又称作最左堆、左倾堆、左偏树、左偏堆。左式堆作为堆的一种，保留了堆的一些属性。
第1，左式堆仍然以二叉树的形式构建；第2，满足堆序性，左式堆的任意结点的值比其子树任意结点值均大（或小）。
但和一般的二叉堆不同，左式堆不再是一棵完全二叉树（Complete tree)，而且是一棵极不平衡的树

优先队列中涉及到"对两个优先队列进行合并"的问题时，二叉堆的效率就不太令人满意，
而基于左式堆实现的优先队列可以高效的实现两个优先队列的合并（含最大元素所在堆的logn次），
而且基于左式堆的优先队列的插入删除元素操作都可以基于合并操作完成。

左式堆是一棵二叉树，它的节点除了和二叉树的节点一样具有左右子树指针外，还有一个额外属性NPL：空节点路径长度。
空节点路径长度：从一个节点到一个"最近的空节点（假想的空节点）"的路径长度，切记是最近的。
空节点NPL为0，叶子节点的NPL为1。
NPL(x)也代表节点x到外部节点的最短距离，也代表以x为根的最大满子树的高度。
左式堆的定义就是基于NPL定义的
性质：
	左倾：对于任何内节点x，都有 NPL(lc(x)) >= NPL(rc(x))
推论：对于任何内节点x，都有 NPL(x) = 1 + NPL(rc(x))

由于左式堆不满足结构上是完全二叉树形式（极不平衡），存储结构上不是连续的，因此选择二叉树作为存储结构更高效。

基于左式堆的优先队列时间复杂度：插入O(logn)，删除O(logn)，检索O(1)
如果两个堆合并，最大元素值较大的堆的节点数为n，合并的时间复杂度O(logn)
*/

typedef int Rank;
template <class T>
class PQ_LeftHeap : public PQ<T>
{
public:
	PQ_LeftHeap();
	~PQ_LeftHeap();
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
	//清除树
	void destroy();
	//将队列r合并到本队列，完成后r全部元素剪切到本队列
	void mergeLeftHeap(PQ_LeftHeap<T>& r);

protected:
	//释放子树空间
	void freeNode(LeftHeapNode<T> *& node);
	//获取子树深度
	int getNodeDepth(LeftHeapNode<T> *& node);
	//先序遍历树，获得每层的节点信息
	void PreOrderGetTreeLevelNode(LeftHeapNode<T> * node, int level, int depth, vector<vector<LeftHeapNode<T>*> >& vItems);
	//输出节点信息
	void printNode(LeftHeapNode<T> * node, ostream& os = cout);
	//输出指定数量的空格
	void printSpace(float fcount, ostream& os = cout);
	//获得节点占用输出的宽度
	int getItemWide();
	//合并两个左式堆
	static LeftHeapNode<T>* mergeLeftHeapTree(LeftHeapNode<T>* l, LeftHeapNode<T>* r);
	//交换左式堆节点
	static void swapLeftHeapNode(LeftHeapNode<T>*& l, LeftHeapNode<T>*& r);

protected:
	//队列大小
	unsigned int _size;
	//节点占用输出的宽度
	int _item_wide;
	//左式堆根
	LeftHeapNode<T>* _root;
};

//将队列r合并到本队列，完成后r全部元素剪切到本队列
template <class T>
void PQ_LeftHeap<T>::mergeLeftHeap(PQ_LeftHeap<T>& r)
{
	_size += r._size;
	_root = mergeLeftHeapTree(_root, r._root);
	if (_root)
		_root->parent = NULL;
	r._root = NULL;
	r._size = 0;
}


template <class T>
void PQ_LeftHeap<T>::swapLeftHeapNode(LeftHeapNode<T>*& l, LeftHeapNode<T>*& r)
{
	LeftHeapNode<T>* temp = l;
	l = r;
	r = temp;
}

//合并两个左式堆树，输入为待合并左式树，返回新左式树树根
//内部采用的递归思路，每次都将元素较大的树的右子树，与另一树合并，直到合并到某一树节点为空，
//递归返回，此时将不为空的树联接到了上级树的右子树，最后整体联接成了一棵树
template <class T>
LeftHeapNode<T>* PQ_LeftHeap<T>::mergeLeftHeapTree(LeftHeapNode<T>* l, LeftHeapNode<T>* r)
{
	//被某一合并树节点为空，则返回另一合并树根，递归结束
	if (NULL == l)
		return r;
	if (NULL == r)
		return l;

	//确保l的元素值大于r的元素，否则进行交换。因为要将元素值较小树合并到较大树
	if (l->key < r->key)
		swapLeftHeapNode(l, r);

	//递归地将l的右子树和r进行左式堆合并，合并完成后联接到l的右子树上
	l->rchild = mergeLeftHeapTree(l->rchild, r);
	l->rchild->parent = l;

	//确保合并后的左式堆满足左子树upl大于等于右子树upl，否则交换左右子树
	if (!l->lchild || l->lchild->NPL < l->rchild->NPL)
		swapLeftHeapNode(l->lchild, l->rchild);

	//更新根的upl
	l->NPL = l->rchild ? l->rchild->NPL + 1 : 1;
	return l;
}

template <class T>
PQ_LeftHeap<T>::~PQ_LeftHeap()
{
	destroy();
}

template <class T>
bool PQ_LeftHeap<T>::emptyPQ()
{
	return 0 == _size;
}

template <class T>
int PQ_LeftHeap<T>::sizePQ()
{
	return _size;
}

template <class T>
T PQ_LeftHeap<T>::topPQ()
{
	return _root->key;
}

template <class T>
T PQ_LeftHeap<T>::popPQ()
{
	//思路：将最大元素保存，然后合并两棵子左式堆，最后返回最大元素
	T key = _root->key;
	LeftHeapNode<T>* l = _root->lchild;
	LeftHeapNode<T>* r = _root->rchild;
	delete _root;
	_root = mergeLeftHeapTree(l, r);
	if (_root)
		_root->parent = NULL;
	--_size;
	return key;
}

template <class T>
void PQ_LeftHeap<T>::pushPQ(T item)
{
	//思路：新元素构成独立节点左式堆，将其合并到自身左式堆
	LeftHeapNode<T>* node = new LeftHeapNode<T>(item, NULL, NULL, NULL);
	_root = mergeLeftHeapTree(_root, node);
	_root->parent = NULL;
	++_size;
}

template <class T>
PQ_LeftHeap<T>::PQ_LeftHeap()
{
	_item_wide = getItemWide();
	_root = NULL;
	_size = 0;
}

template <class T>
void PQ_LeftHeap<T>::printPQ(ostream& os /*= cout*/)
{
	//树的深度
	int depth = getNodeDepth(_root);
	//用来临时存储每层的节点信息
	vector<vector<LeftHeapNode<T>*> > vItems;
	vItems.resize(depth);
	//当前节点层级
	int level = 0;
	//获取每层节点信息
	PreOrderGetTreeLevelNode(_root, level, depth, vItems);

	//输出每层节点信息
	for (int level = 0; level < depth; level++)
	{
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
		for (int i = 0; i < vItems[level].size(); i++)
		{
			LeftHeapNode<T>* node = vItems[level][i];
			if (node)
			{
				os << left << setw(_item_wide) << node->key << right;
			}
			else
			{
				os << left << setw(_item_wide) << " " << right;
			}
			if (i + 1 != vItems[level].size())
				printSpace(inter_space_item_count, os);
		}
		os << endl;
	}
}

template <class T>
void PQ_LeftHeap<T>::destroy()
{
	freeNode(_root);
	_size = 0;
}

template <class T>
void PQ_LeftHeap<T>::freeNode(LeftHeapNode<T> *& node)
{
	if (NULL == node)
		return;

	freeNode(node->lchild);
	freeNode(node->rchild);
	delete node;
	node = NULL;
}

template <class T>
int PQ_LeftHeap<T>::getNodeDepth(LeftHeapNode<T> *& node)
{
	if (NULL == node)
		return 0;

	return max(getNodeDepth(node->lchild), getNodeDepth(node->rchild)) + 1;
}

template <class T>
void PQ_LeftHeap<T>::PreOrderGetTreeLevelNode(LeftHeapNode<T> * node, int level, int depth, vector<vector<LeftHeapNode<T>*> >& vItems)
{
	//超过树的深度，后面都为NULL叶子节点了
	if (level >= depth)
		return;

	//每层分别记录节点信息
	vItems[level].push_back(node);
	if (node)
	{
		PreOrderGetTreeLevelNode(node->lchild, level + 1, depth, vItems);
		PreOrderGetTreeLevelNode(node->rchild, level + 1, depth, vItems);
	}
	else
	{
		PreOrderGetTreeLevelNode(NULL, level + 1, depth, vItems);
		PreOrderGetTreeLevelNode(NULL, level + 1, depth, vItems);
	}
}

template <class T>
void PQ_LeftHeap<T>::printNode(LeftHeapNode<T> * node, ostream& os /*= cout*/)
{
	if (NULL != node)
		os << node->key << " ";
}

template <class T>
void PQ_LeftHeap<T>::printSpace(float fcount, ostream& os /*= cout*/)
{
	//由于fcount经常作为半个元素占用的空间输入，所以采用浮点类型，否则int无法表达0.5个元素占用的空间
	for (int i = 0; i < int(fcount*_item_wide); i++)
		os << " ";
}

template <class T>
int PQ_LeftHeap<T>::getItemWide()
{
	return NODE_CHAR_WIDE + 1;
}

//////////////////////////////////////////////////////////////////////////
void testPQ_ComlHeap();
void reappear_auto();
void reappear_manual();
template<class T>
void printVector(const vector<T>& record, ostream& os=cout);
template<class T>
void saveDump(const vector<T>& record);

