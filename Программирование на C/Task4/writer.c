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
	exit(1);
}

/*********************************/

#define BUF_SHM 20  // for Reader too
#define FTOK_FILE "file" // used for ipc_key
#define FTOK_PROJ 0
#define FILE_TO_WRITE "received"

#define tt 0
#define tP 0  // for sleep
#define _flRule 0

#define MAX_FILE_NAME_LENGTH 10  // < BUF_SHM 15 // snprintf include str[Max-1]+'\0'
#define MAX_FILE_LENGTH 10  // < BUF_SHM 15 // snprintf include str[Max-1]+'\0'

/*********************************/
int main(int argc, char **argv)
{
	int FlagSentHeader, i, Strlen_F_Buf, Str_size, ii;
	int Flag_F_Name_length_Received;
	int Flag_F_Name_Received;
	int Flag_F_length_Received;
	int Flag_You_can_save;	

	char Str_FileName_length[MAX_FILE_NAME_LENGTH];
	char Str_File_length[MAX_FILE_LENGTH];
	int FileName_length, File_length;
	int Header_Size;	
	int CUR_File_Recieved;	

	key_t ipc_key;
	int fd, read_cnt, fl_finish = 0;
	
	char *FileNameBuffer;
	char *FileName;
	char *buf;

	buf = (char *)malloc((BUF_SHM + 1) * sizeof(char));

	if (T) printf("\n=========================================================================\n");
	//atexit(cleanup);

	ipc_key = ftok(FTOK_FILE, FTOK_PROJ);

	if ((shm_id = shmget(ipc_key, BUF_SHM, 0666)) < 0) pexit("shmget"); // Get share memory - [BUF_SHM]
	shm_adress = shmat(shm_id, NULL, 0);   
	if (shm_adress == (void*) -1) pexit("shmat");

	sem_id = semget(ipc_key, 2, 0666);

	sem.sem_flg = 0;

	FlagSentHeader 	= 0;	
	Flag_F_Name_length_Received = 0;
	Flag_F_Name_Received = 0;
	Flag_F_length_Received = 0;
	Flag_You_can_save = 0;

	FileName_length = 0;
	File_length = 0;
	Header_Size = 0;
	CUR_File_Recieved = 0;

	FileNameBuffer = NULL;
	FileName = NULL;	

	if (tt) printf("[ %s ] START val[%d] = %d     val[%d] = %d\n\n",  curtime(), 0, semctl(sem_id, 0, GETVAL, (int) 0), 1, semctl(sem_id, 1, GETVAL, (int) 0) );

	do
	{
		if (tt) printf(":(333):: %s\n", shm_adress);
		sem.sem_num = 1;
		sem.sem_op  = -1; 
	
		if (tt) say_you("3", _flRule);  // what will be in semop() 
		semop(sem_id, &sem, 1);  
				  

		if (tt) printf("[ %s ]      Not Blocked [%d]            NEW val[%d] = %d     \n",  curtime(), sem.sem_num, sem.sem_num, semctl(sem_id, sem.sem_num, GETVAL, (int) 0) ); 

		strcpy(buf, shm_adress);   // reading from the share memory...
		
		fl_finish = 0;

		if (FlagSentHeader == 0 && (int)strlen(buf) > 0 ) 
		{
			if (FileNameBuffer == NULL) 	// strlen(NULL) = undefined
			{ 	Str_size = (strlen(buf)+1) * sizeof(char); 
				Strlen_F_Buf = 0;
			} 
			else
			{	Str_size = (strlen(FileNameBuffer) + strlen(buf)+1) * sizeof(char); 
				Strlen_F_Buf = strlen(FileNameBuffer);
			}
			
			FileNameBuffer = (char *)realloc(FileNameBuffer, Str_size);
			FileNameBuffer[Strlen_F_Buf]= '\0';

			strncat (FileNameBuffer, buf, strlen(buf)+1); 
			Strlen_F_Buf = strlen(FileNameBuffer);  

			if (Strlen_F_Buf >= MAX_FILE_NAME_LENGTH && Flag_F_Name_length_Received == 0)
			{
				for(i=0; i < MAX_FILE_NAME_LENGTH; i++)		Str_FileName_length[i] = FileNameBuffer[i];
				Str_FileName_length[i] = '\0';
				sscanf(Str_FileName_length, "%d ", &FileName_length);
				if (T) printf("FileName_length = %d\n", FileName_length);
				Flag_F_Name_length_Received = 1;
			}
			if (Strlen_F_Buf >= MAX_FILE_NAME_LENGTH + FileName_length && Flag_F_Name_length_Received == 1 && Flag_F_Name_Received == 0)
			{
				FileName = (char *)malloc( (FileName_length+1 + 4) * sizeof(char) );
				ii = MAX_FILE_NAME_LENGTH;

				for(i=0; i < FileName_length; i++)		FileName[i] = FileNameBuffer[ii++];

				FileName[FileName_length] = '.';
				FileName[FileName_length+1] = 'o';
				FileName[FileName_length+2] = 'u';
				FileName[FileName_length+3] = 't';
				FileName[FileName_length+4] = '\0';

				if (T) printf("FileName = %s\n", FileName);
				Flag_F_Name_Received = 1;
			}
			if (Strlen_F_Buf >= MAX_FILE_NAME_LENGTH + FileName_length + 1 + MAX_FILE_LENGTH && Flag_F_Name_length_Received == 1 && 
                                                                           Flag_F_Name_Received == 1 && Flag_F_length_Received == 0)
			{
				ii = MAX_FILE_NAME_LENGTH + FileName_length + 1;
				for(i=0; i < MAX_FILE_LENGTH; i++)		Str_File_length[i] = FileNameBuffer[ii++];
				Str_File_length[MAX_FILE_LENGTH] = '\0';
				sscanf(Str_File_length, "%d ", &File_length);
				if (T) printf("File_length = %d\n", File_length);

				Flag_F_length_Received = 1;
			}
			if (Strlen_F_Buf >= MAX_FILE_NAME_LENGTH + FileName_length + 1 + MAX_FILE_LENGTH  && Flag_F_Name_length_Received == 1 &&        
                                                                           Flag_F_Name_Received == 1 && Flag_F_length_Received == 1)
			{
				
				fd = open(FileName, O_CREAT | O_RDWR, 0666);

				Header_Size = MAX_FILE_NAME_LENGTH + FileName_length + 1 + MAX_FILE_LENGTH;	

				if (T) printf("(1 part) ");
				i=0;
				for(ii = Header_Size; ii < Strlen_F_Buf; ii++) 
				{	
					buf[i++] = FileNameBuffer[ii];
					if (T) printf("%c", FileNameBuffer[ii]);
				}
				buf[i]='\0';
				write(fd, buf, strlen(buf));
				CUR_File_Recieved = Strlen_F_Buf - Header_Size;
				
				Flag_You_can_save = 0;				
				FlagSentHeader = 1;
				if (tt) printf("uuu>>> CUR_File_Recieved = %d", CUR_File_Recieved);
			}

		} // if (FlagSentHeader == 0)		
		
		if (FlagSentHeader == 1 && Flag_You_can_save == 1 && CUR_File_Recieved + strlen(buf) <= File_length)
		{  
			if (T) printf("%s", buf);
			write(fd, buf, strlen(buf));

			CUR_File_Recieved += strlen(buf);	
			if (tt) printf("ooo>>> CUR_File_Recieved = %d", CUR_File_Recieved);
		}

		if (CUR_File_Recieved >= File_length && FlagSentHeader == 1)	
		{
			close(fd);
			FlagSentHeader 	= 0;	
			Flag_F_Name_length_Received = 0;
			Flag_F_Name_Received = 0;
			Flag_F_length_Received = 0;
			Flag_You_can_save = 0;

			FileName_length = 0;
			File_length = 0;
			Header_Size = 0;
			CUR_File_Recieved = 0;

			if (FileNameBuffer != NULL) free(FileNameBuffer);
			if (FileName != NULL) free(FileName);	
			FileNameBuffer = NULL;
			FileName = NULL;
			if (T) printf("\n-------------------------------------------------------------------------\n");
		}	

		Flag_You_can_save = 1;

		if ((strncmp(buf,"###\0", 4) == 0) && (CUR_File_Recieved >= File_length || CUR_File_Recieved == 0) ) 
		{ 
			if (tt) printf("###!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"); 
			fl_finish = 1; 
			shm_adress[0]='\0';
		}

		if (tt) printf("\n.................................................... sleep  ...\n");
		sleep(tP);
		
		sem.sem_num = 0;
		sem.sem_op  = 1;  

		if (tt) say_you("4", _flRule);  // what will be in semop() 
		semop(sem_id, &sem, 1);  

		if (tt) printf("[ %s ]      Not Blocked [%d]            NEW val[%d] = %d     \n",  curtime(), sem.sem_num, sem.sem_num, semctl(sem_id, sem.sem_num, GETVAL, (int) 0) ); 

		if (tt) printf("\n.................................................... sleep  ...\n");
		sleep(tP);

		if (tt) printf(":(444):: %s\n", shm_adress);

	}
	while (fl_finish==0);

	if (buf != NULL) free(buf);
	if (tt) printf("\n-------------------------------------------------------------------------\n");
	if (tt) printf("[ %s ] OVER val[%d] = %d     val[%d] = %d\n",  curtime(), 0, semctl(sem_id, 0, GETVAL, (int) 0), 1, semctl(sem_id, 1, GETVAL, (int) 0) );

	printf("\nDone. \n");
	cleanup();
	return 0;
}


