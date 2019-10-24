#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"
#include "hashtable.h"

ST mas[HASHSIZE];

FILE *F1;

/*********************************/
//         OTHER FUNCTIONS       //
/*********************************/
void print_list1 (ST lfirst)
{
	ST *q;

	printf("%4d %s ", lfirst.num, lfirst.stroka);

	q = lfirst.next;
	while (q != NULL)
	{
		printf(" -> %4d %s", q->num, q->stroka);
		q = (*q).next;
	}
}

/*********************************/
void print_list (ST lfirst)
{
	ST *q;

	printf("%4d %s\n", lfirst.num, lfirst.stroka);

	q = lfirst.next;
	while (q != NULL)
	{
		printf("%4d %s\n", q->num, q->stroka);
		q = (*q).next;
	}
}

/*********************************/
/* FREE_LIST
 * The function makes the memory free from
 * content of all pointers in the list
 */
/*********************************/
void free_list (void)
{
	ST *q, *lfirst;
	int i;
	for (i=0; i<HASHSIZE; i++)
		if (mas[i].next != NULL)
		{
			q = mas[i].next;
			lfirst = mas[i].next;

			while (q != NULL)
			{
				lfirst = (*lfirst).next;
				free(q);
				q = lfirst;
			}
		}
	//printf("Reserved memory is free now...\n");
}

/*********************************/
//          << MAIN >>           //
/*********************************/
int main (int arg_n, char *arg_c[]) {

	char *l_buffer, *word;
	int i, j;
	char c;

//	F1 = fopen("readf.txt", "r");
	F1 = stdin;
	if (F1 == NULL)
	{
		printf("Can't open the file");
		return -1;
	}

	for (i=0; i<HASHSIZE; i++)
	{
		mas[i].stroka=NULL;
		mas[i].num=0;
		mas[i].next=NULL;
	}

	while (1)
	{
		l_buffer = getline_unlim();
		if (l_buffer == NULL) break;  //If end of file then break

		word = NULL;
		word = (char *)realloc(word, strlen(l_buffer) + 1);  //getting memory for the word

		j= 0;
		for (i=0; i<strlen(l_buffer)+1; i++)  //Forming the words from string <l_buffer> into string <word>
		{
			c = l_buffer[i]; // (!!!) More effective algorithm of deleting non symbols??
			if  ( (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') ) word[j++] = l_buffer[i];
			else {					  // If we got a non symbol we form the word
					if (j>0)
					{
					word[j] = '\0';	  // That non symbol performs into '\0'
					fun_hashtable(word);  // Getting word in the hash table
					j=0;
					}
				}
		}
		free(l_buffer);

	}

	for (i=0; i<HASHSIZE; i++)
		if (mas[i].stroka != NULL)
			print_list(mas[i]);  //If the element is not blank we print its content

	free_list();
	fclose(F1);

	return 0;
}
