#pragma  once
#include <vector>
#include <iostream>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "win_color_cnt.h"
using namespace std;
#define NODE_CHAR_WIDE		2

#ifdef _DEBUG
//#define RB_TREE_DEBUG
#endif

/*
	红黑树
	由红黑两类节点组成的BST（亦可给边染色）
	统一增设外部节点NULL，使之成为真二叉树

	红黑树性质
	1. 树根：必须为黑色
	2. 外部节点：均为黑色。外部节点指假想的页节点
	3. 其余节点：或为红，则只能有黑孩子（红之子、之父必黑）
	4. 外部节点到根：途中黑节点(不包含外部节点)数目相同（黑深度）
*/

//节点颜色
enum RBTNColor
{
	RBTColorRed = 1,
	RBTColorBlack
};

template <class T>
class RBTNode
{
public:
	T key;
	RBTNColor color;
	RBTNode<T>* parent;
	RBTNode<T>* lchild;
	RBTNode<T>* rchild;
	RBTNode(T value, RBTNColor c, RBTNode *p, RBTNode *l, RBTNode *r) :
		key(value), color(c), parent(p), lchild(l), rchild(r) 
	{
		if (p)
		{
			if (key < p->key)
				p->lchild = this;
			else
				p->rchild = this;
		}
	}

	bool isLeaf() { return lchild == NULL && rchild == NULL; }
};


template <class T>
class RedBlackTree
{
public:
	RedBlackTree();
	~RedBlackTree();
	//查找key所在节点
	RBTNode<T>* search(const T& key);
	//插入key
	RBTNode<T>* insert(const T& key);
	//删除key
	bool remove(const T& key);
	//按树状输出树到指定流
	void print(ostream& os = cout);
	//清除树
	void destroy();

protected:
	//释放子树空间
	void freeNode(RBTNode<T> *& node);
	//获取子树深度
	int getNodeDepth(RBTNode<T> *& node);
	//先序遍历树，获得每层的节点信息
	void PreOrderGetTreeLevelNode(RBTNode<T> * node, int level, int depth, vector<vector<RBTNode<T>*> >& vItems);
	//输出节点信息
	void printNode(RBTNode<T> * node, ostream& os = cout);
	//输出指定数量的空格
	void printSpace(float fcount, ostream& os = cout);
	//获得节点占用输出的宽度
	int getItemWide();
	//解决双红
	void solveDoubleRed(RBTNode<T>* v);
	//3+4局部树宏观重构，返回新的局部树根
	RBTNode<T>* connect34(RBTNode<T>* a, RBTNode<T>* b, RBTNode<T>* c, 
		RBTNode<T>* t1, RBTNode<T>* t2, RBTNode<T>* t3, RBTNode<T>* t4);
	//zigzig、zigzag、zagzag、zagzig旋转，返回新的局部树根
	RBTNode<T>* rotateAt(RBTNode<T>* v);
	//解决双黑
	void solveDoubleBlack(RBTNode<T>* v);
	//hot既为输入又为输出，返回删除v后，v的替代节点。同时更新hot为实际删除节点,内部不进行内存释放
	RBTNode<T>* removeAt(RBTNode<T>* v, RBTNode<T>*& hot);

protected:
	//记录搜索过程中，最后一个被搜索的有效节点
	RBTNode<T>* _hot;
	//树根
	RBTNode<T>* _root;
	//节点占用输出的宽度
	int _item_wide;
};

template <class T>
RedBlackTree<T>::RedBlackTree()
	:_hot(NULL), _root(NULL)
{
	_item_wide = getItemWide();
}

template <class T>
RedBlackTree<T>::~RedBlackTree()
{
	destroy();
}

template <class T>
RBTNode<T>* RedBlackTree<T>::search(const T& key)
{
	RBTNode<T>* v = _root;
	_hot = NULL;
	while (v!=NULL)
	{
		if (v->key<key)
		{
			_hot = v;
			v = v->rchild;
		}
		else if (key < v->key)
		{
			_hot = v;
			v = v->lchild;
		}
		else
			return v;
	}

	return NULL;
}

template <class T>
RBTNode<T>* RedBlackTree<T>::insert(const T& key)
{
	if (NULL==_root)
	{
		_root = new RBTNode<T>(key, RBTColorBlack, NULL, NULL, NULL);
		return _root;
	}

	RBTNode<T>* v = search(key);
	if (v)
		return v;

	//创建新的红节点
	v = new RBTNode<T>(key, RBTColorRed, _hot, NULL, NULL);
	//解决可能的双红冲突
	solveDoubleRed(v);
	//总是返回key为x的节点
	return v ? v : _hot->parent;
}

