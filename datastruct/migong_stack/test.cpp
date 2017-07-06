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

void init_stack(mystack *& s)
{
	s = (mystack*)malloc(sizeof(mystack));
	s->top = -1;
}

void destory_stack(mystack *& s)
{
	free(s);
}

bool empty(mystack *& s)
{
	return (s->top == -1);
}

bool full(mystack *& s)
{
	return (s->top + 1 == STACK_MAX_SIZE);
}

bool push(mystack *&s, myitem e)
{
	if (full(s))
	{
		return false;
	}

	s->top++;
	s->data[s->top] = e;
	return true;
}

bool pop(mystack *&s, myitem& e)
{
	if (empty(s))
	{
		return false;
	}

	e = s->data[s->top];
	s->top--;
	return true;
}

bool gettop(mystack *&s, myitem** e)
{
	if (empty(s))
	{
		return false;
	}

	*e = &(s->data[s->top]);
	return true;
}

void printf_stack(mystack *&s)
{
	myitem e;
	mystack *stack_temp = NULL;
	init_stack(stack_temp);
	while (pop(s, e))
	{
		push(stack_temp, e);
		//printf("(%d, %d, %d) ", e.x, e.y, e.d);
	}

	while (pop(stack_temp, e))
	{
		printf("(%d, %d, %d, %d) ", e.x, e.y, e.d, mg[e.y][e.x]);
		print_d(e.d);
		printf("\n");
	}
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

bool mgpath_stack(int ix, int iy, int ox, int oy)
{
	myitem item;
	item.x = ix;
	item.y = iy;
	item.d = -2;
	mystack *s = NULL;
	init_stack(s);
	push(s, item);
	myitem *top_e = NULL;
	while (gettop(s, &top_e) && (top_e->x != ox || top_e->y != oy))
	{
		//printf("(%d, %d)", top_e->x, top_e->x);
		myitem ne;
		bool find = find_next(*top_e, ne);

		if (!find)
		{
			myitem temp_e;
			pop(s, temp_e);
			printf("--pop(%d,%d)\n", temp_e.x, temp_e.y);
			mg[temp_e.y][temp_e.x] = -1;
		}
		else
		{
			push(s, ne);
			printf("++push(%d,%d)\n", ne.x, ne.y);
			mg[ne.y][ne.x] = -2;
		}
	}

	if (empty(s))
	{
		printf("no find match path from (%d, %d) to (%d, %d)\n", ix, iy, ox, oy);
		return false;
	}

	printf("find match path from (%d, %d) to (%d, %d)\n", ix, iy, ox, oy);
	printf_stack(s);
	return true;
}

bool find_next(myitem& item, myitem &ne)
{
	bool find = false;
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
			find = true;
			ne.x = x;
			ne.y = y;
			ne.d = -1;
			item.d = d;
			break;;
		}
	}

	return find;
}