#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int test_creat(char *_shel, char *path, char *pr1, char *arg1, char *op, char *pr2, char *arg2, char *res, int k)
{	int pid1, status, pid_tmp;
	int what_we_got, what_we_want;
	

	if ((pid1 = fork())>0) 
	{
		//Parent
		pid_tmp = wait(&status);
		
		if (pid1 == pid_tmp && status > 0) what_we_got  = 0; else  what_we_got  = 1;
		if (strcmp(res, "FAILURE")==0)     what_we_want = 0; else  what_we_want = 1;		

		if (what_we_want == what_we_got)
		{
			printf("Test #%d has been passed\n\n", k);
			return 1;
		}
		else 
		{
			printf("Test #%d hasn't been passed\n", k);
			printf("Should be -> %s\n\n", res);
			return 0;
		} 
	}

	else
	{
		//Child
		execlp(_shel, path, pr1, arg1, op, pr2, arg2, (char *)0);
		_exit(1);
	}
}

int main(void)
{
	
	
	enum {N = 15};
	int pid1, pid_tmp, status, i;
	int arr[N];

	for (i=0; i<N; i++) arr[i]=0;

	i=0;
//	&&	
	arr[i++] = test_creat("./shell_andor", "true", "true", "-", "&&", "echo", "&& works correct", "SUCCESS", i);  // 1 && 1
	arr[i++] = test_creat("./shell_andor", "false", "false", "-", "&&", "echo", "&& works correct", "FAILURE", i);// 0 && 1
	arr[i++] = test_creat("./shell_andor", "cat", "cat", "nonexistingfile", "&&", "echo", "&& doesn't work correct", "FAILURE", i);// 0 && 1
	arr[i++] = test_creat("./shell_andor", "cat", "cat", "test.c", "&&", "sleep", "0.05", "SUCCESS", i);       // 1 && 1
	arr[i++] = test_creat("./shell_andor", "cat", "cat", "shell_andor.c", "&&", "sleep2", "0.05", "FAILURE", i);      // 1 && 0

//	||
	arr[i++] = test_creat("./shell_andor", "true", "true", "-", "||", "echo", "Doesn't work correct", "SUCCESS", i);// 1 || 1
	arr[i++] = test_creat("./shell_andor", "ls", "ls", "-a", "||", "echo", "abc", "SUCCESS", i);			// 1 || 1
	arr[i++] = test_creat("./shell_andor", "false", "false", "", "||", "echo", "abc", "SUCCESS", i);		// 0 || 1
	arr[i++] = test_creat("./shell_andor", "ls1", "ls1", "-a", "||", "echo2", "abc", "FAILURE", i);			// 0 || 0

//	;
	arr[i++] = test_creat("./shell_andor", "sleep", "sleep", "0.03", ";", "sleep", "0.02", "SUCCESS", i); 	// 1 ; 1
	arr[i++] = test_creat("./shell_andor", "false", "false", "", ";", "sleep", "0.02", "SUCCESS", i);  	// 0 ; 1
	arr[i++] = test_creat("./shell_andor", "true", "true", "", ";", "sledsfep", "0.02", "FAILURE", i);  	// 1 ; 0
	arr[i++] = test_creat("./shell_andor", "false", "false", "", ";", "sledsfep", "0.02", "FAILURE", i);  	// 0 ; 0	

//	Laziness

	time_t t1, t2;
	
//	&&
	t1 = time(NULL);
	arr[i++] = test_creat("./shell_andor", "sleep", "sleep1", "--", "&&", "sleep", "5", "FAILURE", i); 	// 0 && 1
	t2 = time(NULL) - t1;
	if (t2 != 0) arr[i - 1] = 0; 

//	||
	t1 = time(NULL);
	arr[i++] = test_creat("./shell_andor", "sleep", "sleep", "5", "||", "sleep", "5", "SUCCESS", i); 	// 1 || 1
	t2 = time(NULL) - t1;
	if (t2 != 5) arr[i - 1] = 0;
	

	int count = 0;
	for (i=0; i<N; i++) count += arr[i];

	printf("\nNumber of tests     => %d\n", N);
	printf("Successfully passed => %d\n\n", count );	

	return 0;
}
