#include "test.h"

int mg[10][10] = {
	1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,1,1,1,1,1,1,
	1,0,1,0,0,0,0,0,1,1,
	1,0,1,1,1,0,1,1,1,1,
	1,0,0,1,1,0,0,1,1,1,
	1,0,0,0,0,1,0,0,0,1,
	1,0,1,0,0,0,0,1,0,1,
	1,0,1,1,1,0,1,1,1,1,
	1,0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,1
};

void init_stack(mypathstack *& s)
{
	s = (mypathstack*)malloc(sizeof(mypathstack));
	s->length = 0;
}

void destory_stack(mypathstack *& s)
{
	free(s);
}

bool empty(mypathstack *& s)
{
	return (s->length == 0);
}

bool full(mypathstack *& s)
{
	return (s->length == STACK_MAX_SIZE);
}

bool push(mypathstack *&s, mypathnode e)
{
	if (full(s))
	{
		printf("error: queue is full\n");
		return false;
	}

	s->data[s->length] = e;
	s->length++;
	return true;
}

bool pop(mypathstack *&s, mypathnode& e)
{
	if (empty(s))
	{
		return false;
	}

	s->length--;
	e = s->data[s->length];
	return true;
}

bool gettop(mypathstack *&s, mypathnode** e)
{
	if (empty(s))
	{
		return false;
	}

	*e = &(s->data[s->length-1]);
	return true;
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

void printpath(mypathstack *& s)
{
	static int count = 0;
	printf("--- methon %d ---\n", ++count);
	for (int i = 0; i < s->length; i++)
	{
		printf("(%d, %d)\n", s->data[i].x, s->data[i].y);
	}
	printf("---total %d step.\n\n", s->length);
}

void mgpath_recursion(int ix, int iy, int ox, int oy, mypathstack *& path)
{
	if (ix==ox && iy==oy)
	{
		mypathnode e;
		e.x = ix;
		e.y = iy;
		push(path, e);
		printpath(path);
		pop(path, e);
	}
	else
	{
		if(mg[iy][ix] == 0)
		{
			int d = -1;
			for (d = 0; d < 4; d++)
			{
				int x = ix;
				int y = iy;
				switch (d)
				{
				case 0:
					y--;
					break;
				case 1:
					x++;
					break;
				case 2:
					y++;
					break;
				case 3:
					x--;
					break;
				}

				mypathnode e;
				e.x = ix;
				e.y = iy;
				push(path, e);
				mg[iy][ix] = -1;
				mgpath_recursion(x, y, ox, oy, path);
				pop(path, e);
				mg[iy][ix] = 0;
			}
		}
	}
}

bool find_next(mypathnode& item, mypathnode &ne)
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
			break;;
		}
	}

	return find;
}