#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#define T 0
#define tt 1

typedef struct node {
		char *pri;
		int pid;
		int finish;
		char **argv;
		int status;
		struct node *next;
	} ST;

ST *first = NULL, *pnext = NULL;

int fon_ps = 0;


/*********************************/
char *getline_unlim (void)
{
	FILE *F1 = stdin;	
	enum {FGETS_BUFSIZE = 2};
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
	if (T) printf("Reserved memory is free now...\n");
}

/*********************************/
void print_list (ST *lfirst, char *info)
{
	ST *q; int kol=0, ii, fll = 1;

	if (info[0]!='\0') printf("\n[>  --- %s\n", info);
	else fll = 0;

	printf("\n");
	q = lfirst;
	while (q != NULL)
	{		
		printf("pri = %d>  %s\n", kol++, q->pri);
		if (fll) printf("pid = %d\n",  q->pid);
		if (fll)printf("status = %d\n",  q->status);
		if (fll)printf("finish = %d\n",  q->finish);
		ii = 0;
		while (q->argv[ii] != NULL) 
		{
			printf("argv[%d]>  %s\n", ii, q->argv[ii]); 
			ii++;
		}
		q = (*q).next;
	}

	if (info[0]!='\0') printf("\n]>  ---   %s\n", info);

	printf("\n");
	return;
}

/*********************************/
char **str_split(int *argc)
{
	char *str, *str_pointer;
	char **argv;
	int argc1 = 0, argc2 = 1, end_of_word, char_num, i, j;
	int length, flag = 1;

	str = getline_unlim();
	if (str == NULL) {
		*argc = 0;
		return NULL;
	}

	if (tt) printf("------------------------------------------\n");	
	if (tt) printf("> stdin:  %s\n", str);
	length = strlen(str);

	if (length == 1 && (str[0] == '\r' || str[0] == '\n') || length == 0) {
		*argc = 0;
		return NULL;
	}
	
	for (i=0; i<length + 1;i++) 
		if (str[i] == ' ' || str[i] == '\0') {
			if (flag) argc1++;
			flag = 0;
		}
		else flag = 1;

	argv = (char **)malloc( (argc1+1) * sizeof(char *) );

	char word[length];

	flag = 1;
	j = 0;
	for (i=0; i<length + 1; i++) 
		if (str[i] == ' ' || str[i] == '\0') {
			if (flag) {
				word[j] = '\0';
				argv[argc2] = (char*)malloc((strlen(word) + 1) * sizeof(char)); 
				strcpy(argv[argc2], word);
				argc2++;
				j = 0;
			}
			flag = 0;
		}
		else {
			flag = 1;
			word[j++] = str[i];
		}
	
	argv[0] = (char*)malloc((strlen(argv[1]) + 1) * sizeof(char)); 
	strcpy(argv[0], argv[1]);

	*argc = argc1 + 1;

	return argv;
}

#define tt 1

