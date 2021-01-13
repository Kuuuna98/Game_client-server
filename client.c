
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

sem_t* cli_sem;
sem_t* sem;

int* ptr;
char* ptr2;

int mypid;

void c0(void);
void c1(void);

int main(int argc, char** argv)
{
	int fd;
	int re;
	int zero=0;


	
	if((fd = open("inputY.txt", O_RDWR, S_IRWXU))<0)
	{
		printf("do not server\n");
		return 1;
	}

	if((re = open("resultY.txt", O_RDWR, S_IRWXU))<0)
	{
		perror("Sem Open Error");
		return 1;
	}
     if((sem = sem_open("mysema",0, 0777, 0) )== SEM_FAILED)
        {
                perror("open");
                return 1;
        }
        if((cli_sem = sem_open("clientsema",0, 0777, 0) )== SEM_FAILED)
        {
                perror("open");
                return 1;
        }



	write(fd, &zero, sizeof(int));
	ptr = mmap(NULL, sizeof(int), PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	ptr2 = (char*)mmap(NULL, 64, PROT_READ, MAP_SHARED, re, 0);
	close(re);



	if(strcmp(ptr2,"0")!=0 && strcmp(ptr2,"1")!=0){
		printf("You can't enter this room!\n");
		return 1;
	}


	mypid = getpid();
        (*ptr)= getpid();

	printf("=========================\n");
	printf("        Hello %d\n",getpid());
	printf("        Game Start\n");
	printf("=========================\n");


	if(strcmp(ptr2,"0")==0)	{
		sem_post(sem);
		c0();}
	else {
		sem_post(sem);
		sleep(2);
		c1();
	}

	sem_post(sem);
	sem_close(sem);
	sem_close(cli_sem);
}

void c0(void){
	sem_wait(cli_sem);
	while(1){
		if(ptr2[1] != '[')
		{
			char inte[20];
			sprintf(inte,"%d",mypid);

			if(strcmp(inte,ptr2)==0)
			{
				printf("Win in a Game\n");
			}else{
				printf("Lose in a Game\n");
			}
			break;

		}else{

			printf("%s\n", (ptr2));
			sem_post(sem);
			sem_wait(cli_sem);

			int a=0;
			while(a!=1 && a!=2 && a!=3){
				printf("input +, -, = (+ : 1 / - : 2 / = : 3) : ");
				scanf("%d",&a);
			}
			(*ptr) = a;

			sem_post(sem);
			sem_wait(cli_sem);

			printf("%s\n", (ptr2));
			sem_post(sem);
			sem_wait(cli_sem);
		}
	}
}

void c1(void){
	while(1){
		int b=0;
	if(ptr2[strlen(ptr2)-2]=='='){
			
		printf("calcurate this operation : ");
			scanf("%d",&b);
			(*ptr) = b;
			sem_post(sem);
			sem_wait(cli_sem);
			break;
		}
		else{
			while(b<1 || 100<b){
				printf("input num(1~100) : ");
				scanf("%d",&b);
			}
			(*ptr) = b;

			sem_post(sem);
			sem_wait(cli_sem);

			printf("%s\n", (ptr2));
			sem_post(sem);
			sem_wait(cli_sem);

			printf("%s\n", (ptr2));
			sem_post(sem);
			sem_wait(cli_sem);
		}
	}

	char inte[20];
	sprintf(inte,"%d",mypid);

	if(strcmp(inte,ptr2)==0){
		printf("Win in a Game\n");
	}else{
		printf("Lose in a Game\n");
	}
}


