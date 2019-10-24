#include <stdio.h>
#include <unistd.h>

int main (int argc, char **argv)  //pr1 arg1 {; && ||} pr2 arg2 
{
	enum {SUCC=1, FAIL=0};
	int pid1, pid2, fl_pr1=SUCC, fl_pr2=SUCC, typee;
	int pid_tmp, status;

	if (argc < 5)
	{
		printf("Wrong input!!!\n");
		return 1;
	}
	if (argv[3][0] == ';') {  /*printf(";\n");*/  typee = 1;  }
	else if (argv[3][0] == '|' && argv[3][1] == '|') {  /*printf("||\n");*/  typee =  0;  }	
	else if (argv[3][0] == '&' && argv[3][1] == '&') {  /*printf("&&\n");*/  typee = -1;  }
	else printf("Wrong input!!!\n");

	if ((pid1 = fork())>0) 
	{
		//Parent
		pid_tmp = wait(&status);
		if (pid1 == pid_tmp && status > 0) fl_pr1 = FAIL;
//		printf("(1) (%d) status = %d\n", getpid(), status);
	}

	else
	{
		//Child1
		execlp(argv[1], argv[1], argv[2], (char *)0);
		_exit(1);
	}

//	printf("(1) (%d) fl_pr1 = %d;  fl_pr2 = %d\n", getpid(), fl_pr1, fl_pr2);


	if ( typee == -1 && fl_pr1 == 0)  // &&
	{
		printf("FAILURE\n");
		return 1;
	}
	if (typee == 0 && fl_pr1 == 1)  // ||
	{
		printf("SUCCESS\n");
		return 0;
	}

	if ((pid2 = fork())>0) 
	{
		//Parent
		pid_tmp = wait(&status);
		if (pid2 == pid_tmp && status > 0) fl_pr2 = FAIL;
//		printf("(2) (%d) status = %d\n", getpid(), status);
	}

	else
	{
		//Child2
		execlp(argv[4], argv[4], argv[5], (char *)0);
		_exit(1);
	}

//	printf("(2) (%d) fl_pr1 = %d;  fl_pr2 = %d\n", getpid(), fl_pr1, fl_pr2);
	
	if ( typee == -1 && fl_pr2 == 0)  // &&
	{
		printf("FAILURE\n");
		return 1;
	}
	if (typee == 0 && fl_pr2 == 1)    // ||
	{
		printf("SUCCESS\n");
		return 0;
	}
	if (fl_pr2 == 1) 		  // ;
	{ 
		printf("SUCCESS\n");
		return 0;
	}
	
	printf("FAILURE\n");
	return 1;

}

