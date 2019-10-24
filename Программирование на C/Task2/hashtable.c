#include "hashtable.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*********************************/
/* HASH === Function that finds new indexes in case of collision
 */
/*********************************/
int hash(char *l_word)  // function that finds new indexes in case of collision
{
	long long res = 0;
	for(res=0; *l_word !='\0'; l_word++)
		res= *l_word + res;			 //res -> hash function

	return (int) (res % HASHSIZE);
}

/*********************************/
/* HASHTABLE
 * Looking with help of <hash> for index in the associative array
 * If the array element is a blank list then we create the list
 * Else we go into the list, looking for appropriate elements
 * If we found the appropriate string then num++
 * Else we create a new node
 */
/*********************************/
void fun_hashtable(char *l_word)
{
	ST *p, *pnext;

	int ind=0, strl=0;

	ind=hash(l_word);
	strl=strlen(l_word);

	if (mas[ind].stroka != NULL)
	{
			if (strcmp(l_word, mas[ind].stroka) == 0) { mas[ind].num++; return; }
			else for (p = mas[ind].next; p != NULL; p = p->next)
					if (p->stroka != NULL && strcmp(l_word, p->stroka) == 0) {(p->num)++; return;}
	}

	if (mas[ind].stroka == NULL)
	{
			mas[ind].stroka = (char *)realloc(mas[ind].stroka, (strl+1)*sizeof(l_word[0]));
			strncpy( mas[ind].stroka, l_word, strl+1);
			mas[ind].stroka[strl]='\0';
			mas[ind].num = 1;
			return;
	}
	else
	{
		pnext = NULL;
		pnext = (ST *)malloc(sizeof(ST));		 // Creating node
		pnext->stroka = NULL;					 // Filling the field "stroka"
		pnext->stroka = (char *)realloc(pnext->stroka, (strl+1)*sizeof(l_word[0]));
		strncpy( pnext->stroka, l_word, strl+1);
		pnext->stroka[strl]='\0';
		pnext->num = 1;							 // Filling the field "num"
		pnext->next = NULL;

		if (mas[ind].next == NULL) { mas[ind].next = pnext; return; }

		p = mas[ind].next;
		while (p->next != NULL)
			p = p->next;

		p->next = pnext;
	}

}
