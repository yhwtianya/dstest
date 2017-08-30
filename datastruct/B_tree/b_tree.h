#pragma  once
#include <vector>
#include <iostream>
#include <algorithm>
#include <iostream>
#include <iomanip>
using namespace std;
#define NODE_CHAR_WIDE		2

/*	����B����С����Ϊ3����Ϊ�����Ϊ2���սڵ�Ҳ����Ч���ڲ��ڵ㣬��B����һ����ƽ������
	B�������˻�����������ʵ���Ի�����AVL�������������Ϊ2����Ҫ���ڲ��սڵ����ܶ��жϴ���
	���ʱ�临�Ӷ����ӡ�2��B��ʵ���Եͣ�ʵ��Ч�ʵͣ��ʲ�ʵ�֡�*/
#define MIN_BTREE_M			3

#ifdef _DEBUG
	#define B_TREE_DEBUG
#endif

/*
B���ֽ�ƽ���·��������(ע������򵥵���Ϊһ��m�׵�B����m����)

B�������ʣ�m�׵�B����

1. ����ÿ�������ຬ��m�����ӣ�m>=2����
2. ��������Ҷ�ӽ���⣬����ÿ�����������[ceil(m / 2)]�����ӣ�����ceil(x)��һ��ȡ���޵ĺ�������
3. �����������2�����ӣ�����B��ֻ����һ����㣺����㣩��
4. ����Ҷ�ӽ�㶼������ͬһ�㣬Ҷ�ӽ�㲻�����κιؼ�����Ϣ(���Կ������ⲿ�����ѯʧ�ܵĽ�㣬
	ָ����Щ����ָ�붼Ϊnull)����ע��Ҷ�ӽڵ�ֻ��û�к��Ӻ�ָ���ӵ�ָ�룬��Щ�ڵ�Ҳ���ڣ�Ҳ��Ԫ�ء����ƺ�����У�ÿһ��NULLָ�뼴����Ҷ�ӽ�㣬ֻ��û���������ѣ���
5. ÿ�����ն˽���а�����n���ؼ�����Ϣ�� (n��P0��K1��P1��K2��P2��......��Kn��Pn)�����У�
	a) Ki (i=1...n)Ϊ�ؼ��֣��ҹؼ��ְ�˳����������K(i-1)< Ki��
	b) PiΪָ���������Ľ�㣬��ָ��P(i-1)ָ�����������н��Ĺؼ��־�С��Ki����������K(i-1)��
	c) �ؼ��ֵĸ���n�������㣺 [ceil(m / 2)-1]<= n <= m-1��������j�����ӵķ�Ҷ���ǡ����j-1���ؼ��롣

2-3������3��B��
2-3-4������4��B��
*/

template <class T>
class BTNode
{
public:
	BTNode();
	BTNode(T key);
	//����key���ڵ�������ʧ�ܷ���-1
	int search(T key);
	//ͨ��key�������������������key���ڴ˽ڵ㣬���ش˽ڵ�������ڵ���������
	int getChildIndex(T key);
	//�Ƿ���Ҷ�ӽڵ�
	bool isLeaf();
	//ɾ��ָ��key
	void removeKey(T key);
	//ɾ��ָ��λ�õ�key
	void removeKeyByPos(int pos_child);

public:
	BTNode<T>* parent;
	vector<T> keys;
	vector<BTNode<T> *> childs;
};

//ɾ��ָ��λ�õ�key
template <class T>
void BTNode<T>::removeKeyByPos(int pos_child)
{
	keys.erase(keys.begin() + pos_child);
	childs.erase(childs.begin() + pos_child + 1);
}

//ɾ��ָ��key
template <class T>
void BTNode<T>::removeKey(T key)
{
	int pos_child = getChildIndex(key) - 1;
	removeKeyByPos(pos_child);
}

template <class T>
BTNode<T>::BTNode(T key)
	:parent(NULL)
{
	keys.push_back(key);
	childs.push_back(NULL);
	childs.push_back(NULL);
}

template <class T>
bool BTNode<T>::isLeaf()
{
	return NULL==childs[0];
}

template <class T>
BTNode<T>::BTNode()
	:parent(NULL)
{
}

//����ҵ�������keys���������򷵻�-1
template <class T>
int BTNode<T>::search(T key)
{
	for (int i = 0; i < keys.size(); i++)
	{
		if (keys[i] == key)
			return i;
	}
	return -1;
}

