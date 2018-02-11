#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_LEN 1000

/*
Custom cat function - Author : Viresh Gupta (2016118)

Supported switches :
-n : number all lines
-b : number only non empty lines
*/

int i;
char buf[2];
int counter = 1, flag=1, consec=0;

void readFile(int fd, int n, int b)
{
	while((i=read(fd,buf,1))>0)
	{
		if(b==1){
			if(flag==1 && buf[0]!='\n'){
				printf("%6d  ", counter);
				counter++;
				flag=0;
			}
			if(buf[0]=='\n')
			{
				flag=1;
			}
		}
		else if(n==1){
			if(flag==1){
				printf("%6d  ", counter);
				counter++;
				flag=0;
			}
			if(buf[0]=='\n')
			{
				flag=1;
			}
		}
		printf("%c",buf[0]);
	}
}

int main(int argc, char *argv[])
{

	char buffer[BUF_LEN], filePath[BUF_LEN];
	int n=0, b=0, fileStart=-1;
	for(int i=1; i<argc; i++)
	{
		if(argv[i][0]=='-'){
			//option
			int j=1;
			while(argv[i][j]!='\0')
			{
				if(argv[i][j]=='n'){
					// set n flag, all lines numbered
					n = 1;
				}
				else if(argv[i][j]=='b')
				{
					// set b flag, do not print line numbers on empty lines
					b = 1;
				}
				j++;
			}
		}
		else{
			fileStart = i;
			break;
		}
	}
	// printf("%d %d\n", b ,n );
	if(fileStart==-1)
	{
		readFile(0,n,b);
		// file descriptor 0 is for the standard input
	    return 0;
	}
	else{
		// check if all files are openable
		for(int i=fileStart; argv[i]!=NULL; i++)
		{
			int fd = open(argv[i],O_RDONLY,0777);
			if(fd>0)
			{
				close(fd);
			}
			else{
				printf("Could not open %s\n", argv[i]);
				return -1;
			}
		}

		// all files openable, read em one by one
		for(int i=fileStart; argv[i]!=NULL; i++)
		{
			int fd = open(argv[i],O_RDONLY,0777);
			readFile(fd,n,b);
			close(fd);
		}
	}

	return 0;
}