#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/**************************************************/
char *getline_unlim (void)
{
	enum {FGETS_BUFSIZE = 5};
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