//ͨ��key�������������������key���ڴ˽ڵ㣬���ش˽ڵ�������ڵ���������
template <class T>
int BTNode<T>::getChildIndex(T key)
{
	if (keys.empty())
		return 0;

	int i = 0;
	for (; i < keys.size(); i++)
	{
		if (keys[i] == key)
			return i + 1;
		else if (keys[i] > key)
			return i;
	}

	return keys.size();
}

template <class T>
class BTree
{
public:
	BTree(int order);
	virtual ~BTree();
	//����key���ڽڵ�
	BTNode<T>* search(T key);
	//����key
	bool insert(T key);
	//ɾ��key
	bool remove(T key);
	//����״�������ָ����
	void print(ostream& os = cout);
	//�����������ָ����
	void printInOrder(ostream& os = cout);
	//����������ָ�������ף���������ף�������СΪ3
	void clear(int order=0);


protected:
	//�ɵ����Ͻ������ʱ���ܲ���������
	void solveUpFlow(BTNode<T> * node);
	//�ɵ����Ͻ����ɾ��ʱ���ܲ���������
	void solveDownFlow(BTNode<T> * node, const T key);
	//��������ؼ�������
	int maxKeyCount(BTNode<T> * node);
	//�������С�ؼ�������
	int minKeyCount(BTNode<T> * node);
	//��ȡ����ʱ�м�key����
	int	getMidIndex(BTNode<T> * node);
	//���ֵ�key����
	int getLBrotherKeysSize(BTNode<T> * node, int pos_child);
	//���ֵ�key����
	int getRBrotherKeysSize(BTNode<T> * node, int pos_child);
	//�����ֵ�Ԫ��
	void borrowLeftBrother(BTNode<T> * node, int pos_child);
	//�����ֵ�Ԫ��
	void borrowRightBrother(BTNode<T> * node, int pos_child);
	//�����ڵ�ϲ������ֵ�
	void mergeLeftBrother(BTNode<T> * node, int pos_child);
	//�����ֵܺϲ������ڵ�
	void mergeRightBrother(BTNode<T> * node, int pos_child);
	//�ͷ������ռ�
	void freeNode(BTNode<T> *& node);
	//��ȡ�������
	int getNodeDepth(BTNode<T> *& node);
	//��������������ÿ��Ľڵ���Ϣ
	void PreOrderGetTreeLevelNode(BTNode<T> * node, int level, int depth, vector<vector<BTNode<T>*> >& vItems);
	//����ڵ���Ϣ
	void printNode(BTNode<T> * node, ostream& os=cout);
	//���ָ�������Ŀո�
	void printSpace(float fcount, ostream& os=cout);
	//��ýڵ�ռ������Ŀ��
	int getItemWide();
	//ɾ��key�����и��¸��ڵ�
	void updataRootWhenRemove(BTNode<T> * node);

protected:
	//��¼���������У����һ������������Ч�ڵ�
	BTNode<T>* _hot;
	//����
	BTNode<T>* _root;
	//�ڵ�����
	int _size;
	//����
	int _order;
	//�ڵ�ռ������Ŀ��
	int _item_wide;
};

template <class T>
void BTree<T>::updataRootWhenRemove(BTNode<T> * node)
{
	delete _root;
	_root = node;
	if (node)
		_root->parent = NULL;
	--_size;
}

template <class T>
int BTree<T>::getItemWide()
{
	/*	(_order - 1)�������key������NODE_CHAR_WIDE����ÿ��keyռ�ÿռ䣬
		_order-2����key֮��ķָ�������
		1����ڵ�������һ���ո�
	*/
	return (_order - 1)*NODE_CHAR_WIDE + (_order - 2) + 1;
}

template <class T>
void BTree<T>::printSpace(float fcount, ostream& os/*=cout*/)
{
	//����fcount������Ϊ���Ԫ��ռ�õĿռ����룬���Բ��ø������ͣ�����int�޷����0.5��Ԫ��ռ�õĿռ�
	for (int i = 0; i < int(fcount*_item_wide); i++)
	{
		os << " ";
	}
}

template <class T>
void BTree<T>::clear(int order)
{
	freeNode(_root);
	_hot = NULL;
	_size = 0;
	if (order>=MIN_BTREE_M)
		_order = order;
	_item_wide = getItemWide();
}

