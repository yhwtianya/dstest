#pragma once

template <class T>
class PQ
{
public:
	virtual ~PQ() {};
	virtual T popPQ() = 0;
	virtual void pushPQ(T item) = 0;
	virtual T topPQ() = 0;
	virtual int sizePQ() = 0;
	virtual bool emptyPQ() = 0;
};
