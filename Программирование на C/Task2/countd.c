#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "util.h"

FILE *F1;


int main (void) {
	
	char c;
	int i, kol=0;
	char *l_buffer, *prev=NULL;

	//F1 = fopen ("text.txt", "r");  if (F1 == NULL) {printf("Can't open the fyle\n"); return -1;}
	F1 = stdin;

	while (1) 
	{
		l_buffer = getline_unlim();
		
		if (l_buffer == NULL) break;
		c = l_buffer[0];
		if (c == '\n' || c == '\r') continue;
		if ((c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9'))
		{
			if (prev == NULL) 
			{
				prev = (char *)realloc(prev, strlen(l_buffer)+1);			
				strncpy(prev, l_buffer, strlen(l_buffer)+1);
				kol++;
			}
			else if (strcmp(prev, l_buffer) == 0) kol++;
				else
				{
					printf("%4d %s\n", kol, prev);
					prev = (char *)realloc(prev, strlen(l_buffer)+1);			
					strncpy(prev, l_buffer, strlen(l_buffer)+1);
					kol=1;
				}	

		}
		free(l_buffer);

	}  // while

	printf("%4d %s\n", kol, prev);

	//if (fclose (F1) == EOF) printf("Can't close the fyle\n");

	return 0;
}