template <class T>
void RedBlackTree<T>::solveDoubleRed(RBTNode<T>* v)
{
	if (_root == v)
	{
		//v为根，根节点直接染黑
		_root->color = RBTColorBlack;
		return;
	}

	//v不为根，则存在父，如果父为黑，不存在冲突，直接返回
	RBTNode<T>* p = v->parent;
	if (RBTColorBlack == p->color)	//p可能为root
		return;	
	
	//***到这里说明父不为root，必存在祖父，即以下v、p、g均有效，v、p为红，g为黑***
	RBTNode<T>* g = p->parent;
	//叔父节点
	RBTNode<T>* u = (p == g->lchild) ? g->rchild : g->lchild;

	if (!u || RBTColorBlack == u->color)		//RR-1 叔父为黑
	{
		/*	叔父节点为黑色，v、p为红，u、g为黑
		此时原树外部节点到祖父节点的黑高度为2，想象一下叔父的外部节点到祖父即可。
		这种情况想象v、p、g组成一个4阶B树的超级节点，超级节点孩子（外部节点）均为黑色（u为g的孩子），
		只要进行3+4重构（中序等价转换），使超级节点为RBR（红黑红）形式，此时想象还原回二阶红黑树，
		新的祖父（v或p）仍为黑，祖父的两个孩子为红，外部节点是两个孩子的孩子，均为黑，
		此时所有外部节点到祖父的黑高度仍均为2，此时已经构成红黑树的平衡
		这种情况对树局部进行了1--2次重构和2次染色，无需向上层继续结构调整和染色
		（先染色后旋转需要2次染色，但染色时有很多判断，先旋转后染色需要3次染色，操作直观）
		*/

		//rotateAt内部实现了新祖父和曾祖父的重联接
		RBTNode<T>* root = rotateAt(v);
		//这里对调整后的局部树直接宏观上染色，非常直观
		root->color = RBTColorBlack;
		root->lchild->color = RBTColorRed;
		root->rchild->color = RBTColorRed;

		//旋转的意义是提升父节点p为局部根并染黑，v、g为p的子并染红，这样使树高降低，并保持红黑树性质
	}
	else										//RR-2 叔父为红
	{
		/*	叔父节点为红色，则叔父有黑色外部节点，v、p、u均为红，g为黑
		此时原树外部节点到祖父节点的黑高度为1，想象一下叔父到祖父即可。
		这种情况不需要进行任何结构调整，只要将p、u染黑，g染红
		此时所有外部节点到祖父的黑高度仍均为1，但g节点为红，对上可能引起新的双红冲突。
		此时可以由g向上递归解决双红冲突，直到解决到树根。
		这种情况对树局部进行了0次重构和3次染色，可能需向上继续递归进行双红冲突解决
		向上递归，是按双层向上递归的，递归过程如过遇到冲突，既可能是RR-1也可能是RR-2，
		但是一个好的情况是，一旦遇到RR-1，进行完一次旋转操作后，递归就结束了。
		所以红黑树解决双红问题只需进行1--2次重构操作，进行logn次的染色操作即可完成。
		*/
		p->color = RBTColorBlack;
		u->color = RBTColorBlack;
		if (_root != g)
		{
			g->color = RBTColorRed;
			solveDoubleRed(g);
		}
	}
}

//3+4局部树宏观重构，不是通过旋转达到平衡，而是将本节点、父节点、祖父节点及局部树所有叶子节点全部拆开，
//按照原来的大小关系直接重构成一个局部平衡树。这样就没有旋转繁琐的操作了，对所有节点宏观上重组，简单直观。
template <class T>
RBTNode<T>* RedBlackTree<T>::connect34(RBTNode<T>* a, RBTNode<T>* b, RBTNode<T>* c, 
	RBTNode<T>* t1, RBTNode<T>* t2, RBTNode<T>* t3, RBTNode<T>* t4)
{
	b->lchild = a;
	a->parent = b;
	b->rchild = c;
	c->parent = b;

	a->lchild = t1;
	if (t1)
		t1->parent = a;
	a->rchild = t2;
	if (t2)
		t2->parent = a;

	c->lchild = t3;
	if (t3)
		t3->parent = c;
	c->rchild = t4;
	if (t4)
		t4->parent = c;

	//这里实现了曾祖父向下联接
	if (b->parent)
	{
		if (b->key < b->parent->key)
			b->parent->lchild = b;
		else
			b->parent->rchild = b;
	}
	else
	{
		//没有父，则用根直接联接
		_root = b;
	}

	return b;
}

