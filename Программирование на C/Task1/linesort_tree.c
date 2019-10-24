#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum {FGETS_BUFSIZE = 5};

FILE *F1;
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
struct tree {
	char *line;
	struct tree *left;
	struct tree *right;
};

/*********************************/
int kol = 0;

/*********************************/
void treeprint (struct tree *prin)
{

	if (prin != NULL) {
		treeprint (prin->left);
		//printf ("%d> %s\n", ++kol, prin->line);
		printf ("%s\n", prin->line);
		treeprint(prin->right);
	}
}
/*********************************/
struct tree *node_add (struct tree *p, char *w)
{
	char *q;
	
	if (p == NULL)
	{
		p = (struct tree *) malloc (sizeof (struct tree));
		q = (char *)malloc(strlen(w) + 1);
	        if (q != NULL) strcpy(q, w);


		p->line = q;
		p->left = p->right = NULL;
	} else if (strcmp ( p->line, w ) > 0)
		p->left  = node_add( p->left,  w);
		else    p->right = node_add( p->right, w);


	return p;
}

/*********************************/
 void free_tree (struct tree *p)
{
		if (p != NULL)
		{
			free_tree(p->left);
			free_tree(p->right);
			free(p);
		}
	return;
}

 /*********************************/
int main (void)
{
	struct tree *root;
	char *str;

	F1 = stdin;
	root = NULL;
	while ((str = getline_unlim()) != NULL)  root = node_add (root, str);
	treeprint (root);
	free_tree (root);
//	printf("\nMemory is succesfully cleared\n\n");
	return 0;
}
