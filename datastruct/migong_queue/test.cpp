#include "test.h"

int mg[10][10] = {
	1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,1,1,1,1,1,1,
	1,0,1,0,0,0,0,0,1,1,
	1,0,1,1,1,0,1,1,1,1,
	1,0,0,1,1,0,0,1,1,1,
	1,0,0,0,0,1,0,0,0,1,
	1,0,1,0,0,0,0,1,0,1,
	1,0,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,1
};

void init_stack(myqueue *& s)
{
	s = (myqueue*)malloc(sizeof(myqueue));
	s->front = s->rear = -1;
}

void destory_stack(myqueue *& s)
{
	free(s);
}

bool empty(myqueue *& s)
{
	return (s->front == s->rear);
}

bool full(myqueue *& s)
{
	return (s->rear+1 == STACK_MAX_SIZE);
}

bool enqueue(myqueue *&s, myitem e)
{
	if (full(s))
	{
		printf("queue over memory!\n");
		return false;
	}

	++(s->rear);
	s->data[s->rear] = e;
	return true;
}

bool dequeue(myqueue *&s, myitem& e)
{
	if (empty(s))
	{
		return false;
	}

	++(s->front);
	e = s->data[s->front];
	return true;
}

bool getfront(myqueue *&s, myitem** e)
{
	if (empty(s))
	{
		return false;
	}

	*e = &(s->data[s->front+1]);
	return true;
}

void print_queue(myqueue *&s)
{
	myitem *item = NULL;
	getfront(s, &item);
	int nstep = 0;
	while(item->pre != -1)
	{
		printf("(%d, %d)\n", item->x, item->y);
		*item = s->data[item->pre];
		++nstep;
	}

	printf("(%d, %d)\n", item->x, item->y);
	printf("total %d stup\n", nstep);
	printf("total queue data %d\n", s->rear + 1);
}

void print_d(int d)
{
	switch (d)
	{
	case 0:
		printf("up");
		break;
	case 1:
		printf(">");
		break;
	case 2:
		printf("down");
		break;
	case 3:
		printf("<");
		break;

	default:
		printf("over");
		break;
	}
}

bool mgpath_queue(int ix, int iy, int ox, int oy)
{
	myitem item;
	item.x = ix;
	item.y = iy;
	item.pre = -1;
	myqueue *s = NULL;
	init_stack(s);
	enqueue(s, item);
	myitem *top_e = NULL;
	while (getfront(s, &top_e))
	{
		//printf("(%d, %d)", top_e->x, top_e->x);
		if (top_e->x == ox && top_e->y == oy)
		{
			break;
		}
		find_all_next(s);
	}

	if (top_e->x == ox && top_e->y == oy)
	{
		printf("find match path from (%d, %d) to (%d, %d)\n", ix, iy, ox, oy);
		print_queue(s);
	}
	else
	{
		printf("no find match path from (%d, %d) to (%d, %d)\n", ix, iy, ox, oy);
		return false;
	}

	return true;
}

bool find_all_next(myqueue *&s)
{
	int front = s->front + 1;
	myitem item;
	dequeue(s, item);
	mg[item.y][item.x] = -1;
	int d = 0;
	for (; d < 4; ++d)
	{
		int x = item.x;
		int y = item.y;
		switch (d)
		{
		case 0:
		{
			y--;
			break;
		}
		case 1:
		{
			x++;
			break;
		}
		case 2:
		{
			y++;
			break;
		}
		case 3:
		{
			x--;
			break;
		}

		}
		if (mg[y][x] == 0)
		{
			myitem ne;
			ne.x = x;
			ne.y = y;
			ne.pre = front;
			enqueue(s, ne);
		}
	}

	return true;
}