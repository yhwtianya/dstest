#pragma once
#include <stdio.h>
#include <stdlib.h>

#define STACK_MAX_SIZE		512
struct myitem
{
	int x;
	int y;
	int pre;
};

struct myqueue
{
	int front, rear;
	myitem data[STACK_MAX_SIZE];
};

void init_stack(myqueue *& s);

void destory_stack(myqueue *& s);

bool empty(myqueue *& s);

bool full(myqueue *& s);

bool enqueue(myqueue *&s, myitem e);

bool dequeue(myqueue *&s, myitem& e);

bool getfront(myqueue *&s, myitem** e);
void print_queue(myqueue *&s);
void print_d(int d);
bool mgpath_queue(int ix, int iy, int ox, int oy);
bool find_all_next(myqueue *&s);