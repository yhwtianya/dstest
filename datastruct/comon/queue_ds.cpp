#include "queue_ds.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

void DS::InitQueue(PQueue& q)
{
	if (NULL == q)
	{
		q = (PQueue)malloc(sizeof(Queue));
		q->head = q->tail = 0;
	}
	else
		printf("error:queue is not null\n");
}

void* DS::DelQueue(PQueue& q)
{
	if (IsEmptyQueue(q))
	{
		return NULL;
	}
	void* p = q->queue[q->tail];
	q->tail = (q->tail + 1) % MAX_QUEUEDS_LEN;
	return p;
}

void DS::EnQueue(PQueue& q, void* pitem)
{
	if (IsFullQueue(q))
	{
		printf("queue is full\n");
		return;
	}
	q->queue[q->head] = pitem;
	q->head = (q->head + 1) % MAX_QUEUEDS_LEN;
}

bool DS::IsFullQueue(PQueue& q)
{
	if (((q->head + 1) % MAX_QUEUEDS_LEN) == q->tail)
		return true;
	return false;
}

bool DS::IsEmptyQueue(PQueue& q)
{
	return q->head == q->tail;
}

int DS::SizeQueue(PQueue& q)
{
	int size = q->head - q->tail;
	if (size<0)
	{
		size += MAX_QUEUEDS_LEN;
	}

	return size;
}

void DS::CopyQueue(PQueue& src, PQueue& dst)
{
	if (NULL==dst)
		InitQueue(dst);

	memcpy(dst->queue, src->queue, sizeof(void*)*MAX_QUEUEDS_LEN);
	dst->head = src->head;
	dst->tail = src->tail;
}