template <class T>
void BTree<T>::printNode(BTNode<T> * node, ostream& os/*=cout*/)
{
	if (NULL!=node)
	{
		for (int i = 0; i < node->keys.size(); i++)
		{
			os << node->keys[i];
			if (i+1 != node->keys.size())
			{
				//Ϊ�˱��ڹ۲죬ÿ��key֮���÷ָ����ֿ�
				os << '|';
			}
		}
		//ÿ��node�������һ���ո�
		os << " ";
	}
}

template <class T>
void BTree<T>::printInOrder(ostream& os /*= cout*/)
{
	int depth = getNodeDepth(_root);
	vector<vector<BTNode<T>*> > vItems;
	vItems.resize(depth);
	int level = 0;
	PreOrderGetTreeLevelNode(_root, level, depth, vItems);
	for (int level = 0; level < depth; level++)
	{
		for (int i = 0; i < vItems[level].size(); i++)
		{
			printNode(vItems[level][i], os);
		}
		os << endl;
	}
	os << endl << std::flush;
}

template <class T>
void BTree<T>::print(ostream& os /*= cout*/)
{
	//�������
	int depth = getNodeDepth(_root);
	//������ʱ�洢ÿ��Ľڵ���Ϣ
	vector<vector<BTNode<T>*> > vItems;
	vItems.resize(depth);
	//��ǰ�ڵ�㼶
	int level = 0;
	//��ȡÿ��ڵ���Ϣ
	PreOrderGetTreeLevelNode(_root, level, depth, vItems);

	int bufsize = _order *(NODE_CHAR_WIDE+1) + 3;
	char *buf = new char[bufsize];
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
			//�ɹ۲�ó������ͨ�ʽ
			inter_space_item_count = pow(_order, (high - 1)) - 1;
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
			char* p = buf;
			BTNode<T>* node = vItems[level][i];
			if (node && !node->keys.empty())
			{
				for (int i = 0; i < node->keys.size(); i++)
				{
					sprintf_s(p, bufsize-(p-buf)-1, "%d|", node->keys[i]);
					p += strlen(p);
				}
				p[strlen(p) - 1] = '\0';
			}
			else
			{
				sprintf_s(p, bufsize - (p - buf) - 1, "_");
			}
			os << left << setw(_item_wide) << buf << right;
			printSpace(inter_space_item_count, os);
		}
		os << endl;
	}

	delete[] buf;
}

template <class T>
void BTree<T>::PreOrderGetTreeLevelNode(BTNode<T> * node, int level, int depth, vector<vector<BTNode<T>*> >& vItems)
{
	//����������ȣ����涼ΪNULLҶ�ӽڵ���
	if (level >= depth)
		return;

	//ÿ��ֱ��¼�ڵ���Ϣ
	vItems[level].push_back(node);
	for (int i = 0; i < _order; i++)
	{
		if(node)
		{
			if (i < node->childs.size() && node->childs[i])
				PreOrderGetTreeLevelNode(node->childs[i], level + 1, depth, vItems);
			else
				PreOrderGetTreeLevelNode(NULL, level + 1, depth, vItems);
		}
		else
			PreOrderGetTreeLevelNode(NULL, level + 1, depth, vItems);
	}
}

template <class T>
int BTree<T>::getNodeDepth(BTNode<T> *& node)
{
	if (NULL == node)
		return 0;

	vector<int> vDepths;
	for (int i = 0; i < node->childs.size(); i++)
	{
		vDepths.push_back(getNodeDepth(node->childs[i]));
	}

	return *(max_element(vDepths.begin(), vDepths.end())) + 1;
}

template <class T>
void BTree<T>::freeNode(BTNode<T> *& node)
{
	if (NULL == node)
		return;

	for (int i = 0; i < node->childs.size(); i++)
		freeNode(node->childs[i]);

	free(node);
	node = NULL;
}

template <class T>
BTree<T>::BTree(int order)
	:_hot(NULL), _root(NULL), _size(0), _order(MIN_BTREE_M)
{
	if (order >= MIN_BTREE_M)
		_order = order;

	_item_wide = getItemWide();
}

template <class T>
BTree<T>::~BTree()
{
	freeNode(_root);
}

