#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>

#define T 1

int shm_id = -1;
int sem_id = -1;
char *shm_adress;
struct sembuf sem;


/*********************************/
char *curtime(void)
{
	char *ptr;
	
	time_t timer = time(NULL);
	struct tm *t = localtime(&timer);
	ptr = (char *) malloc(8+1);
	sprintf(ptr, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec );
	
	return ptr;
}

/*********************************/
void say_you(char inf[], int flRule)
{
	char str[256], str1[256];
	int op = sem.sem_op;
	int num = sem.sem_num;
	int val = semctl(sem_id, sem.sem_num, GETVAL, (int) 0);
	int val0 = semctl(sem_id, 0, GETVAL, (int) 0);
	int val1 = semctl(sem_id, 1, GETVAL, (int) 0);
	
	if (op == 0 && val == 0) 	{ sprintf(str, "[_ op[%d] == 0 && val[%d] == 0 _] NEW val[%d] = 0, Not Blocked [%d]", num, num, num, num); 
					  sprintf(str1, "NOT BLOCKED [%d]", num);
                    			}	
	if (op == 0 && val != 0) 	{ sprintf(str, "[_ op[%d] == 0 && val[%d] <> 0) _] NEW val[%d] <> 0, Blocked [%d] while val !=0\n--------\n\n\n", num, num, num, num);
					  sprintf(str1, "BLOCKED [%d]", num);
         				}	
	if (op != 0 && val+op >= 0) 	{ sprintf(str, "[_ op[%d] <> 0 && (val+op)[%d] >= 0 _] NEW val[%d] >= 0, Not Blocked [%d]", num, num, num, num);
					  sprintf(str1, "NOT BLOCKED [%d]", num);
 					}
	if (op != 0 && val+op < 0 ) 	{ sprintf(str, "[_ op[%d] <> 0 && (val+op)[%d] < 0 _] NEW val[%d] < 0, Blocked [%d] while val+op < 0\n--------\n\n\n", num, num, num, num);
					  sprintf(str1, "BLOCKED [%d]", num);
					}
	
	if (sem.sem_num == 0) val0 += sem.sem_op;
	if (sem.sem_num == 1) val1 += sem.sem_op;

	printf("[ %s ] (%s)   val[0]=%d     val[1]=%d     %s\n", curtime(), inf, val0, val1, str1);
	if (flRule) 
	{
		printf("[ %s ] (%s)                             op[%d] = %d,  old val[%d] = %d,  NEW val[%d] = %d\n", curtime(), inf, num, op, num, val, num, val+op);
		printf("[ %s ] (%s)                             %s\n", curtime(), inf, str);
	}

}
/*********************************/
void cleanup(void)
{
	if (shm_adress != (void*) -1) shmdt(shm_adress);
	if (sem_id >= 0) semctl(sem_id, 0, IPC_RMID);
	if (shm_id >= 0) semctl(shm_id, IPC_RMID, 0);
}

/*********************************/
void pexit(char const *str) 
{
	perror(str);
	cleanup();
	exit(1);
}

/*********************************/
#define BUF_SHM 20 // for Writer too
#define FTOK_FILE "file"  // used for ipc_key -don't delete it
#define FTOK_PROJ 0

#define tt 0
#define tP 0  // for sleep
#define _flRule 0

#define MAX_FILE_NAME_LENGTH 10  // < BUF_SHM 15
#define MAX_FILE_LENGTH 10  // < BUF_SHM 15