template <class T>
RBTNode<T>* RedBlackTree<T>::rotateAt(RBTNode<T>* v)
{
	RBTNode<T>* p = v->parent;
	RBTNode<T>* g = p->parent;
	if (p == g->lchild)		//zig
	{
		if (v == p->lchild)		//zigzig
		{
			//向上联接曾祖父，connect34内部实现了向下联接
			p->parent = g->parent;
			return connect34(v, p, g,
				v->lchild, v->rchild, p->rchild, g->rchild);
		} 
		else					//zigzag
		{
			//向上联接曾祖父，connect34内部实现了向下联接
			v->parent = g->parent;
			return connect34(p, v, g,
				p->lchild, v->lchild, v->rchild, g->rchild);
		}
	}
	else					//zag
	{
		if (v == p->lchild)		//zagzig
		{
			//向上联接曾祖父，connect34内部实现了向下联接
			v->parent = g->parent;
			return connect34(g, v, p, 
				g->lchild, v->lchild, v->rchild, p->rchild);
		}
		else					//zagzag
		{
			//向上联接曾祖父，connect34内部实现了向下联接
			p->parent = g->parent;
			return connect34(g, p, v,
				g->lchild, p->lchild, v->lchild, v->rchild);
		}
	}
}

template <class T>
bool RedBlackTree<T>::remove(const T& key)
{
	RBTNode<T>* v = search(key);
	if (NULL == v)
		return false;

	RBTNode<T>* del = _hot;
	//removeAt返回删除v后，v的替代节点。同时更新del为实际删除节点,
	//由于还需要使用实际被删除节点，removeAt内部没有进行内存释放，仅进行了相关父子关系调整
	RBTNode<T>* r = removeAt(v, del);
	if (NULL==del->parent)
	{
		//删除了root节点，root的左子接替root，染黑直接返回
		//此时所有节点到根的黑深度仍保持一致。
#ifdef RB_TREE_DEBUG
		cout << "Del Root" << endl;
#endif
		if (_root)
			_root->color = RBTColorBlack;
		return true;
	}

	//如果del和r的颜色不同，则只需将r置黑，r到根的黑深度不变，达到平衡。注意del、r不可能同时为红
	if (!r)
	{
		if (del->color == RBTColorRed)
		{
#ifdef RB_TREE_DEBUG
			cout << "Del R" << endl;
#endif
			delete del;
			return true;
		}
	}
	else if (del->color != r->color)
	{
#ifdef RB_TREE_DEBUG
		cout << "R->B" << endl;
#endif
		r->color = RBTColorBlack;
		delete del;
		return true;
	}
	
	//_hot此时用来记录被删节点的父节点，辅助在solveDoubleBlack定位此父节点，否则需要单独参数传入
	_hot = del->parent;
	delete del;
	//被删除节点和替代节点同时为黑，需要解决双黑冲突
	solveDoubleBlack(r);
	return true;
}

