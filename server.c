
#include <semaphore.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define MEM_SIZE 64

int main(int argc, char** argv)
{
	int fd;
	int re;
	int count=0;
	int operator= 1;
	int calculator = 0;

	int zero=0;
	int* ptr;
	char* ptr2 = NULL;

	sem_t* sem;
	sem_t* cli_sem;

	int client[2];
	char num[100];

	sem_unlink("mysema");
	if((sem = sem_open("mysema", O_CREAT, 0777, 0) )== SEM_FAILED)
	{
		perror("open");
		return 1;
	}
	sem_unlink("clientsema");
	if((cli_sem = sem_open("clientsema", O_CREAT, 0777, 0) )== SEM_FAILED)
	{
		perror("open");
		return 1;
	}

	if((fd = open("inputY.txt", O_RDWR|O_CREAT, S_IRWXU))<0){
		perror("Sem Open Error");
		return 1;
	}
	if((re = open("resultY.txt", O_RDWR|O_CREAT, S_IRWXU))<0){
		perror("Sem Open Error");
		return 1;
	}

	write(fd, &zero, sizeof(int));
	ptr = mmap(NULL, sizeof(int), PROT_READ , MAP_SHARED, fd, 0);
	close(fd);

	write(re, " ", 1);
	ptr2 = (char*)mmap(NULL, MEM_SIZE, PROT_WRITE|PROT_READ, MAP_SHARED, re, 0);
	close(re);



	//클라이언트 0받기
	strcpy((ptr2),"0");
	sem_wait(sem);
	client[0] = (*ptr);
	printf("client 0 hi\n");
	strcpy((ptr2),"1");

	//클라이언트 1받기
	sem_wait(sem);
	client[1] = (*ptr);
	printf("client 1 hi\n");
	strcpy((ptr2),"start");

	printf("=========================\n");
	printf("        Game Start!!\n");
	printf("=========================\n");

	count =1;//클라이언트 1부터 시작.(숫자입력먼저)	
	while(1){
		sem_wait(sem); //클라이언트에 값을 받기위해서 기다린다.
		sprintf(num,"%d",client[count]);
		if(count==0){

			operator=(*ptr);
			if(operator==1){
				strcpy((ptr2)," [");
				strcat((ptr2),num);
				strcat((ptr2),"]1(+)");
				printf("%s", (ptr2));

			}else if(operator == 2)
			{
				strcpy((ptr2)," [");
				strcat((ptr2),num);
				strcat((ptr2),"]2(-)");
				printf("%s", (ptr2));
				
			}else if(operator == 3){
				strcpy((ptr2)," [");
				strcat((ptr2),num);
				strcat((ptr2),"]3(=)");
				printf("%s\n", (ptr2));

			}
			sem_post(cli_sem);
			sem_wait(sem);
			sem_post(cli_sem);
			sem_wait(sem);

			count=1;
			if(operator == 3)break;
		}
		else if(count==1){ 

			if(1<=(*ptr) && (*ptr)<=100){
				char inte[3];
				sprintf(inte,"%d",(*ptr));

				strcpy((ptr2)," [");
				strcat((ptr2),num);
				strcat((ptr2),"]");
				strcat((ptr2),inte);

				if(operator ==1) calculator = calculator + (*ptr);
				else calculator = calculator - (*ptr);

		
				printf("%s\n", (ptr2));
				sem_post(cli_sem);
				sem_wait(sem);
				sem_post(cli_sem);
				sem_wait(sem);
	
			}

			count=0;
		}

		sem_post(cli_sem);
	}

	sem_post(cli_sem);
	sem_wait(sem);


	if(calculator == (*ptr)){
		printf("%d win!\n",client[1]);
		char id[20];
		sprintf(id,"%d",client[1]);
		strcpy((ptr2),id);

	}else{
		printf("%d win!\n",client[0]);
		char id[20];
		sprintf(id,"%d",client[0]);
		strcpy((ptr2),id);	
	}


	sem_post(cli_sem);
	sem_wait(sem);
	sem_post(cli_sem);
	sem_wait(sem);


	unlink("inputY.txt");	
	unlink("resultY.txt");
	sem_close(sem);
	sem_close(cli_sem);
}

