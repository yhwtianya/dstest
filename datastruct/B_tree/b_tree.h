#pragma  once
#include <vector>
#include <iostream>
#include <algorithm>
#include <iostream>
#include <iomanip>
using namespace std;
#define NODE_CHAR_WIDE		2

/*	定义B树最小阶数为3，因为如果阶为2，空节点也是有效的内部节点，则B树不一定是平衡树，
	B树经常退化成线性树，实用性还不如AVL。而且如果阶数为2，需要对内部空节点做很多判断处理，
	造成时间复杂度增加。2阶B树实用性低，实现效率低，故不实现。*/
#define MIN_BTREE_M			3

#ifdef _DEBUG
	#define B_TREE_DEBUG
#endif

/*
B树又叫平衡多路查找树。(注：切勿简单的认为一棵m阶的B树是m叉树)

B树的性质（m阶的B树）

1. 树中每个结点最多含有m个孩子（m>=2）；
2. 除根结点和叶子结点外，其它每个结点至少有[ceil(m / 2)]个孩子（其中ceil(x)是一个取上限的函数）；
3. 根结点至少有2个孩子（除非B树只包含一个结点：根结点）；
4. 所有叶子结点都出现在同一层，叶子结点不包含任何关键字信息(可以看做是外部结点或查询失败的结点，
	指向这些结点的指针都为null)；（注：叶子节点只是没有孩子和指向孩子的指针，这些节点也存在，也有元素。类似红黑树中，每一个NULL指针即当做叶子结点，只是没画出来而已）。
5. 每个非终端结点中包含有n个关键字信息： (n，P0，K1，P1，K2，P2，......，Kn，Pn)。其中：
	a) Ki (i=1...n)为关键字，且关键字按顺序升序排序K(i-1)< Ki。
	b) Pi为指向子树根的结点，且指针P(i-1)指向子树种所有结点的关键字均小于Ki，但都大于K(i-1)。
	c) 关键字的个数n必须满足： [ceil(m / 2)-1]<= n <= m-1。比如有j个孩子的非叶结点恰好有j-1个关键码。

2-3树，即3阶B树
2-3-4树，即4阶B树
*/

template <class T>
class BTNode
{
public:
	BTNode();
	BTNode(T key);
	//查找key所在的索引，失败返回-1
	int search(T key);
	//通过key查找子树的索引，如果key就在此节点，返回此节点后面相邻的子树索引
	int getChildIndex(T key);
	//是否是叶子节点
	bool isLeaf();
	//删除指定key
	void removeKey(T key);
	//删除指定位置的key
	void removeKeyByPos(int pos_child);

public:
	BTNode<T>* parent;
	vector<T> keys;
	vector<BTNode<T> *> childs;
};

//删除指定位置的key
template <class T>
void BTNode<T>::removeKeyByPos(int pos_child)
{
	keys.erase(keys.begin() + pos_child);
	childs.erase(childs.begin() + pos_child + 1);
}

//删除指定key
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

//如果找到，返回keys索引，否则返回-1
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

//通过key查找子树的索引，如果key就在此节点，返回此节点后面相邻的子树索引
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
	//查找key所在节点
	BTNode<T>* search(T key);
	//插入key
	bool insert(T key);
	//删除key
	bool remove(T key);
	//按树状输出树到指定流
	void print(ostream& os = cout);
	//中序输出树到指定流
	void printInOrder(ostream& os = cout);
	//清除树，如果指定新树阶，则更新树阶，树阶最小为3
	void clear(int order=0);


