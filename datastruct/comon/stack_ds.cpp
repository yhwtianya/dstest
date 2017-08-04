#include "stack_ds.h"
#include <stdlib.h>
#include <stdio.h>

void DS::InitStack(PStack & s)
{
	if (NULL == s)
	{
		s = (PStack)malloc(sizeof(Stack));
		s->top = -1;
	}
	else
		printf("error:stack is not null\n");
}

void DS::DestoryStack(PStack & s)
{
	free(s);
	s = NULL;
}

bool DS::IsEmptyStack(PStack & s)
{
	return (s->top == -1);
}

bool DS::IsFullStack(PStack & s)
{
	return (s->top + 1 == MAX_STACKDS_LEN);
}

bool DS::PushStack(PStack &s, void* v)
{
	if (IsFullStack(s))
	{
		return false;
	}

	s->top++;
	s->data[s->top] = v;
	return true;
}

void* DS::PopStack(PStack &s)
{
	if (IsEmptyStack(s))
	{
		return NULL;
	}

	void* v = s->data[s->top];
	s->top--;
	return v;
}

void* DS::GetTopStack(PStack &s)
{
	if (IsEmptyStack(s))
	{
		return NULL;
	}

	void* v = &(s->data[s->top]);
	return v;
}

int DS::SizeStack(PStack& s)
{
	return s->top + 1;
}
