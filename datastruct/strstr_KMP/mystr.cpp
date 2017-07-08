#include "mystr.h"

void init_str(mystring *& s)
{
	s = (mystring*)malloc(sizeof(mystring));
	s->length = 0;
	s->data[s->length] = '\0';
}

void free_str(mystring*& s)
{
	free(s);
}

bool empty(mystring *& s)
{
	return 0 == s->length;
}

bool full(mystring *& s)
{
	return s->length + 1 == STRING_MAX_SIZE;
}

int getlength(mystring *& s)
{
	return s->length;
}

int copystr(mystring*& s, const char *ps)
{
	int len = strlen(ps);
	if (len < STRING_MAX_SIZE)
	{
		memcpy(s->data, (void*)ps, len * sizeof(char));
		s->data[len] = '\0';
		s->length = len;
		return len;
	}

	return 0;
}

bool mysubstr(mystring*& s, int start, int len, mystring *& dst)
{
	if (start + len <= s->length)
	{
		init_str(dst);
		char* pdst = (dst->data);
		char* psrc = &(s->data[start]);
		memcpy(pdst, psrc, len);
		dst->length = len;
		dst->data[len] = '\0';
		return true;
	}

	return false;
}

void get_kmp_next(mystring*& s, int next[])
{
	int j = 0;
	int k = -1;
	next[0] = -1;
	while (j < s->length)
	{
		if (k == -1 || s->data[j] == s->data[k])
		{
			++k;
			++j;
			if (s->data[j] == s->data[k])
				next[j] = next[k];
			else
				next[j] = k;
		}
		else
		{
			k = next[k];
		}
	}
}

int strstr_kmp(mystring*& s, mystring*&p)
{
	int next[32] = { 0 };
	get_kmp_next(p, next);
	int i = 0;
	int j = 0;
	while (i < s->length && j < p->length)
	{
		if (j == -1 || p->data[j] == s->data[i])
		{
			j++;
			i++;
		}
		else
		{
			j = next[j];
		}
	}

	if (j == p->length)
	{
		return i-p->length;
	}

	return -1;
}