template <class T>
void BTree<T>::mergeRightBrother(BTNode<T> * node, int pos_child)
{
	//���ֵ�child������node��child������1�����ڵ㱻�ϲ�Ԫ��key������node��child������ͬ
	//��ȡ���ֵܽڵ�
	BTNode<T>* RB = node->parent->childs[pos_child+1];

	//�����ڵ��Ӧkey�ϲ������ڵ�
	node->keys.push_back(node->parent->keys[pos_child]);
	node->parent->keys.erase(node->parent->keys.begin() + pos_child);
	node->parent->childs.erase(node->parent->childs.begin() + (pos_child+1));

	//�����ֵ�����key�ϲ������ڵ�
	int i = 0;
	for ( ; i < RB->keys.size(); i++)
	{
		node->keys.push_back(RB->keys[i]);
		if (RB->childs[i])
			RB->childs[i]->parent = node;
		node->childs.push_back(RB->childs[i]);
	}

	if (RB->childs[i])
		RB->childs[i]->parent = node;
	node->childs.push_back(RB->childs[i]);

	delete RB;
	--_size;
}

template <class T>
void BTree<T>::mergeLeftBrother(BTNode<T> * node, int pos_child)
{
	//����һ��Ҫ�����ڵ�ϲ������ֵܣ��������ֵܱ��ϲ������ڵ㣬��������ӾͿ���Ϊ�սڵ�
	//�����Ϊ�սڵ�󣬺��������������keys�����жϸ��Ƿ��������ӣ����������

	//���ֵ�child������node��child����С1�����ڵ㱻�ϲ�Ԫ��key������node��child����С1
	pos_child -= 1;
	//��ȡ���ֵܽڵ�
	BTNode<T>* LB = node->parent->childs[pos_child];

	//�����ڵ��Ӧkey�ϲ������ֵ�
	LB->keys.push_back(node->parent->keys[pos_child]);
	LB->parent->keys.erase(node->parent->keys.begin() + pos_child);
	LB->parent->childs.erase(node->parent->childs.begin() + pos_child+1);

	//�����ڵ�����key������˳��ϲ������ֵ�
	int i = 0;
	for (; i<node->keys.size(); ++i)
	{
		LB->keys.push_back(node->keys[i]);
		if (node->childs[i])
			node->childs[i]->parent = LB;
		LB->childs.push_back(node->childs[i]);
	}

	//���水��keys���������б����������childs��һ��û�кϲ�����
	if (node->childs[i])
		node->childs[i]->parent = LB;
	LB->childs.push_back(node->childs[i]);

	delete node;
	--_size;
}

template <class T>
void BTree<T>::borrowRightBrother(BTNode<T> * node, int pos_child)
{
	//��ȡ���ֵܽڵ�
	BTNode<T>* RB = node->parent->childs[pos_child + 1];

	//�����ڵ��Ӧkey�赽���ڵ�
	node->keys.push_back(node->parent->keys[pos_child]);
	if (RB->childs[0])
		RB->childs[0]->parent = node;
	node->childs.push_back(RB->childs[0]);

	//�����ֵܽڵ�ĵ�һ��key�赽���ڵ�
	node->parent->keys[pos_child] = *RB->keys.begin();
	RB->keys.erase(RB->keys.begin());
	RB->childs.erase(RB->childs.begin());
}

template <class T>
void BTree<T>::borrowLeftBrother(BTNode<T> * node, int pos_child)
{
	//���ֵ�child������node��child����С1�����ڵ㱻�ϲ�Ԫ��key������node��child����С1
	pos_child -= 1;
	//��ȡ���ֵܽڵ�
	BTNode<T>* LB = node->parent->childs[pos_child];

	//�����ڵ��Ӧkey�赽���ڵ�
	node->keys.insert(node->keys.begin(), node->parent->keys[pos_child]);
	node->childs.insert(node->childs.begin(), *LB->childs.rbegin());
	if (node->childs[0])
		node->childs[0]->parent = node;

	//�����ֵܽڵ�����һ��key�赽���ڵ�
	node->parent->keys[pos_child] = *LB->keys.rbegin();
	LB->keys.pop_back();
	LB->childs.pop_back();
}

template <class T>
int BTree<T>::getLBrotherKeysSize(BTNode<T> * node, int pos_child)
{
	BTNode<T> * parent = node->parent;
	if(parent)
	{
		//���ֵ�child������node��child����С1
		pos_child -= 1;
		if ((pos_child >= 0) && (parent->childs.size() > pos_child))
			if (parent->childs[pos_child])
				return parent->childs[pos_child]->keys.size();
	}
	
	return 0;
}

template <class T>
int BTree<T>::getRBrotherKeysSize(BTNode<T> * node, int pos_child)
{
	BTNode<T> * parent = node->parent;
	if(parent)
	{
		//���ֵ�child������node��child������1
		pos_child += 1;
		if (pos_child >= 0 && parent->childs.size() > pos_child)
			if (parent->childs[pos_child])
				return parent->childs[pos_child]->keys.size();
	}

	return 0;
}