template <class T>
void RedBlackTree<T>::solveDoubleBlack(RBTNode<T>* v)
{
	//被删除节点和替代节点同时为黑
	//被删节点的父节点
	RBTNode<T>* p = v?v->parent:_hot;
	if (NULL == p)
	{
		//递归到达根节点，直接返回
#ifdef RB_TREE_DEBUG
		cout << "Root" << endl;
#endif
		return;
	}

	//s为被删节点的兄弟节点,因为被删除节点原来是存在的，且为黑，所以s必存在
	RBTNode<T>* s = (v == p->lchild) ? p->rchild: p->lchild;

	if (s->color==RBTColorBlack)
	{
		//s为黑
		if ((s->lchild&&s->lchild->color == RBTColorRed) || (s->rchild&&s->rchild->color == RBTColorRed))		//BB-1
		{
			//s为黑，且其至少一个孩子t为红
			/*	此时可以将t、s、p进行一次3+4重构，然后将新根节点染成原来根p的颜色，
				将新根的两个子染黑，这样所有孩子到局部根的黑高度仍和原来保存一样，
				而且新根颜色保持不变，整个树红黑平衡保持不变。
				
				原理：将局部树想象成4阶B树，ts构成超级节点，p构成超级节点，被删除节点v是黑，
				所以单独构成黑色节点，v的删除引起下溢，因此需要通过p借用ts里的节点，
				因为s有节点可以借，这里就相当于进行了3+4重构，最后根据需要重新染色即可

				BB-1 进行了1-2此旋转，2次染色，无需递归
			*/
#ifdef RB_TREE_DEBUG
			cout << "BB-1" << endl;
#endif

			RBTNode<T>* root;
			if (s->lchild && s->lchild->color == RBTColorRed)
				root = rotateAt(s->lchild);
			else
				root = rotateAt(s->rchild);

			//这里对调整后的局部树直接宏观上染色，非常直观
			root->color = p->color;
			root->lchild->color = RBTColorBlack;
			root->rchild->color = RBTColorBlack;
		}
		else
		{
			//s为黑，且其孩子均为黑
			if (p->color==RBTColorRed)														//BB-2R
			{
				//s为黑，其孩子均为黑,且父节点p为红
				/*	此时可以将p染黑，s染红即可，s孩子到p的黑高度不变，r到p的黑高度也不变，
					此时达到了局部平衡，p原来是红色因此也达到了整体平衡。

					原理：将局部树想象成4阶B树，s、v为黑，分别构成独立超级节点，p为红所以p和p的父构成超级节点
					由于删除v造成下溢，且s无多余节点借出，此时需要s、v借父节点p进行合并解决下溢，
					因此形成了sp组成的新超级节点，由于s为子，s染为红，p染为黑。又由于p原来为红，
					p的父必定为黑，p原来的超级节点必定至少有两个节点，因此即使p被借出，p和其父组成的超级
					节点也不会引起下溢，因此整个树也达到了红黑平衡

					BB-2R 进行2次染色，无需递归
				*/
#ifdef RB_TREE_DEBUG
				cout << "BB-2R" << endl;
#endif
				p->color = RBTColorBlack;
				s->color = RBTColorRed;
			}
			else																			//BB-2B
			{
				//s为黑，其孩子均为黑,且父节点p为黑
				/*	此时可以将s染红即可，s孩子到p的黑高度减1，删除了v，r到p的黑高度也减1，
					此时达到了局部树平衡，但可能破坏整体树红黑平衡。s被染红相当于删除了一个黑节点，
					而s和p均为黑，此时可能也产生了新的双黑冲突，需要继续向上解决双黑冲突。

					原理：将局部树想象成4阶B树，s、v、p均为黑，分别构成独立超级节点，由于删除v造成下溢，
					且s无多余节点借出，此时需要s、v借父节点p进行合并解决下溢，因此形成了sp组成的新超级节点，
					由于s为子，s染为红。又由于原来p为独立的超级节点，p被借出后，形成了新的下溢，需要向上层继续
					解决下溢。
					这种情形虽然可能引起多次递归染色（最多logn），但最多两次或三次旋转即可退出递归，完成整树的红黑平衡。

					BB-2B 进行1次染色，需继续递归
				*/
#ifdef RB_TREE_DEBUG
				cout << "BB-2B" << endl;
#endif
				s->color = RBTColorRed;
#ifdef RB_TREE_DEBUG
				print();
				cout << "---" << endl;
#endif
				solveDoubleBlack(p);

			}
		}
	}
	else																					//BB-3
	{
		//兄弟节点s为红（其孩子均为黑）
		/*	此时需对p进行zag或zig单旋转,交换s和p的位置和颜色，此时p为红色，s为黑色，s的黑子成了v的新兄弟s',s'为黑
			此时整个局部树转换成了BB-1或BB-2R的形式，后面再继续解决这两种简单的双黑冲突即可

			原理：对p进行单旋，构成出BB-1或BB-2R的形式，然后再继续解决简单的双黑冲突。
			由于BB-1或BB-2R内均不需递归就可完全解决双黑冲突，因此这种方法只需两次即可解决双黑冲突

			BB-3 进行了2次染成，1次旋转，然后转为BB-1或BB-2R
		*/
#ifdef RB_TREE_DEBUG
		cout << "BB-3" << endl;
#endif
		RBTNode<T>* g = p->parent;
		s->parent = g;
		if (NULL == g)
			_root = s;
		else
			(p == g->lchild) ? g->lchild = s : g->rchild = s;

		if (s == p->lchild)
		{
			p->lchild = s->rchild;
			if (s->rchild)
				s->rchild->parent = p;
			s->rchild = p;
		}
		else
		{
			p->rchild = s->lchild;
			if (s->lchild)
				s->lchild->parent = p;
			s->lchild = p;
		}

		p->parent = s;
		p->color = RBTColorRed;
		s->color = RBTColorBlack;

#ifdef RB_TREE_DEBUG
		print();
		cout << "---" << endl;
#endif
		solveDoubleBlack(v);
	}
}

