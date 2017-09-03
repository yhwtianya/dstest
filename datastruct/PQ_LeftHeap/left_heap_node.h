#pragma once


template <class T>
class LeftHeapNode
{
public:
	T key;
	unsigned int NPL;		//空节点路径长度
	LeftHeapNode<T>* parent;
	LeftHeapNode<T>* lchild;
	LeftHeapNode<T>* rchild;
	LeftHeapNode(T k, LeftHeapNode<T>* p, LeftHeapNode<T>* l, LeftHeapNode<T>* r)
		:key(k), parent(p), lchild(l), rchild(r), NPL(1){}
};

