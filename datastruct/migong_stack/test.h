#pragma once
#include <stdio.h>
#include <stdlib.h>

#define STACK_MAX_SIZE		100
struct myitem
{
	int x;
	int y;
	int d;
};

struct mystack
{
	int top;
	myitem data[STACK_MAX_SIZE];
};

void init_stack(mystack *& s);

void destory_stack(mystack *& s);

bool empty(mystack *& s);

bool full(mystack *& s);

bool push(mystack *&s, myitem e);

bool pop(mystack *&s, myitem& e);

bool gettop(mystack *&s, myitem** e);
void printf_stack(mystack *&s);
void print_d(int d);
bool mgpath_stack(int ix, int iy, int ox, int oy);
bool find_next(myitem& item, myitem &ne);