/*	v为待删除节点，hot既为输入又为输出，hot做输入为次最后搜索的有效节点
	removeAt返回删除v后，v的替代节点。同时更新hot为实际删除节点,
	由于还需要使用实际被删除节点，removeAt内部没有进行内存释放，仅进行了相关父子关系调整	*/
template <class T>
RBTNode<T>* RedBlackTree<T>::removeAt(RBTNode<T>* v, RBTNode<T>*& hot)
{
	//记录v是否为左儿子
	bool bLeft;
	if (hot)
		bLeft = (v == hot->lchild) ? true : false;

	//被删结点的替代节点
	RBTNode<T>* r = v;
	//实际被删结点
	RBTNode<T>* del = v;
	//局部树根
	RBTNode<T>* root = v;
	if (!v->lchild)
	{
		r = v->rchild;
		if (r)
			r->parent = hot;
		root = r;
		//delete v;
	}
	else if (!v->rchild)
	{
		r = v->lchild;
		if (r)
			r->parent = hot;
		root = r;
		//delete v;
	}
	else
	{
		RBTNode<T>* succ = v->rchild;
		while (NULL!=succ->lchild)
			succ = succ->lchild;

		T key = v->key;
		v->key = succ->key;
		succ->key = key;

		r = succ->rchild;
		if (succ == v->rchild)
			v->rchild = r;
		else
			succ->parent->lchild = r;

		if (r)
			r->parent = succ->parent;
		del = succ;
		//delete succ;
	}

	if (hot)
	{
		if (bLeft)
			hot->lchild = root;
		else
			hot->rchild = root;
	}
	else
		_root = root;		//更新root
	
	//hot保存实际被删节点
	hot = del;
	return r;
}

template <class T>
void RedBlackTree<T>::print(ostream& os /*= cout*/)
{
	//树的深度
	int depth = getNodeDepth(_root);
	//用来临时存储每层的节点信息
	vector<vector<RBTNode<T>*> > vItems;
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
			RBTNode<T>* node = vItems[level][i];
			if (node)
			{
				if (RBTColorRed == node->color)
					os << left << setw(_item_wide) << redcolor << node->key << defcolor << right;
				else
					os << left << setw(_item_wide) << node->key << right;
			}
			else
			{
				os << left << setw(_item_wide) << " " << right;
				//os << left << setw(_item_wide) << "_" << right;
			}
			if (i + 1 != vItems[level].size())
				printSpace(inter_space_item_count, os);
		}
		os << endl;
	}
}

template <class T>
void RedBlackTree<T>::destroy()
{
	freeNode(_root);
	_hot = NULL;
}

template <class T>
void RedBlackTree<T>::freeNode(RBTNode<T> *& node)
{
	if (NULL == node)
		return;

	freeNode(node->lchild);
	freeNode(node->rchild);
	delete node;
	node = NULL;
}

template <class T>
int RedBlackTree<T>::getNodeDepth(RBTNode<T> *& node)
{
	if (NULL == node)
		return 0;

	return max(getNodeDepth(node->lchild), getNodeDepth(node->rchild)) + 1;
}

template <class T>
void RedBlackTree<T>::PreOrderGetTreeLevelNode(RBTNode<T> * node, int level, int depth, vector<vector<RBTNode<T>*> >& vItems)
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
void RedBlackTree<T>::printNode(RBTNode<T> * node, ostream& os /*= cout*/)
{
	if (NULL != node)
		os << node->key << " ";
}

template <class T>
void RedBlackTree<T>::printSpace(float fcount, ostream& os /*= cout*/)
{
	//由于fcount经常作为半个元素占用的空间输入，所以采用浮点类型，否则int无法表达0.5个元素占用的空间
	for (int i = 0; i < int(fcount*_item_wide); i++)
		os << " ";
}

template <class T>
int RedBlackTree<T>::getItemWide()
{
	return NODE_CHAR_WIDE + 1;
}

void testRedBlackTree();
template<class T> void saveDump(const vector<T>& record);
void reappear_manual();
void reappear_auto();