template <class T>
int BTree<T>::getMidIndex(BTNode<T>* node)
{
	return node->keys.size() / 2;
}

template <class T>
int BTree<T>::maxKeyCount(BTNode<T>* node)
{
	return _order - 1;
}

template <class T>
int BTree<T>::minKeyCount(BTNode<T> * node)
{
	if (_root == node)
		return 1;

	if (node->isLeaf())
		return 1;

	return (_order-_order%2) / 2;
}

//����ɾ������ܵ����硣nodeΪ������������Ľڵ㣬keyΪnode�ﱻɾ��Ԫ��
template <class T>
void BTree<T>::solveDownFlow(BTNode<T> * node, const T key)
{
	//��ǰkey����С����С����key��������������
	if (node->keys.size() < minKeyCount(node))
	{
		//key�����ڸ��ڵ��е���������
		int pos_child = node->parent->getChildIndex(key);
		if (getLBrotherKeysSize(node, pos_child) > minKeyCount(node))
		{
			//���ֵ�Ԫ�������㣬�ɽ����ֵ�Ԫ��
			borrowLeftBrother(node, pos_child);
		}
		else if (getRBrotherKeysSize(node, pos_child) > minKeyCount(node))
		{
			//���ֵ�Ԫ�������㣬�ɽ����ֵ�Ԫ��
			borrowRightBrother(node, pos_child);
		}
		else
		{
			//�����ֵܾ�û���㹻��Ԫ�ؿɽ裬��ɽ����ֵܺϲ�
			//���游�ڵ㱻�ϲ���Ԫ�أ������ڵ㽫ɾ����Ԫ��
			T rm_key;
			if (getLBrotherKeysSize(node, pos_child) != 0)
			{
				//�������ֵܣ������ڵ�ϲ������ֵ�
				BTNode<T>* parent = node->parent;
				rm_key = node->parent->keys[pos_child - 1];
				//�����ڵ�ϲ������ֵܺ󣬱��ڵ㱻ɾ����nodeָ�����ֵ�
				mergeLeftBrother(node, pos_child);
				node = parent->childs[pos_child - 1];
			}
			else
			{
				//������������ֵܣ��򱾽ڵ��������֧����ʱ�ɺ����ֵܺϲ�����ʹ���ֵ�ΪNULL
				rm_key = node->parent->keys[pos_child];
				mergeRightBrother(node, pos_child);
			}
			
			if (_root==node->parent && _root->keys.empty())
			{
				//���ڵ�Ԫ��Ϊ��ʱ��ɾ�����ڵ㣬���ڵ�ָ��ָ���ºϲ��ڵ㣬�¸��ڵ㸸ָ���ÿ�
				updataRootWhenRemove(node);
			}
			else
			{
#ifdef B_TREE_DEBUG
				cout << endl << "solveDownFlow:" << key << endl << "---" << endl;
				print();
				cout << "---" << endl;
#endif
				//�ɵ����ϴ���ɾ�����ܲ���������
				solveDownFlow(node->parent, rm_key);
			}
		}
	}
}