/*********************************/
int main(int argc, char **argv)
{	
	int cnt=0;

	key_t ipc_key;
	int fd, read_cnt, i, dl, ii;
	char buf[BUF_SHM + 1], *buf1, *buf2;
	long File_length;
	char Str_FileName_length[MAX_FILE_NAME_LENGTH];
	char Str_File_length[MAX_FILE_LENGTH];
	char Str_Format[10];
	char sss[5];
	char *FileNameBuffer;
	int FlagSentHeader, SeekCur, MaxSeekCur;
	int CUR_BUF_SHM;	

	//int argc = 4;
	//char *argv[] = {"./rrr", "aaa1234567890a123456 aa", "fff", "fff1"};

	//int argc = 2;
	//char *argv[] = {"./rrr", "fff1"};

	if (T) printf("\n=========================================================================\n");

	if (tt) printf("Argc = %d\n", argc);
	if (tt) for (i = 0; i < argc; i++) printf("Argv[%d] = %s\n",i, argv[i]);
	if (tt) printf("\n");

	FileNameBuffer = NULL;
	buf1 = NULL;
	buf2 = NULL;

	atexit(cleanup);

	ipc_key = ftok(FTOK_FILE, FTOK_PROJ);

	if ((shm_id = shmget(ipc_key, BUF_SHM, IPC_CREAT | 0666)) < 0) pexit("shmget"); // Creating share memory (reader)  - [BUF_SHM]
	shm_adress = shmat(shm_id, NULL, 0);
	if (shm_adress == (void*) -1) pexit("shmat");
	strncpy(shm_adress, "   \0", 4);

	sem_id = semget(ipc_key, 2, IPC_CREAT | 0666);    // Creating semaphores

	//--------------------------//
	for (i = 1; i < argc; i++) 
	{
		if (T) printf("\n-------------------------------------------------------------------------\n");
		if ( (fd = open(argv[i], O_RDWR, 0666) <0) )
		{
			printf("Error occurred with File <%s>\n", argv[i]);
			continue;
		}
		File_length = (long)lseek(fd, 0L, SEEK_END);
		if (File_length <= 0)
		{	
			close(fd);
			fd = open(argv[i], O_RDWR, 0666);
			File_length = (int)lseek(fd, 0L, SEEK_END);
		}
		lseek(fd, 0L, SEEK_SET);

		sprintf(sss, "%d", (long)strlen(argv[i]) );
		snprintf(Str_Format, sizeof(Str_Format), "%s%d%s", "%-", MAX_FILE_NAME_LENGTH, "s"); // snprintf include str[Max-1]+'\0'
		snprintf(Str_FileName_length, sizeof(Str_FileName_length), Str_Format, sss);
		
		sprintf(sss, "%d", (int)File_length );
		snprintf(Str_Format, sizeof(Str_Format), "%s%d%s", "%-", MAX_FILE_LENGTH, "s");
		snprintf(Str_File_length, sizeof(Str_File_length), Str_Format, sss);

		if (T) printf("(%d) <%s> <%s> <%s> <content> \n", i, Str_FileName_length,  argv[i], Str_File_length);
		if (tt) printf("-----------------\n");
	
		dl = (int)( sizeof(Str_FileName_length) + strlen(argv[i]) + sizeof(Str_File_length) ) + 1 + 1 + 2; // for blanks
		FileNameBuffer = (char *)malloc(dl * sizeof(char)); 
		
		snprintf(FileNameBuffer, dl * sizeof(char), "%s %s %s ", Str_FileName_length, argv[i], Str_File_length);  //snprintf by itself makes FileNameBuffer[dl] = '\0';
		if (tt) printf("dl = %d = %d + %d + %d + 1 = %d = %d\n", dl, (int)sizeof(Str_FileName_length), 
                             (int)strlen(argv[i]), (int)sizeof(Str_File_length), (int)(dl * sizeof(char)), (int)(strlen(FileNameBuffer)) );
		
		FlagSentHeader 	= 0;
		SeekCur 	= 0;
		MaxSeekCur	= (int)(strlen(FileNameBuffer));

		if (tt) printf("\n%s, dl = %d\n", FileNameBuffer, MaxSeekCur);
		if (T) printf("-----------------\n");

		semctl(sem_id, 0, SETVAL, (int) 1);	// val[0] = 1
		semctl(sem_id, 1, SETVAL, (int) -1);	// val[1] = 0

		sem.sem_flg = 0;

		if (tt) printf("[ %s ] START val[%d] = %d     val[%d] = %d\n\n",  curtime(), 0, semctl(sem_id, 0, GETVAL, (int) 0), 1, semctl(sem_id, 1, GETVAL, (int) 0) );

		do
		{
			if (tt) printf(":(111):: %s\n", shm_adress);
			sem.sem_num = 0;
			sem.sem_op  = -1;  

			if (tt) say_you("1", _flRule);  // what will be in semop() 
			semop(sem_id, &sem, 1);  		
																
			if (tt) printf("[ %s ]      Not Blocked [%d]            NEW val[%d] = %d     \n",  curtime(), sem.sem_num, sem.sem_num, semctl(sem_id, sem.sem_num, GETVAL, (int) 0) );  

			if (FlagSentHeader == 0)
			{
				read_cnt = BUF_SHM;
				 
				for(ii=0; ii < BUF_SHM && SeekCur < MaxSeekCur; ii++)
					buf[ii] = FileNameBuffer[SeekCur++];
				if (ii == BUF_SHM)	
				{
					buf[BUF_SHM] = '\0';
					if (T) printf("%s\n", buf);
					strncpy(shm_adress, buf, BUF_SHM + 1);	 // writing into the share memory...
				}
				else
				{
					buf[ii] = '\0';
					CUR_BUF_SHM = BUF_SHM - (int)strlen(buf);
					buf1 = (char *)malloc( (int)strlen(buf) * sizeof(char));
					strcpy(buf1, buf);
					if (T) printf("(1 part) %s\n", buf1);
					FlagSentHeader = 1;
				}
				 
			}
			if (FlagSentHeader == 1)
			{
				if (tt) printf("\n... reading from the file new info-block......\n");
				
				if ((read_cnt = read(fd, &buf, CUR_BUF_SHM) ) != 0) //reading from the file new info-block...
				{
					buf[read_cnt] = '\0';
					if (CUR_BUF_SHM < BUF_SHM)
					{
						buf2 = (char *)malloc( (int)strlen(buf) * sizeof(char));
						strcpy(buf2, buf);
						strcpy(buf, buf1);
						strcat(buf, buf2);
						read_cnt = (int)strlen(buf);	
					}
					buf[read_cnt] = '\0';
					if (T) printf("%s", buf);
					strncpy(shm_adress, buf, read_cnt+1);	// writing into the share memory...
					CUR_BUF_SHM = BUF_SHM;
				}
				else
				{	
					strncpy(shm_adress, "\0", 1);
					FlagSentHeader = 0;
					SeekCur = 0;	
				}
			}
			buf[0] = '\0';
					
			if (tt) printf("\n.................................................... sleep  ...\n");
			sleep(tP);
	
			sem.sem_num = 1;
			sem.sem_op  = 1;  

			if (tt) say_you("2", _flRule);  // what will be in semop() 
			semop(sem_id, &sem, 1);  // semop(ID, &struct, for [1] sem.op -operation in massiv) 

			if (tt) printf("[ %s ]      Not Blocked [%d]            NEW val[%d] = %d     \n",  curtime(), sem.sem_num, sem.sem_num, semctl(sem_id, sem.sem_num, GETVAL, (int) 0) ); 
			if (tt) printf("\n.................................................... sleep  ...\n");
			sleep(tP);

			if (tt) printf(":(222):: %s\n", shm_adress);

		}
		while(read_cnt != 0);	
	
		if (T) printf("\n-------------------------------------------------------------------------\n");
		if (tt) printf("[ %s ] OVER val[%d] = %d     val[%d] = %d\n",  curtime(), 0, semctl(sem_id, 0, GETVAL, (int) 0), 1, semctl(sem_id, 1, GETVAL, (int) 0) );

		close(fd);
		if (FileNameBuffer != NULL) free(FileNameBuffer);
		if (buf1 != NULL) free(buf1);
		if (buf2 != NULL) free(buf2);	

	} //for (i = 1; i < argc; i++)

	printf("Done. \n");

	strncpy(shm_adress, "###\0", 4);	

	return 0;

}


