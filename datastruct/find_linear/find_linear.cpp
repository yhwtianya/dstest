#include "find_linear.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int SeqSearch(RecordList l, KeyType k)
{
	int i = l.length;
	while (i > 0 && k != l.r[i].key)
		--i;

	if (i > 0) return i;
	return 0;
}

int SeqSearch_Post(RecordList l, KeyType k)
{
	//监视哨防止越界，并减少了循环里的一条比较语句
	l.r[0].key = k;
	int i = l.length;
	while (k != l.r[i].key)
		--i;

	return i;
}

int BinSearch(RecordList l, KeyType k)
{
	int low = 1;
	int high = l.length;
	int mid;
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (l.r[mid].key == k)
			return mid;
		else if (k < l.r[mid].key)
			high = mid - 1;
		else
			low = mid + 1;
	}

	return 0;
}

void TestFindLinear()
{
	RecordList l;
	l.length = LIST_SIZE;
	for (size_t i = 0; i < l.length; i++)
	{
		l.r[i+1].key = l.r[i+1].oter_data = i + 1;
	}

	int key = 3;
	int pos;
	pos = SeqSearch(l, key);
	printf("SeqSearch find key %d, posization is %d.\n", key, pos);
	pos = SeqSearch_Post(l, key);
	printf("SeqSearch_Post find key %d, posization is %d.\n", key, pos);
	pos = BinSearch(l, key);
	printf("BinSearch find key %d, posization is %d.\n", key, pos);
}
