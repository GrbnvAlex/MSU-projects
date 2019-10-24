#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "util.h"

FILE *F1;


int main (int arg_n, char *arg_c[]) {
	
	char c;
	int i;
	char *l_buffer;
	int Flag_Low=0;

	//F1 = fopen ("text.txt", "r");  if (F1 == NULL) {printf("Can't open the fyle\n"); return -1;}
	F1 = stdin;
	if (arg_n > 1) 
		if (strcmp(arg_c[1], "-l") == 0) Flag_Low=1;

	while (1) {
		l_buffer = getline_unlim();
		if (l_buffer == NULL) break;
		for (i=0; i<strlen(l_buffer); i++) 
		{			
			c = l_buffer[i];
			if (!((c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9')))
				l_buffer[i]='\n';
			else if (Flag_Low) 
				l_buffer[i] = tolower(l_buffer[i]);
		}	
		printf("%s\n", l_buffer);
		free(l_buffer);

	}

	//if (fclose (F1) == EOF) printf("Can't close the fyle\n");

	return 0;
}