protected:
	//由底向上解决插入时可能产生的上溢
	void solveUpFlow(BTNode<T> * node);
	//由底向上解决插删除时可能产生的下溢
	void solveDownFlow(BTNode<T> * node, const T key);
	//允许的最大关键字数量
	int maxKeyCount(BTNode<T> * node);
	//允许的最小关键字数量
	int minKeyCount(BTNode<T> * node);
	//获取分裂时中间key索引
	int	getMidIndex(BTNode<T> * node);
	//左兄弟key数量
	int getLBrotherKeysSize(BTNode<T> * node, int pos_child);
	//右兄弟key数量
	int getRBrotherKeysSize(BTNode<T> * node, int pos_child);
	//借左兄弟元素
	void borrowLeftBrother(BTNode<T> * node, int pos_child);
	//借右兄弟元素
	void borrowRightBrother(BTNode<T> * node, int pos_child);
	//将本节点合并到左兄弟
	void mergeLeftBrother(BTNode<T> * node, int pos_child);
	//将右兄弟合并到本节点
	void mergeRightBrother(BTNode<T> * node, int pos_child);
	//释放子树空间
	void freeNode(BTNode<T> *& node);
	//获取子树深度
	int getNodeDepth(BTNode<T> *& node);
	//先序遍历树，获得每层的节点信息
	void PreOrderGetTreeLevelNode(BTNode<T> * node, int level, int depth, vector<vector<BTNode<T>*> >& vItems);
	//输出节点信息
	void printNode(BTNode<T> * node, ostream& os=cout);
	//输出指定数量的空格
	void printSpace(float fcount, ostream& os=cout);
	//获得节点占用输出的宽度
	int getItemWide();
	//删除key过程中更新根节点
	void updataRootWhenRemove(BTNode<T> * node);

protected:
	//记录搜索过程中，最后一个被搜索的有效节点
	BTNode<T>* _hot;
	//树根
	BTNode<T>* _root;
	//节点数量
	int _size;
	//树阶
	int _order;
	//节点占用输出的宽度
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
	/*	(_order - 1)代表最大key数量，NODE_CHAR_WIDE代表每个key占用空间，
		_order-2代表key之间的分隔符数量
		1代表节点最后输出一个空格
	*/
	return (_order - 1)*NODE_CHAR_WIDE + (_order - 2) + 1;
}

template <class T>
void BTree<T>::printSpace(float fcount, ostream& os/*=cout*/)
{
	//由于fcount经常作为半个元素占用的空间输入，所以采用浮点类型，否则int无法表达0.5个元素占用的空间
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
				//为了便于观察，每个key之间用分隔符分开
				os << '|';
			}
		}
		//每个node后面输出一个空格
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
	//树的深度
	int depth = getNodeDepth(_root);
	//用来临时存储每层的节点信息
	vector<vector<BTNode<T>*> > vItems;
	vItems.resize(depth);
	//当前节点层级
	int level = 0;
	//获取每层节点信息
	PreOrderGetTreeLevelNode(_root, level, depth, vItems);

	int bufsize = _order *(NODE_CHAR_WIDE+1) + 3;
	char *buf = new char[bufsize];
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
			//由观察得出的这个通项公式
			inter_space_item_count = pow(_order, (high - 1)) - 1;
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
	//超过树的深度，后面都为NULL叶子节点了
	if (level >= depth)
		return;

	//每层分别记录节点信息
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
	//右兄弟child索引比node的child索引大1，父节点被合并元素key索引和node的child索引相同
	//获取右兄弟节点
	BTNode<T>* RB = node->parent->childs[pos_child+1];

	//将父节点对应key合并到本节点
	node->keys.push_back(node->parent->keys[pos_child]);
	node->parent->keys.erase(node->parent->keys.begin() + pos_child);
	node->parent->childs.erase(node->parent->childs.begin() + (pos_child+1));

	//将右兄弟所有key合并到本节点
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
	//这里一定要将本节点合并到左兄弟，否则，左兄弟被合并到本节点，父的左儿子就可能为空节点
	//左儿子为空节点后，后面是依据左儿子keys数量判断父是否存在左儿子，会造成误判

	//左兄弟child索引比node的child索引小1，父节点被合并元素key索引比node的child索引小1
	pos_child -= 1;
	//获取左兄弟节点
	BTNode<T>* LB = node->parent->childs[pos_child];

	//将父节点对应key合并到左兄弟
	LB->keys.push_back(node->parent->keys[pos_child]);
	LB->parent->keys.erase(node->parent->keys.begin() + pos_child);
	LB->parent->childs.erase(node->parent->childs.begin() + pos_child+1);

	//将本节点所有key，保持顺序合并到左兄弟
	int i = 0;
	for (; i<node->keys.size(); ++i)
	{
		LB->keys.push_back(node->keys[i]);
		if (node->childs[i])
			node->childs[i]->parent = LB;
		LB->childs.push_back(node->childs[i]);
	}

	//上面按照keys的数量进行遍历，会造成childs有一个没有合并过来
	if (node->childs[i])
		node->childs[i]->parent = LB;
	LB->childs.push_back(node->childs[i]);

	delete node;
	--_size;
}

