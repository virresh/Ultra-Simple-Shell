#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define BUF_LEN 1000

/*
Custom date function - Author : Viresh Gupta (2016118)

Supported switches : -r -R
-r <filename/path> : Gives the last modified date of a file
-R : Output date in RFC Format (Mon, 14 Aug 2006 02:34:56 -0600)
*/

int main(int argc, char *argv[])
{
	char buffer[BUF_LEN], filePath[BUF_LEN];
	int r=0, R=0;
	for(int i=1; i<argc; i++)
	{
		if(argv[i][0]=='-'){
			//option
			int j=1;
			while(argv[i][j]!='\0')
			{
				if(argv[i][j]=='r'){
					// set small r flag, for file's last modified
					r = 1;
				}
				else if(argv[i][j]=='R')
				{
					// set big R flag, time format
					R = 1;
				}
				j++;
			}
		}
		else{
			strcpy(filePath,argv[i]);
		}
	}
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);

	if(r==1){
		struct stat attrib;
		if(stat(filePath, &attrib) < 0){
			printf("Invalid Path.\n");
			return 0;
		}
		tm = localtime(&(attrib.st_ctime));
	}
	
	// printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	// printf("%s\n",asctime(tm));
	if(R==1){
		// Mon, 14 Aug 2006 02:34:56 -0600
		strftime(buffer,BUF_LEN, "%a, %d %b %G %R:%S %z", tm);
	}
	else{
		// Wed Jan 17 13:46:42 IST 2018
		strftime(buffer,BUF_LEN, "%a %b %d %R:%S %Z %Y", tm);
	}
	printf("%s\n", buffer);
	return 0;
}