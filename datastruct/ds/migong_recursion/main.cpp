#include <stdio.h>
#include <iostream>
#include "test.h"
using namespace std;

int main()
{
	mypathstack *path;
	init_stack(path);
	mgpath_recursion(1, 1, 8, 8, path);
	//mgpath_stack(1, 1, 8, 8);
	return 0;
}

