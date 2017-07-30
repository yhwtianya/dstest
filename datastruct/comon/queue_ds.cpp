#include "queue_ds.h"
#include <stdlib.h>
#include <stdio.h>

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

