#pragma once
#include <stdlib.h>
#include <stdio.h>
#define MAX_STACKDS_LEN		128

namespace DS
{
	typedef struct Stack
	{
		void* data[MAX_STACKDS_LEN];
		int top;
	}Stack, *PStack;
	void InitStack(PStack & s);
	void DestoryStack(PStack & s);
	bool IsEmptyStack(PStack & s);
	bool IsFullStack(PStack & s);
	bool PushStack(PStack &s, void* v);
	void* PopStack(PStack &s);
	void* GetTopStack(PStack &s);
	int SizeStack(PStack& s);
}