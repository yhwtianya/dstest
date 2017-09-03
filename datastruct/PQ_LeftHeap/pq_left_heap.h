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

/*��ʽ�ѣ�Leftist Heaps)�ֳ�������ѡ�����ѡ���ƫ������ƫ�ѡ���ʽ����Ϊ�ѵ�һ�֣������˶ѵ�һЩ���ԡ�
��1����ʽ����Ȼ�Զ���������ʽ��������2����������ԣ���ʽ�ѵ��������ֵ��������������ֵ���󣨻�С����
����һ��Ķ���Ѳ�ͬ����ʽ�Ѳ�����һ����ȫ��������Complete tree)��������һ�ü���ƽ�����

���ȶ������漰��"���������ȶ��н��кϲ�"������ʱ������ѵ�Ч�ʾͲ�̫�������⣬
��������ʽ��ʵ�ֵ����ȶ��п��Ը�Ч��ʵ���������ȶ��еĺϲ��������Ԫ�����ڶѵ�logn�Σ���
���һ�����ʽ�ѵ����ȶ��еĲ���ɾ��Ԫ�ز��������Ի��ںϲ�������ɡ�

��ʽ����һ�ö����������Ľڵ���˺Ͷ������Ľڵ�һ��������������ָ���⣬����һ����������NPL���սڵ�·�����ȡ�
�սڵ�·�����ȣ���һ���ڵ㵽һ��"����Ŀսڵ㣨����Ŀսڵ㣩"��·�����ȣ��м�������ġ�
�սڵ�NPLΪ0��Ҷ�ӽڵ��NPLΪ1��
NPL(x)Ҳ����ڵ�x���ⲿ�ڵ����̾��룬Ҳ������xΪ��������������ĸ߶ȡ�
��ʽ�ѵĶ�����ǻ���NPL�����
���ʣ�
	���㣺�����κ��ڽڵ�x������ NPL(lc(x)) >= NPL(rc(x))
���ۣ������κ��ڽڵ�x������ NPL(x) = 1 + NPL(rc(x))

������ʽ�Ѳ�����ṹ������ȫ��������ʽ������ƽ�⣩���洢�ṹ�ϲ��������ģ����ѡ���������Ϊ�洢�ṹ����Ч��

������ʽ�ѵ����ȶ���ʱ�临�Ӷȣ�����O(logn)��ɾ��O(logn)������O(1)
��������Ѻϲ������Ԫ��ֵ�ϴ�ĶѵĽڵ���Ϊn���ϲ���ʱ�临�Ӷ�O(logn)
*/

typedef int Rank;
template <class T>
class PQ_LeftHeap : public PQ<T>
{
public:
	PQ_LeftHeap();
	~PQ_LeftHeap();
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
	//�����
	void destroy();
	//������r�ϲ��������У���ɺ�rȫ��Ԫ�ؼ��е�������
	void mergeLeftHeap(PQ_LeftHeap<T>& r);

protected:
	//�ͷ������ռ�
	void freeNode(LeftHeapNode<T> *& node);
	//��ȡ�������
	int getNodeDepth(LeftHeapNode<T> *& node);
	//��������������ÿ��Ľڵ���Ϣ
	void PreOrderGetTreeLevelNode(LeftHeapNode<T> * node, int level, int depth, vector<vector<LeftHeapNode<T>*> >& vItems);
	//����ڵ���Ϣ
	void printNode(LeftHeapNode<T> * node, ostream& os = cout);
	//���ָ�������Ŀո�
	void printSpace(float fcount, ostream& os = cout);
	//��ýڵ�ռ������Ŀ��
	int getItemWide();
	//�ϲ�������ʽ��
	static LeftHeapNode<T>* mergeLeftHeapTree(LeftHeapNode<T>* l, LeftHeapNode<T>* r);
	//������ʽ�ѽڵ�
	static void swapLeftHeapNode(LeftHeapNode<T>*& l, LeftHeapNode<T>*& r);

protected:
	//���д�С
	unsigned int _size;
	//�ڵ�ռ������Ŀ��
	int _item_wide;
	//��ʽ�Ѹ�
	LeftHeapNode<T>* _root;
};

//������r�ϲ��������У���ɺ�rȫ��Ԫ�ؼ��е�������
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

//�ϲ�������ʽ����������Ϊ���ϲ���ʽ������������ʽ������
//�ڲ����õĵݹ�˼·��ÿ�ζ���Ԫ�ؽϴ������������������һ���ϲ���ֱ���ϲ���ĳһ���ڵ�Ϊ�գ�
//�ݹ鷵�أ���ʱ����Ϊ�յ������ӵ����ϼ�����������������������ӳ���һ����
template <class T>
LeftHeapNode<T>* PQ_LeftHeap<T>::mergeLeftHeapTree(LeftHeapNode<T>* l, LeftHeapNode<T>* r)
{
	//��ĳһ�ϲ����ڵ�Ϊ�գ��򷵻���һ�ϲ��������ݹ����
	if (NULL == l)
		return r;
	if (NULL == r)
		return l;

	//ȷ��l��Ԫ��ֵ����r��Ԫ�أ�������н�������ΪҪ��Ԫ��ֵ��С���ϲ����ϴ���
	if (l->key < r->key)
		swapLeftHeapNode(l, r);

	//�ݹ�ؽ�l����������r������ʽ�Ѻϲ����ϲ���ɺ����ӵ�l����������
	l->rchild = mergeLeftHeapTree(l->rchild, r);
	l->rchild->parent = l;

	//ȷ���ϲ������ʽ������������upl���ڵ���������upl�����򽻻���������
	if (!l->lchild || l->lchild->NPL < l->rchild->NPL)
		swapLeftHeapNode(l->lchild, l->rchild);

	//���¸���upl
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
	//˼·�������Ԫ�ر��棬Ȼ��ϲ���������ʽ�ѣ���󷵻����Ԫ��
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
	//˼·����Ԫ�ع��ɶ����ڵ���ʽ�ѣ�����ϲ���������ʽ��
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
	//�������
	int depth = getNodeDepth(_root);
	//������ʱ�洢ÿ��Ľڵ���Ϣ
	vector<vector<LeftHeapNode<T>*> > vItems;
	vItems.resize(depth);
	//��ǰ�ڵ�㼶
	int level = 0;
	//��ȡÿ��ڵ���Ϣ
	PreOrderGetTreeLevelNode(_root, level, depth, vItems);

	//���ÿ��ڵ���Ϣ
	for (int level = 0; level < depth; level++)
	{
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
	//����������ȣ����涼ΪNULLҶ�ӽڵ���
	if (level >= depth)
		return;

	//ÿ��ֱ��¼�ڵ���Ϣ
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
	//����fcount������Ϊ���Ԫ��ռ�õĿռ����룬���Բ��ø������ͣ�����int�޷����0.5��Ԫ��ռ�õĿռ�
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

