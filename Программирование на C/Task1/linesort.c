#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum {FGETS_BUFSIZE=5};

typedef struct node {
	char *stroka;
	struct node *next;
} ST;

FILE *F1;

/*********************************/
//           BUBBLE SORT         //
/*********************************/

void change(ST *prev, ST *cur, ST *nnext, ST *finish)
{
	ST *tmp_finish, *tmp_cur, *tmp_next;

	tmp_finish = finish;
	tmp_cur = cur;
	tmp_next = nnext;

	prev->next  = tmp_cur;
	cur->next   = tmp_next;

	if (nnext != NULL)	nnext->next = tmp_finish;

	return;
}

void bubble_sort (ST **lfirst)
{
	ST *q;
	ST *tmp2;
	int fl;

	if ((*lfirst) == NULL || (*lfirst)->next == NULL) return;

	do   // }  while (fl == 1);
	{
		q = (*lfirst);
		fl = 0;
		if (strcmp((*q).stroka, (*(*q).next).stroka) > 0)
		{
			fl = 1;
			tmp2 = q->next;
			if (q->next->next == NULL) change(q->next, q, q->next->next, NULL);
			else	change(q->next, q, q->next->next, q->next->next->next);
			(*lfirst) = tmp2;
			q = tmp2;
		}

		while (q->next->next != NULL)
		{
			if (strcmp(q->next->stroka, q->next->next->stroka) > 0)
			{
				fl = 1;
				change(q, q->next->next, q->next, q->next->next->next);
			}
			q = (*q).next;
		}
	}  while (fl == 1);

}

/*********************************/
//         OTHER FUNCTIONS       //
/*********************************/
void print_list (ST *lfirst, char *info)
{
	ST *q; int kol=0;

	//if (info[0]!='\0') printf("\n>  --- %s\n", info);

	//printf("\n");
	q = lfirst;
	while (q != NULL)
	{
		//printf("%d>  %s\n", ++kol, (*q).stroka);
		printf("%s\n", (*q).stroka);
		q = (*q).next;
	}

	//if (info[0]!='\0') printf("\n...   %s\n", info);

	//printf("\n");
	return;
}

/*********************************/
char *getline_unlim (void)
{
	char loc_buf[FGETS_BUFSIZE];
	int Fl_Error_Mem = 0;

	char *ptr = NULL; // for return
	size_t Str_size;
	int dl_buffer, fl_exit=0;
	int Strlen_Ptr = 0;

	char *ptr_new_memory; // sizeof( loc_buf[FGETS_BUFSIZE] )

	do
	{
		ptr_new_memory = fgets(loc_buf, sizeof(loc_buf), F1); // NULL => end of File or Error => read [FGETS_BUFSIZE-1] symbols
		if (ptr_new_memory == NULL && !feof (F1)) { printf("!!!!!!! fgets can't read File (0)"); fprintf(stderr, ".fgets can't read File... (0)\n"); exit(EXIT_FAILURE);}

		if (ptr_new_memory !=NULL)
		{
			dl_buffer = strlen(loc_buf);
			if (loc_buf[dl_buffer-1] == '\n')
				{
					loc_buf[dl_buffer-1] = '\0';
					fl_exit = 1;
				}
			if (ptr == NULL) 	{ Str_size = (strlen(loc_buf)+1) * sizeof(loc_buf[0]); Strlen_Ptr = 0;}
			else 			{ Str_size = (strlen(ptr) + strlen(loc_buf)+1) * sizeof(loc_buf[0]); Strlen_Ptr = strlen(ptr); }

			ptr = (char *) realloc(ptr, Str_size); //printf("(00)\n");
			ptr[Strlen_Ptr] = '\0';

			if (ptr == NULL) { printf("!!!!!!! not enough memory (00)"); fprintf(stderr, ".memory exhausted... (00)\n"); ++Fl_Error_Mem;}

			strncat (ptr, loc_buf, strlen(loc_buf)+1);   // append [FGETS_BUFSIZE-1] symbols to ptr[]

		}
		else fl_exit = 1;

		if (ptr == NULL && !feof (F1)) // only one symbol was in line and it was '\n'
		{
			Str_size = 2 * sizeof(loc_buf[0]);
			ptr = (char *) realloc(ptr, Str_size); //printf("(000)\n");
			if (ptr == NULL) { printf("!!!!!!! not enough memory (000)"); fprintf(stderr, ".memory exhausted... (000)\n"); ++Fl_Error_Mem;}
			ptr[0] = '\r';
			ptr[1] = '\0';
		}

	} while (fl_exit == 0);

	if (!feof (F1)) 	return ptr;
	else return NULL;


}

