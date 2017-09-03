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
	�����
	�ɺ������ڵ���ɵ�BST����ɸ���Ⱦɫ��
	ͳһ�����ⲿ�ڵ�NULL��ʹ֮��Ϊ�������

	���������
	1. ����������Ϊ��ɫ
	2. �ⲿ�ڵ㣺��Ϊ��ɫ���ⲿ�ڵ�ָ�����ҳ�ڵ�
	3. ����ڵ㣺��Ϊ�죬��ֻ���кں��ӣ���֮�ӡ�֮���غڣ�
	4. �ⲿ�ڵ㵽����;�кڽڵ�(�������ⲿ�ڵ�)��Ŀ��ͬ������ȣ�
*/

//�ڵ���ɫ
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
	//����key���ڽڵ�
	RBTNode<T>* search(const T& key);
	//����key
	RBTNode<T>* insert(const T& key);
	//ɾ��key
	bool remove(const T& key);
	//����״�������ָ����
	void print(ostream& os = cout);
	//�����
	void destroy();

protected:
	//�ͷ������ռ�
	void freeNode(RBTNode<T> *& node);
	//��ȡ�������
	int getNodeDepth(RBTNode<T> *& node);
	//��������������ÿ��Ľڵ���Ϣ
	void PreOrderGetTreeLevelNode(RBTNode<T> * node, int level, int depth, vector<vector<RBTNode<T>*> >& vItems);
	//����ڵ���Ϣ
	void printNode(RBTNode<T> * node, ostream& os = cout);
	//���ָ�������Ŀո�
	void printSpace(float fcount, ostream& os = cout);
	//��ýڵ�ռ������Ŀ��
	int getItemWide();
	//���˫��
	void solveDoubleRed(RBTNode<T>* v);
	//3+4�ֲ�������ع��������µľֲ�����
	RBTNode<T>* connect34(RBTNode<T>* a, RBTNode<T>* b, RBTNode<T>* c, 
		RBTNode<T>* t1, RBTNode<T>* t2, RBTNode<T>* t3, RBTNode<T>* t4);
	//zigzig��zigzag��zagzag��zagzig��ת�������µľֲ�����
	RBTNode<T>* rotateAt(RBTNode<T>* v);
	//���˫��
	void solveDoubleBlack(RBTNode<T>* v);
	//hot��Ϊ������Ϊ���������ɾ��v��v������ڵ㡣ͬʱ����hotΪʵ��ɾ���ڵ�,�ڲ��������ڴ��ͷ�
	RBTNode<T>* removeAt(RBTNode<T>* v, RBTNode<T>*& hot);

protected:
	//��¼���������У����һ������������Ч�ڵ�
	RBTNode<T>* _hot;
	//����
	RBTNode<T>* _root;
	//�ڵ�ռ������Ŀ��
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

	//�����µĺ�ڵ�
	v = new RBTNode<T>(key, RBTColorRed, _hot, NULL, NULL);
	//������ܵ�˫���ͻ
	solveDoubleRed(v);
	//���Ƿ���keyΪx�Ľڵ�
	return v ? v : _hot->parent;
}

