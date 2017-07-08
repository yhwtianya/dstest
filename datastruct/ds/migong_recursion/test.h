#pragma once
#include <stdio.h>
#include <stdlib.h>

#define STACK_MAX_SIZE		100
struct mypathnode
{
	int x;
	int y;
};

struct mypathstack
{
	int length;
	mypathnode data[STACK_MAX_SIZE];
};

void init_stack(mypathstack *& s);

void destory_stack(mypathstack *& s);

bool empty(mypathstack *& s);

bool full(mypathstack *& s);

bool push(mypathstack *&s, mypathnode e);

bool pop(mypathstack *&s, mypathnode& e);

bool gettop(mypathstack *&s, mypathnode** e);
void printf_stack(mypathstack *&s);
void print_d(int d);
void mgpath_recursion(int ix, int iy, int ox, int oy, mypathstack *& path);
bool find_next(mypathnode& item, mypathnode &ne);