//����������ܵ����硣nodeΪ������������Ľڵ�
template <class T>
void BTree<T>::solveUpFlow(BTNode<T> * node)
{
	//��ǰkey���������������key��������������
	if (node->keys.size() > maxKeyCount(node))
	{
		//node�ڵ�������ѵ�λ�ã����Ѻ�node��Ϊ���������ӣ�RChild��Ϊ�������Һ���
		int midIndex = getMidIndex(node);
		BTNode<T>* RChild = new BTNode<T>();
		for (int i = 0; i < node->keys.size(); i++)
		{
			if (i<midIndex)
				continue;
			else if (i>midIndex)
			{
				//node���Ѻ���Ҳ��ָ�ֵ��������
				RChild->keys.push_back(node->keys[i]);
				RChild->childs.push_back(node->childs[i]);
				//�����½ڵ�ĸ��ڵ���Ϣ
				if (node->childs[i])
					node->childs[i]->parent = RChild;

				if (i+1 == node->keys.size())
				{
					//���������Ҳຢ�ӽڵ�
					RChild->childs.push_back(node->childs[i + 1]);
					if (node->childs[i + 1])
						node->childs[i + 1]->parent = RChild;
				}
			}
			else
			{
				//�����ѽڵ��������뵽���ڵ�
				if (node->parent)
				{
					//���ڸ��ڵ㣬�ڸ��ڵ��ҵ����ѽڵ�Ĳ���λ�ã������ѽڵ��������뵽���ڵ�
					int pos_child = node->parent->getChildIndex(node->keys[midIndex]);
					node->parent->keys.insert(node->parent->keys.begin() + (pos_child), node->keys[midIndex]);
					//�����µ��������ڵ�
					node->parent->childs.insert(node->parent->childs.begin() + (pos_child + 1), RChild);
					RChild->parent = node->parent;
					++_size;
				}
				else
				{
					//���ڵ�ΪNULL���򱾽ڵ��Ǹ�����Ҫ�����µĸ��ڵ�
					BTNode<T>* root = new BTNode<T>();
					root->keys.push_back(node->keys[midIndex]);
					//node��Ϊ���������ӣ�RChild��Ϊ�������Һ���
					root->childs.push_back(node);
					root->childs.push_back(RChild);
					node->parent = root;
					RChild->parent = root;
					_root = root;
					++_size;
				}
			}
		}

		T key = node->keys[midIndex];
		//��node�����ѳ�ȥ��Ԫ��ȫ��ɾ��
		node->keys.erase(node->keys.begin() + midIndex, node->keys.end());
		node->childs.erase(node->childs.begin() + midIndex+1, node->childs.end());
#ifdef B_TREE_DEBUG
		cout << endl << "solveUpFlow:" << key << endl << "---" << endl;
		print();
		cout << "---" << endl;
#endif
		//�ɵ����ϴ��������ܲ���������
		solveUpFlow(node->parent);
	}
}

template <class T>
bool BTree<T>::remove(T key)
{
	BTNode<T>* v = search(key);
	if (!v)
		return false;

	//��ȡkey��keys�е�����
	int pos_child = v->getChildIndex(key) - 1;
	if (!v->isLeaf())
	{
		//�����ɾ���ڵ㲻��Ҷ�ӽڵ㣬���ùؼ������ӽڵ�����ֵ�����滻��ǰ�ڵ�ֵ����ɾ�����ֵ�ڵ�
		BTNode<T>* u = v->childs[pos_child + 1];
		while (u->childs[0])
			u = u->childs[0];
		v->keys[pos_child] = u->keys[0];
		v = u;
		pos_child = 0;
		key = u->keys[0];
	}

	v->removeKeyByPos(pos_child);
	if (_root == v)
	{
		//���ֻ���ڸ��ڵ㣬���ô�������
		if (_root->keys.empty())
			updataRootWhenRemove(NULL);
		return true;
	}

	//�ɵ����ϴ���ɾ�����ܲ���������
	solveDownFlow(v, key);
	return true;
}

template <class T>
bool BTree<T>::insert(T key)
{
	if (!_root)
	{
		_root = new BTNode<T>(key);
		++_size;
		return true;
	}
	
	BTNode<T>* v = search(key);
	if (v)
		return false;

	//��ȡchilds������
	int pos_child = _hot->getChildIndex(key);
	_hot->keys.insert(_hot->keys.begin()+ (pos_child), key);
	_hot->childs.insert(_hot->childs.begin() + (pos_child + 1), NULL);

	//�ɵ����ϴ��������ܲ���������
	solveUpFlow(_hot);
	return true;
}

//�ҵ��򷵻ؽڵ��ַ�����򷵻�NULL��ͬʱ_hot��¼���������У����һ������������Ч�ڵ�
template <class T>
BTNode<T>* BTree<T>::search(T key)
{
	if (NULL == _root)
		return NULL;

	BTNode<T>* v = _root;
	_hot = NULL;
	while (NULL!=v)
	{
		int pos_child = v->getChildIndex(key);
		//����ڵ����ҵ�key��getChildIndex����key�������ڵ�ChildIndex
		if (pos_child >= 1 && pos_child<=v->keys.size() && v->keys[pos_child-1] == key)
			return v;
		
		_hot = v;
		if (pos_child < v->childs.size())
			v = v->childs[pos_child];
		else
			v = NULL;
	}
	
	return NULL;
}

void testBTree();
//�Զ������ϴ�ִ�й���
void reappear_auto();
//�ֶ�����ĳ����
void reappear_manual();
//���vector
template<class T> void printVector(const vector<T>& record, ostream& os = cout);
//����ԭʼ��Ϣ�������Զ�����
template<class T> void saveDump(const vector<T>& record, int order);