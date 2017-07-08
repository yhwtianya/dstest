#include <stdio.h>
#include <iostream>
#include "mystr.h"
using namespace std;

int main()
{
	mystring* s = NULL;
	mystring* p = NULL;
	init_str(s);
	init_str(p);
	copystr(s, "ABDABCABABDEABDACDABDABDAAC");
	copystr(p, "ABDABD");
	int index = strstr_kmp(s, p);
	if (-1==index)
	{
		printf("in %s find no %s\n", s->data, p->data);
	}
	else
	{
		printf("in %s find %s\n", s->data, p->data);
		printf("find index start %d\n", index);
		mystring * ppre, *pnext, *pfind;
		mysubstr(s, 0, index, ppre);
		mysubstr(s, index, strlen(p->data), pfind);
		mysubstr(s, index+strlen(p->data), s->length - index - strlen(p->data), pnext);
		printf("%s\"%s\"%s\n", ppre->data, pfind->data, pnext->data);
	}
	return index;
}