template <class T>
void RedBlackTree<T>::solveDoubleRed(RBTNode<T>* v)
{
	if (_root == v)
	{
		//vΪ�������ڵ�ֱ��Ⱦ��
		_root->color = RBTColorBlack;
		return;
	}

	//v��Ϊ��������ڸ��������Ϊ�ڣ������ڳ�ͻ��ֱ�ӷ���
	RBTNode<T>* p = v->parent;
	if (RBTColorBlack == p->color)	//p����Ϊroot
		return;	
	
	//***������˵������Ϊroot���ش����游��������v��p��g����Ч��v��pΪ�죬gΪ��***
	RBTNode<T>* g = p->parent;
	//�常�ڵ�
	RBTNode<T>* u = (p == g->lchild) ? g->rchild : g->lchild;

	if (!u || RBTColorBlack == u->color)		//RR-1 �常Ϊ��
	{
		/*	�常�ڵ�Ϊ��ɫ��v��pΪ�죬u��gΪ��
		��ʱԭ���ⲿ�ڵ㵽�游�ڵ�ĺڸ߶�Ϊ2������һ���常���ⲿ�ڵ㵽�游���ɡ�
		�����������v��p��g���һ��4��B���ĳ����ڵ㣬�����ڵ㺢�ӣ��ⲿ�ڵ㣩��Ϊ��ɫ��uΪg�ĺ��ӣ���
		ֻҪ����3+4�ع�������ȼ�ת������ʹ�����ڵ�ΪRBR����ں죩��ʽ����ʱ����ԭ�ض��׺������
		�µ��游��v��p����Ϊ�ڣ��游����������Ϊ�죬�ⲿ�ڵ����������ӵĺ��ӣ���Ϊ�ڣ�
		��ʱ�����ⲿ�ڵ㵽�游�ĺڸ߶��Ծ�Ϊ2����ʱ�Ѿ����ɺ������ƽ��
		������������ֲ�������1--2���ع���2��Ⱦɫ���������ϲ�����ṹ������Ⱦɫ
		����Ⱦɫ����ת��Ҫ2��Ⱦɫ����Ⱦɫʱ�кܶ��жϣ�����ת��Ⱦɫ��Ҫ3��Ⱦɫ������ֱ�ۣ�
		*/

		//rotateAt�ڲ�ʵ�������游�����游��������
		RBTNode<T>* root = rotateAt(v);
		//����Ե�����ľֲ���ֱ�Ӻ����Ⱦɫ���ǳ�ֱ��
		root->color = RBTColorBlack;
		root->lchild->color = RBTColorRed;
		root->rchild->color = RBTColorRed;

		//��ת���������������ڵ�pΪ�ֲ�����Ⱦ�ڣ�v��gΪp���Ӳ�Ⱦ�죬����ʹ���߽��ͣ������ֺ��������
	}
	else										//RR-2 �常Ϊ��
	{
		/*	�常�ڵ�Ϊ��ɫ�����常�к�ɫ�ⲿ�ڵ㣬v��p��u��Ϊ�죬gΪ��
		��ʱԭ���ⲿ�ڵ㵽�游�ڵ�ĺڸ߶�Ϊ1������һ���常���游���ɡ�
		�����������Ҫ�����κνṹ������ֻҪ��p��uȾ�ڣ�gȾ��
		��ʱ�����ⲿ�ڵ㵽�游�ĺڸ߶��Ծ�Ϊ1����g�ڵ�Ϊ�죬���Ͽ��������µ�˫���ͻ��
		��ʱ������g���ϵݹ���˫���ͻ��ֱ�������������
		������������ֲ�������0���ع���3��Ⱦɫ�����������ϼ����ݹ����˫���ͻ���
		���ϵݹ飬�ǰ�˫�����ϵݹ�ģ��ݹ�������������ͻ���ȿ�����RR-1Ҳ������RR-2��
		����һ���õ�����ǣ�һ������RR-1��������һ����ת�����󣬵ݹ�ͽ����ˡ�
		���Ժ�������˫������ֻ�����1--2���ع�����������logn�ε�Ⱦɫ����������ɡ�
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

//3+4�ֲ�������ع�������ͨ����ת�ﵽƽ�⣬���ǽ����ڵ㡢���ڵ㡢�游�ڵ㼰�ֲ�������Ҷ�ӽڵ�ȫ���𿪣�
//����ԭ���Ĵ�С��ϵֱ���ع���һ���ֲ�ƽ������������û����ת�����Ĳ����ˣ������нڵ��������飬��ֱ�ۡ�
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

	//����ʵ�������游��������
	if (b->parent)
	{
		if (b->key < b->parent->key)
			b->parent->lchild = b;
		else
			b->parent->rchild = b;
	}
	else
	{
		//û�и������ø�ֱ������
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
			//�����������游��connect34�ڲ�ʵ������������
			p->parent = g->parent;
			return connect34(v, p, g,
				v->lchild, v->rchild, p->rchild, g->rchild);
		} 
		else					//zigzag
		{
			//�����������游��connect34�ڲ�ʵ������������
			v->parent = g->parent;
			return connect34(p, v, g,
				p->lchild, v->lchild, v->rchild, g->rchild);
		}
	}
	else					//zag
	{
		if (v == p->lchild)		//zagzig
		{
			//�����������游��connect34�ڲ�ʵ������������
			v->parent = g->parent;
			return connect34(g, v, p, 
				g->lchild, v->lchild, v->rchild, p->rchild);
		}
		else					//zagzag
		{
			//�����������游��connect34�ڲ�ʵ������������
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
	//removeAt����ɾ��v��v������ڵ㡣ͬʱ����delΪʵ��ɾ���ڵ�,
	//���ڻ���Ҫʹ��ʵ�ʱ�ɾ���ڵ㣬removeAt�ڲ�û�н����ڴ��ͷţ�����������ظ��ӹ�ϵ����
	RBTNode<T>* r = removeAt(v, del);
	if (NULL==del->parent)
	{
		//ɾ����root�ڵ㣬root�����ӽ���root��Ⱦ��ֱ�ӷ���
		//��ʱ���нڵ㵽���ĺ�����Ա���һ�¡�
#ifdef RB_TREE_DEBUG
		cout << "Del Root" << endl;
#endif
		if (_root)
			_root->color = RBTColorBlack;
		return true;
	}

	//���del��r����ɫ��ͬ����ֻ�轫r�úڣ�r�����ĺ���Ȳ��䣬�ﵽƽ�⡣ע��del��r������ͬʱΪ��
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
	
	//_hot��ʱ������¼��ɾ�ڵ�ĸ��ڵ㣬������solveDoubleBlack��λ�˸��ڵ㣬������Ҫ������������
	_hot = del->parent;
	delete del;
	//��ɾ���ڵ������ڵ�ͬʱΪ�ڣ���Ҫ���˫�ڳ�ͻ
	solveDoubleBlack(r);
	return true;
}

template <class T>
void RedBlackTree<T>::solveDoubleBlack(RBTNode<T>* v)
{
	//��ɾ���ڵ������ڵ�ͬʱΪ��
	//��ɾ�ڵ�ĸ��ڵ�
	RBTNode<T>* p = v?v->parent:_hot;
	if (NULL == p)
	{
		//�ݹ鵽����ڵ㣬ֱ�ӷ���
#ifdef RB_TREE_DEBUG
		cout << "Root" << endl;
#endif
		return;
	}

	//sΪ��ɾ�ڵ���ֵܽڵ�,��Ϊ��ɾ���ڵ�ԭ���Ǵ��ڵģ���Ϊ�ڣ�����s�ش���
	RBTNode<T>* s = (v == p->lchild) ? p->rchild: p->lchild;

	if (s->color==RBTColorBlack)
	{
		//sΪ��
		if ((s->lchild&&s->lchild->color == RBTColorRed) || (s->rchild&&s->rchild->color == RBTColorRed))		//BB-1
		{
			//sΪ�ڣ���������һ������tΪ��
			/*	��ʱ���Խ�t��s��p����һ��3+4�ع���Ȼ���¸��ڵ�Ⱦ��ԭ����p����ɫ��
				���¸���������Ⱦ�ڣ��������к��ӵ��ֲ����ĺڸ߶��Ժ�ԭ������һ����
				�����¸���ɫ���ֲ��䣬���������ƽ�Ᵽ�ֲ��䡣
				
				ԭ�����ֲ��������4��B����ts���ɳ����ڵ㣬p���ɳ����ڵ㣬��ɾ���ڵ�v�Ǻڣ�
				���Ե������ɺ�ɫ�ڵ㣬v��ɾ���������磬�����Ҫͨ��p����ts��Ľڵ㣬
				��Ϊs�нڵ���Խ裬������൱�ڽ�����3+4�ع�����������Ҫ����Ⱦɫ����

				BB-1 ������1-2����ת��2��Ⱦɫ������ݹ�
			*/
#ifdef RB_TREE_DEBUG
			cout << "BB-1" << endl;
#endif

			RBTNode<T>* root;
			if (s->lchild && s->lchild->color == RBTColorRed)
				root = rotateAt(s->lchild);
			else
				root = rotateAt(s->rchild);

			//����Ե�����ľֲ���ֱ�Ӻ����Ⱦɫ���ǳ�ֱ��
			root->color = p->color;
			root->lchild->color = RBTColorBlack;
			root->rchild->color = RBTColorBlack;
		}
		else
		{
			//sΪ�ڣ����亢�Ӿ�Ϊ��
			if (p->color==RBTColorRed)														//BB-2R
			{
				//sΪ�ڣ��亢�Ӿ�Ϊ��,�Ҹ��ڵ�pΪ��
				/*	��ʱ���Խ�pȾ�ڣ�sȾ�켴�ɣ�s���ӵ�p�ĺڸ߶Ȳ��䣬r��p�ĺڸ߶�Ҳ���䣬
					��ʱ�ﵽ�˾ֲ�ƽ�⣬pԭ���Ǻ�ɫ���Ҳ�ﵽ������ƽ�⡣

					ԭ�����ֲ��������4��B����s��vΪ�ڣ��ֱ𹹳ɶ��������ڵ㣬pΪ������p��p�ĸ����ɳ����ڵ�
					����ɾ��v������磬��s�޶���ڵ�������ʱ��Ҫs��v�踸�ڵ�p���кϲ�������磬
					����γ���sp��ɵ��³����ڵ㣬����sΪ�ӣ�sȾΪ�죬pȾΪ�ڡ�������pԭ��Ϊ�죬
					p�ĸ��ض�Ϊ�ڣ�pԭ���ĳ����ڵ�ض������������ڵ㣬��˼�ʹp�������p���丸��ɵĳ���
					�ڵ�Ҳ�����������磬���������Ҳ�ﵽ�˺��ƽ��

					BB-2R ����2��Ⱦɫ������ݹ�
				*/
#ifdef RB_TREE_DEBUG
				cout << "BB-2R" << endl;
#endif
				p->color = RBTColorBlack;
				s->color = RBTColorRed;
			}
			else																			//BB-2B
			{
				//sΪ�ڣ��亢�Ӿ�Ϊ��,�Ҹ��ڵ�pΪ��
				/*	��ʱ���Խ�sȾ�켴�ɣ�s���ӵ�p�ĺڸ߶ȼ�1��ɾ����v��r��p�ĺڸ߶�Ҳ��1��
					��ʱ�ﵽ�˾ֲ���ƽ�⣬�������ƻ����������ƽ�⡣s��Ⱦ���൱��ɾ����һ���ڽڵ㣬
					��s��p��Ϊ�ڣ���ʱ����Ҳ�������µ�˫�ڳ�ͻ����Ҫ�������Ͻ��˫�ڳ�ͻ��

					ԭ�����ֲ��������4��B����s��v��p��Ϊ�ڣ��ֱ𹹳ɶ��������ڵ㣬����ɾ��v������磬
					��s�޶���ڵ�������ʱ��Ҫs��v�踸�ڵ�p���кϲ�������磬����γ���sp��ɵ��³����ڵ㣬
					����sΪ�ӣ�sȾΪ�졣������ԭ��pΪ�����ĳ����ڵ㣬p��������γ����µ����磬��Ҫ���ϲ����
					������硣
					����������Ȼ���������εݹ�Ⱦɫ�����logn������������λ�������ת�����˳��ݹ飬��������ĺ��ƽ�⡣

					BB-2B ����1��Ⱦɫ��������ݹ�
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
		//�ֵܽڵ�sΪ�죨�亢�Ӿ�Ϊ�ڣ�
		/*	��ʱ���p����zag��zig����ת,����s��p��λ�ú���ɫ����ʱpΪ��ɫ��sΪ��ɫ��s�ĺ��ӳ���v�����ֵ�s',s'Ϊ��
			��ʱ�����ֲ���ת������BB-1��BB-2R����ʽ�������ټ�����������ּ򵥵�˫�ڳ�ͻ����

			ԭ����p���е��������ɳ�BB-1��BB-2R����ʽ��Ȼ���ټ�������򵥵�˫�ڳ�ͻ��
			����BB-1��BB-2R�ھ�����ݹ�Ϳ���ȫ���˫�ڳ�ͻ��������ַ���ֻ�����μ��ɽ��˫�ڳ�ͻ

			BB-3 ������2��Ⱦ�ɣ�1����ת��Ȼ��תΪBB-1��BB-2R
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

/*	vΪ��ɾ���ڵ㣬hot��Ϊ������Ϊ�����hot������Ϊ�������������Ч�ڵ�
	removeAt����ɾ��v��v������ڵ㡣ͬʱ����hotΪʵ��ɾ���ڵ�,
	���ڻ���Ҫʹ��ʵ�ʱ�ɾ���ڵ㣬removeAt�ڲ�û�н����ڴ��ͷţ�����������ظ��ӹ�ϵ����	*/
template <class T>
RBTNode<T>* RedBlackTree<T>::removeAt(RBTNode<T>* v, RBTNode<T>*& hot)
{
	//��¼v�Ƿ�Ϊ�����
	bool bLeft;
	if (hot)
		bLeft = (v == hot->lchild) ? true : false;

	//��ɾ��������ڵ�
	RBTNode<T>* r = v;
	//ʵ�ʱ�ɾ���
	RBTNode<T>* del = v;
	//�ֲ�����
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
		_root = root;		//����root
	
	//hot����ʵ�ʱ�ɾ�ڵ�
	hot = del;
	return r;
}

template <class T>
void RedBlackTree<T>::print(ostream& os /*= cout*/)
{
	//�������
	int depth = getNodeDepth(_root);
	//������ʱ�洢ÿ��Ľڵ���Ϣ
	vector<vector<RBTNode<T>*> > vItems;
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
void RedBlackTree<T>::printNode(RBTNode<T> * node, ostream& os /*= cout*/)
{
	if (NULL != node)
		os << node->key << " ";
}

template <class T>
void RedBlackTree<T>::printSpace(float fcount, ostream& os /*= cout*/)
{
	//����fcount������Ϊ���Ԫ��ռ�õĿռ����룬���Բ��ø������ͣ�����int�޷����0.5��Ԫ��ռ�õĿռ�
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
