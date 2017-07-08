#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_MAX_SIZE		100
struct mystring
{
	char data[STRING_MAX_SIZE];
	int length;
};

void init_str(mystring *& s);

void free_str(mystring*& s);

bool empty(mystring *& s);

bool full(mystring *& s);

int getlength(mystring *& s);

int copystr(mystring*& s, const char *ps);

bool mysubstr(mystring*& s, int start, int len, mystring *& dst);

void get_kmp_next(mystring*& s, int next[]);

int strstr_kmp(mystring*& s, mystring*&p);