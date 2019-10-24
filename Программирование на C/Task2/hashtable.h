#ifndef _hashtable_h
#define _hashtable_h
#include <stdio.h>

enum {HASHSIZE=10000};

typedef struct node {
	char 	*stroka;
	int 	num;
	struct 	node *next;
} ST;

extern ST mas[HASHSIZE];

void fun_hashtable(char *l_word);

#endif 
