#pragma once

#define MAX_QUEUEDS_LEN		128
namespace DS
{
	typedef struct Queue
	{
		void* queue[MAX_QUEUEDS_LEN];
		int head, tail;
	}Queue, *PQueue;

	void InitQueue(PQueue& q);
	void* DelQueue(PQueue& q);
	void EnQueue(PQueue& q, void* pitem);
	bool IsFullQueue(PQueue& q);
	bool IsEmptyQueue(PQueue& q);
	int SizeQueue(PQueue& q);
	void CopyQueue(PQueue& src, PQueue& dst);
}