/*********************************/
//       << MERGE SORT >>	 //
/*********************************/
void append_spisok(ST *a, ST *b, ST **c)
{
	int ii=0;
	ST tmp;
	(*c) = NULL;

	if (a == NULL) {  (*c) = b; return;  }
	if (b == NULL) {  (*c) = a; return;  }

	if ( strcmp( a->stroka, b->stroka ) < 0 )
		{  (*c) = a;  a = a->next;  }
	else	{  (*c) = b;  b = b->next;  }

	tmp.next = (*c);
        ii++;

	while (a != NULL && b != NULL)
	{
		if ( strcmp( a->stroka, b->stroka) < 0 )
		{
			(*c)->next = a;
			a = a->next;
		} else {
			(*c)->next = b;
			b = b->next;
		}
		(*c) = (*c)->next;
	}

	while (a != NULL)
	{
		(*c)->next = a;
		(*c) = (*c)->next;
		a = a->next;

	}

	while (b != NULL)
	{
		(*c)->next = b;
		(*c) = (*c)->next;
		b = b->next;
	}

	(*c) = tmp.next;
}

/*********************************/
void divide_spisok (ST *tempo, ST **bot, ST **top)
{
	ST *double_middle = NULL;
	ST *middle = NULL;

	if (tempo == NULL || tempo->next == NULL) {
		(*bot) = tempo;
		(*top) = NULL;
		return;
	}

	middle = tempo;
	double_middle = tempo->next;

	while (double_middle != NULL) {
		double_middle = double_middle->next;
		if (double_middle != NULL) {
			double_middle = double_middle->next;
			middle = middle->next;
		}
	}

	(*bot) = tempo;
	(*top) = middle->next;
	middle->next = NULL;
}

/*********************************/
void free_list (ST *lfirst)
{
	ST *q;
	q = lfirst;
	while (q != NULL)
	{
		lfirst = (*lfirst).next;
		free(q);
		q = lfirst;
	}
	//printf("Reserved memory is free now...\n");
}

/*********************************/
void merge_sort(ST **head)
{
	ST *bot = NULL;
	ST *top = NULL;

	if ((*head == NULL) || ((*head)->next == NULL)) return;

	divide_spisok (*head, &bot, &top);
	merge_sort(&bot);
	merge_sort(&top);
	append_spisok(bot, top, head);

}

/*********************************/
//          << MAIN >>           //
/*********************************/
int main (int arg_n, char *arg_c[]) {


	size_t Str_size1;
	char *l_buffer;
	int kol = 0, Fl_Error_Memory = 0;

	ST *first = NULL, *pnext = NULL;  // pnext - current

	F1 = stdin;

	while (1) {
		l_buffer = getline_unlim();
		if (l_buffer == NULL) break;

		if (first == NULL)
		{
			Str_size1 = (strlen(l_buffer)+1)*sizeof(l_buffer[0]);

			first 			= (ST *)malloc(sizeof(ST)); //printf("(m0)\n");
			if (first == NULL) { printf("!!!!!!! not enough memory (m0)"); fprintf(stderr, ".memory exhausted... (m0)\n"); ++Fl_Error_Memory;}

			(*first).stroka = NULL;
			(*first).stroka = (char *)realloc((*first).stroka, Str_size1 ); //printf("(m1)\n");
			if ((*first).stroka == NULL) { printf("!!!!!!! not enough memory (m1)"); fprintf(stderr, ".memory exhausted... (m1)\n"); ++Fl_Error_Memory;}

			strncpy( (*first).stroka, l_buffer, strlen(l_buffer)+1);
			(*first).next 	= NULL;
		}
		else
		{
			Str_size1 = (strlen(l_buffer)+1)*sizeof(l_buffer[0]);

			pnext = (ST *)malloc(sizeof(ST)); //printf("(m2)\n");
			if (pnext == NULL) { printf("!!!!!!! not enough memory (m2)"); fprintf(stderr, ".memory exhausted... (m2)\n"); ++Fl_Error_Memory;}

			(*pnext).stroka = NULL;
			(*pnext).stroka = (char *)realloc((*pnext).stroka, Str_size1 ); //printf("(m3)\n");

			if ((*pnext).stroka == NULL) { printf("!!!!!!! not enough memory (m3)"); fprintf(stderr, ".memory exhausted... (m3)\n"); ++Fl_Error_Memory;}

			strncpy( (*pnext).stroka, l_buffer, strlen(l_buffer)+1);
			(*pnext).next = (*first).next;
			(*first).next = pnext;
		}

	}

	if      (strcmp(arg_c[1], "-m") == 0) {merge_sort(&first);  print_list(first, "File(s)'ve been sorted by MERGE method") ;}
	else if (strcmp(arg_c[1], "-b") == 0) {bubble_sort(&first); print_list(first, "File(s)'ve been sorted by BUBBLE method");}

	free_list(first);

	return 0;
}

