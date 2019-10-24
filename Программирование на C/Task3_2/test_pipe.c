#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>

#define T 0
#define PS_MAX 20

typedef struct ps {
		char *str_ps;
		struct node *next;
	} ps;

char *str_ps[PS_MAX], *str_ps_new[PS_MAX];
int N_str_ps, N_str_ps_new;

/*********************************/
int test_shell_pipe_zomby(char *tmp_str_ps[], int flag_zombie_only)
{
	int fd[2], fd_tmp, pid, status, i, k = 0, cnt = 0;
	int kol=0;
	char tmppatt[] = "/tmp/XXXXXX"; 
	char tmp_str[256];
	char Str_info[4096];

	fd_tmp = mkstemp(tmppatt);	
	unlink(tmppatt);
	dup2(0, fd_tmp); 

	pipe(fd);
	
	if (fork() == 0) 
		{
			
		dup2(fd[1], 1);
		close(fd[1]);	 close(fd[0]);
		execlp("ps", "ps", "-o", "stat,ppid,pid,cmd,stime", (char *)0);

		}

	dup2(fd[0], 0);	
	close(fd[0]); close(fd[1]);
	
	while( (kol = read(0, &Str_info, sizeof(Str_info)) ) != -1 )	
		{ //printf("0) kol = %d >>> %lu >>>> %s >>>>>>> \n", kol, sizeof(Str_info), Str_info );
		  fcntl(0, F_SETFL, O_NONBLOCK);
		}
	
	for (i = 0; i < strlen(Str_info); i++) 
	{
		if (Str_info[i] != '\n' /*&& i < strlen(Str_info) - 1*/) tmp_str[k++] = Str_info[i];
		else 
		{
			tmp_str[k] = '\0';
			k = 0;
			if (tmp_str[0] == 'Z' && flag_zombie_only || !flag_zombie_only) 
			{
				tmp_str_ps[cnt] = (char *)malloc( (strlen(tmp_str) + 1) * sizeof(char) );
				strcpy(tmp_str_ps[cnt], tmp_str);
				cnt++;
			}
		}
	}

	while ( (pid = wait(&status)) !=-1); //printf("[%d]", pid);
	
	dup2(0, fd_tmp);

	return cnt;	
}

/*********************************/
void exec_shell(char *var)
{
	int fd[2], fd1[2], fd_tmp;
	int pid, kol=0, status;
	char Str_info[4096];
	char tmppatt[] = "/tmp/XXXXXX"; 

	fd_tmp = mkstemp(tmppatt);	
	unlink(tmppatt);
	dup2(0, fd_tmp); 

	pipe(fd); 

	if (fork() == 0) 
	{
		dup2(fd[0], 0);
		
		close(fd[1]);	 close(fd[0]); 

		execlp("./ppp", "ppp", (char *)0);
		
		exit(1);
	}
	dup2(fd[1], 1);		

	close(fd[0]); close(fd[1]);

	if (strcmp(var, "1") == 0)
	{

		write(1, "cat ttt\n", strlen("cat ttt\n"));	

		write(1, "cat ttt | head -n 2\n", strlen("cat ttt | head -n 2\n"));

		write(1, "abracadabra\n", strlen("abracadabra\n"));

		write(1, "head -n 5 ttt | head -n 2\n", strlen("head -n 5 ttt | head -n 2\n"));	

		write(1, "cat ttt | head -n 6 | head -n 5 | head -n 4 | head -n 3\n", strlen("cat ttt | head -n 6 | head -n 5 | head -n 4 | head -n 3\n"));

		write(1, "sleep 1 | ls\n", strlen("sleep 1 | ls\n"));

		write(1, "cat ttt | head -n 5 | sort -r\n", strlen("cat ttt | head -n 5 | sort -r\n"));

		write(1, "sdaf | asdf | ls\n", strlen("sdaf | asdf | ls\n"));

		write(1, "ls | cat ttt\n", strlen("ls | cat ttt\n"));

		write(1, "echo abc vvv 111111\n", strlen("echo abc vvv 111111\n"));

		//write(1, "sleep 3 | sleep 5 | sleep 4\n", strlen("sleep 3 | sleep 5 | sleep 4\n"));
		fprintf(stderr, "\n======= Test of collecting zombies & Test of parallel programmes in conveyor\n");

		write(1, "ps\n", strlen("ps\n"));

		write(1, "sleep 2 &\n", strlen("sleep 2 &\n"));

		write(1, "ps\n", strlen("ps\n"));
	}
	if (strcmp(var, "2") == 0)  write(1, "sleep 3 | sleep 5 | sleep 4\n", strlen("sleep 3 | sleep 5 | sleep 4\n"));

	write(1, "\n", strlen("\n"));	

	while ( (pid = wait(&status)) !=-1);

	dup2(0, fd_tmp);

}

#define tt 1

/*********************************/
int main(int argc, char **argv)
{
	time_t t1, t2;
	enum {N = 11}; // {N = 11};
	int pid1, pid_tmp, status, i, ii;
	int arr[N];

	if (argc == 1) 
	{
		if (tt) printf("\n====================================================================================\n");
		exec_shell("1");
		return 0;
	}
	
	if (tt) printf("\n====================================================================================\n");
	if (tt) printf("\n=== Test of collecting zombies & Test of parallel programmes in conveyor ===========\n");
	// sleep 3 | sleep 5 | sleep 4
	//*************************************** # 9 == Test of collecting zombies & Test of parallel programmes in conveyor
	N_str_ps     = test_shell_pipe_zomby(str_ps, 1);
	 
	t1 = time(NULL);
					
	exec_shell("2");
	t2 = time(NULL) - t1;
	fprintf(stderr, "\ntime of process => %f\n", (double)t2);
	 
	N_str_ps_new = test_shell_pipe_zomby(str_ps_new, 1);

	if (N_str_ps > 0) 
	{
		fprintf(stderr, "\nZombie table before running test  \n");
		for (ii = 0; ii < N_str_ps; ii++) fprintf(stderr, "%d> %s\n", ii, str_ps[ii]);
	}
	else fprintf(stderr, "\nThere were no zombies before running this test \n");

	if (N_str_ps > 0) 
	{
		fprintf(stderr, "\nZombie table after running test  \n");
		for (ii = 0; ii < N_str_ps_new; ii++) fprintf(stderr, "%d> %s\n", ii, str_ps_new[ii]);
	}
	else fprintf(stderr, "\nThere were no zombies after running test  \n");

	return 0;
}	