/*********************************/
void exec_conveyor(ST *first, int pr_num, int flag_fon)
{
	int pid, i, status, last_status, p, ii;
	char Str[256];
	int fd[pr_num][2];
	ST *tmp;

	printf("\n");
	
	tmp = first;
	for (i = 0; i < pr_num; i++) 
	{
		if (i < pr_num - 1) pipe(fd[i]);

		if ((pid = fork()) < 0) { perror("fork"); exit(1); }

		tmp->pid = pid;
  
		if (pid == 0)  // Child
		{
			if (i < pr_num - 1) close(fd[i][0]); 
			
			if (i > 0) 
			{
				dup2(fd[i - 1][0], 0);     // for reading: we took stream N 0 for OUR using = for PIPE
				close(fd[i - 1][0]);			
			}
			if (i < pr_num - 1) 
			{
				dup2(fd[i][1], 1);
				close(fd[i][1]);				
			}
			
			execvp(tmp->pri, tmp->argv);
			perror("exec");
			exit(1);

		}  //if (pid == 0)  // Child

		if (flag_fon == 1) { ++fon_ps; fprintf(stderr, "[%d]   %d\n", fon_ps, pid); }

		if (i > 0) 	close(fd[i - 1][0]);
		
		if (i < pr_num - 1) close(fd[i][1]);
	
		tmp = tmp->next;

	}  // for (i = 0; i < pr_num; i++)
	
	if (flag_fon == 0)
	{
		do { 		
			pid = wait(&status);
			 
			tmp = first;
			while (tmp != NULL)
			{
				if (pid == tmp->pid) 
				{
					tmp->finish = -1;
					tmp->status = status;
					if (!status) printf("\nProgram name = %s, status = %d => finished ok\n", tmp->pri, status);
					else printf("\nProgram name = %s, status = %d => finished with error\n", tmp->pri, status);
					break;
				}
				tmp = tmp->next;
			}
		} while (pid != -1);

		tmp = first;
		p = 1;
		while (tmp != NULL)
		{
			p *= tmp->finish;
			last_status = tmp->status;
			tmp = tmp->next;
		}

		if (p != 0) 
		{
			print_list(first, "");

			tmp = first;
			ii = 0;		
			while (tmp != NULL)
			{
				i = 0;
				if (ii++) printf("| ");		
				while (tmp->argv[i] != NULL) 
				{
					printf("%s ", tmp->argv[i]); 
					i++;
				}	
				tmp = tmp->next;
				if (flag_fon) printf("& ");
			}
			printf("\n\n");
			printf("WIFEXITED(status) = %d\n", WIFEXITED(status)); //if exited successfully => not 0
			printf("Status of the conveyor => %d.\n", last_status);

		}

	} // if (flag_fon == 0)


/*	    if ((pid = fork()) < 0) { perror("fork"); exit(1); }
		if (pid == 0)  // Child
		{
			sprintf(Str, "%d\n", last_status);
	    	//write(1, Str, (strlen(Str)+1) * sizeof(char) );
			execlp("echo", "echo", Str, (char *)0);
		}
		pid = wait(&status);
*/

}



/*********************************/
int main(int _argc, char **_argv)
{
	int i, j, n1, n2, npr, flag=0;
	int N; 		//number of arguments for each process
	int pr_num = 0;	//number of processes
	ST *first = NULL, *pnext = NULL, *tmp = NULL;
	int flag_fon;

	int argc1;
	char **argv;
	int argc = 0;

	while ((argv = str_split(&argc)) != NULL)
	{
		flag_fon = 0;
		if (strcmp(argv[argc - 1], "&") == 0) flag_fon++;

		argc1 = argc - flag_fon;
		pr_num = 0;
		flag=0;

		first = NULL; pnext = NULL;
		printf("------------------------------------------\n");
		n1 = 1;
		for (i = 1; i < argc1; i++)
		{
			flag = 0;	     
			if (argv[i][0]=='|') 
			{ 	npr = n1; n2=i-1; N = n2-n1; n1 = i+1;        
				flag = 1;
			} 
			if (i==argc1 - 1)    
			{	npr = n1; N = i-n1;        
				flag = 1;
			}
			if (flag)        
			{                                   
				pr_num++;
				if (first == NULL)
				{
					first = (ST *)malloc(sizeof(ST));                           
					
					first->argv = (char **)malloc( (N+2) * sizeof(char *) );    
					first->finish = 0;
					first->pri = argv[npr];
					first->argv[0] = argv[npr];                                 
					for (j = 1; j < N + 1; j++) first->argv[j] = argv[npr + j];
					first->argv[N + 1] = (char *)0;
					first->next = NULL;

					if (T) printf("PRI> %s\n", first->pri);
					if (T) for (j = 0; j < N + 2; j++) printf("ARGV111[%d]> %s\n", j, first->argv[j]);

				}
				else
				{
					pnext = (ST *)malloc(sizeof(ST));                                
					pnext->argv = (char **)malloc( (N+2) * sizeof(char *) );          
					pnext->finish = 0;
					pnext->pri = argv[npr];
					pnext->argv[0] = argv[npr];
					for (j = 1; j < N + 1; j++) pnext->argv[j] = argv[npr + j];         
					pnext->argv[N + 1] = (char *)0;
					pnext->next = NULL;

					if (T) printf("PRI> %s\n", pnext->pri);
					if (T) for (j = 0; j < N + 2; j++) printf("ARGV222[%d]> %s\n", j, pnext->argv[j]);
				
					tmp = first;    

					while (tmp->next != NULL)
						tmp = tmp->next; 

					tmp->next = pnext;   
				}

			}  // if (flag) 
				 
		} //for (i = 1; i < argc1; i++)

		exec_conveyor(first, pr_num, flag_fon);

		if (T) print_list(first, "********");

		free_list(first);
		fflush(stdin);

	} // while ((argv = str_split(&argc)) != NULL)

	return 0;
}