template <class T>
void BTree<T>::borrowRightBrother(BTNode<T> * node, int pos_child)
{
	//获取右兄弟节点
	BTNode<T>* RB = node->parent->childs[pos_child + 1];

	//将父节点对应key借到本节点
	node->keys.push_back(node->parent->keys[pos_child]);
	if (RB->childs[0])
		RB->childs[0]->parent = node;
	node->childs.push_back(RB->childs[0]);

	//将右兄弟节点的第一个key借到父节点
	node->parent->keys[pos_child] = *RB->keys.begin();
	RB->keys.erase(RB->keys.begin());
	RB->childs.erase(RB->childs.begin());
}

template <class T>
void BTree<T>::borrowLeftBrother(BTNode<T> * node, int pos_child)
{
	//左兄弟child索引比node的child索引小1，父节点被合并元素key索引比node的child索引小1
	pos_child -= 1;
	//获取左兄弟节点
	BTNode<T>* LB = node->parent->childs[pos_child];

	//将父节点对应key借到本节点
	node->keys.insert(node->keys.begin(), node->parent->keys[pos_child]);
	node->childs.insert(node->childs.begin(), *LB->childs.rbegin());
	if (node->childs[0])
		node->childs[0]->parent = node;

	//将左兄弟节点的最后一个key借到父节点
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
		//左兄弟child索引比node的child索引小1
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
		//右兄弟child索引比node的child索引大1
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

//处理删除后可能的下溢。node为可能引发下溢的节点，key为node里被删除元素
template <class T>
void BTree<T>::solveDownFlow(BTNode<T> * node, const T key)
{
	//当前key数量小于最小允许key数量，处理下溢
	if (node->keys.size() < minKeyCount(node))
	{
		//key子树在父节点中的子树索引
		int pos_child = node->parent->getChildIndex(key);
		if (getLBrotherKeysSize(node, pos_child) > minKeyCount(node))
		{
			//左兄弟元素数充足，可借左兄弟元素
			borrowLeftBrother(node, pos_child);
		}
		else if (getRBrotherKeysSize(node, pos_child) > minKeyCount(node))
		{
			//右兄弟元素数充足，可借右兄弟元素
			borrowRightBrother(node, pos_child);
		}
		else
		{
			//左右兄弟均没有足够的元素可借，则可进行兄弟合并
			//保存父节点被合并的元素，即父节点将删除的元素
			T rm_key;
			if (getLBrotherKeysSize(node, pos_child) != 0)
			{
				//存在左兄弟，将本节点合并到左兄弟
				BTNode<T>* parent = node->parent;
				rm_key = node->parent->keys[pos_child - 1];
				//将本节点合并到左兄弟后，本节点被删除，node指向左兄弟
				mergeLeftBrother(node, pos_child);
				node = parent->childs[pos_child - 1];
			}
			else
			{
				//如果不存在左兄弟，则本节点是最左分支，此时可和右兄弟合并，即使右兄弟为NULL
				rm_key = node->parent->keys[pos_child];
				mergeRightBrother(node, pos_child);
			}
			
			if (_root==node->parent && _root->keys.empty())
			{
				//根节点元素为空时，删除根节点，根节点指针指向新合并节点，新根节点父指针置空
				updataRootWhenRemove(node);
			}
			else
			{
#ifdef B_TREE_DEBUG
				cout << endl << "solveDownFlow:" << key << endl << "---" << endl;
				print();
				cout << "---" << endl;
#endif
				//由底向上处理删除可能产生的下溢
				solveDownFlow(node->parent, rm_key);
			}
		}
	}
}

//处理插入后可能的上溢。node为可能引发上溢的节点
template <class T>
void BTree<T>::solveUpFlow(BTNode<T> * node)
{
	//当前key数量大于最大允许key数量，处理上溢
	if (node->keys.size() > maxKeyCount(node))
	{
		//node节点产生分裂的位置，分裂后，node作为新子树左孩子，RChild作为新子树右孩子
		int midIndex = getMidIndex(node);
		BTNode<T>* RChild = new BTNode<T>();
		for (int i = 0; i < node->keys.size(); i++)
		{
			if (i<midIndex)
				continue;
			else if (i>midIndex)
			{
				//node分裂后的右部分赋值到右子树
				RChild->keys.push_back(node->keys[i]);
				RChild->childs.push_back(node->childs[i]);
				//更新新节点的父节点信息
				if (node->childs[i])
					node->childs[i]->parent = RChild;

				if (i+1 == node->keys.size())
				{
					//最后插入最右侧孩子节点
					RChild->childs.push_back(node->childs[i + 1]);
					if (node->childs[i + 1])
						node->childs[i + 1]->parent = RChild;
				}
			}
			else
			{
				//将分裂节点上升插入到父节点
				if (node->parent)
				{
					//存在父节点，在父节点找到分裂节点的插入位置，将分裂节点上升插入到父节点
					int pos_child = node->parent->getChildIndex(node->keys[midIndex]);
					node->parent->keys.insert(node->parent->keys.begin() + (pos_child), node->keys[midIndex]);
					//插入新的右子树节点
					node->parent->childs.insert(node->parent->childs.begin() + (pos_child + 1), RChild);
					RChild->parent = node->parent;
					++_size;
				}
				else
				{
					//父节点为NULL，则本节点是根，需要产生新的父节点
					BTNode<T>* root = new BTNode<T>();
					root->keys.push_back(node->keys[midIndex]);
					//node作为新子树左孩子，RChild作为新子树右孩子
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
		//将node被分裂出去的元素全部删除
		node->keys.erase(node->keys.begin() + midIndex, node->keys.end());
		node->childs.erase(node->childs.begin() + midIndex+1, node->childs.end());
#ifdef B_TREE_DEBUG
		cout << endl << "solveUpFlow:" << key << endl << "---" << endl;
		print();
		cout << "---" << endl;
#endif
		//由底向上处理插入可能产生的上溢
		solveUpFlow(node->parent);
	}
}

template <class T>
bool BTree<T>::remove(T key)
{
	BTNode<T>* v = search(key);
	if (!v)
		return false;

	//获取key在keys中的索引
	int pos_child = v->getChildIndex(key) - 1;
	if (!v->isLeaf())
	{
		//如果待删除节点不是叶子节点，则用关键码右子节点的最大值进行替换当前节点值，并删除最大值节点
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
		//如果只存在根节点，则不用处理下溢
		if (_root->keys.empty())
			updataRootWhenRemove(NULL);
		return true;
	}

	//由底向上处理删除可能产生的下溢
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

	//获取childs的索引
	int pos_child = _hot->getChildIndex(key);
	_hot->keys.insert(_hot->keys.begin()+ (pos_child), key);
	_hot->childs.insert(_hot->childs.begin() + (pos_child + 1), NULL);

	//由底向上处理插入可能产生的上溢
	solveUpFlow(_hot);
	return true;
}

//找到则返回节点地址，否则返回NULL，同时_hot记录搜索过程中，最后一个被搜索的有效节点
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
		//如果节点中找到key，getChildIndex返回key后面相邻的ChildIndex
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
//自动重现上次执行过程
void reappear_auto();
//手动重现某过程
void reappear_manual();
//输出vector
template<class T> void printVector(const vector<T>& record, ostream& os = cout);
//保存原始信息，便于自动重现
template<class T> void saveDump(const vector<T>& record, int order);