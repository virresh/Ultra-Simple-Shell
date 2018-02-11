#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#ifndef BUFSIZE
#define BUFSIZE 1000
#endif

/*
Custom history function - Author : Viresh Gupta (2016118)

Supported switches :
-c : suppress newline at the ending
-d offset : delete the command displayed at the number offset
*/

extern char history[BUFSIZE][BUFSIZE];
extern int hsiz;

int showhistory(int argc, char *argv[]){

	char buffer[1000]="";
	int offSet=0;
	int clear = 0, deleteOffset = 0, delFlag=0, startLoc=-1;
	for(int i=1; i<argc; i++)
	{
		if(deleteOffset==1)
		{
			offSet = atoi(argv[i]);
			deleteOffset=0;
			if(offSet!=0){
				delFlag = 1;
			}
			else{
				return -1;
			}
		}
		else if(argv[i][0]=='-'){
			int j=1;
			while(argv[i][j]!='\0'){
				if(argv[i][j]=='c'){
					clear=1;
				}
				else if(argv[i][j]=='d'){
					deleteOffset = 1;
				}
				j++;
			}
		}
		else{
			break;
		}
	}
	if(clear==0 && delFlag==0)
	{
		for(int i=1; i<=hsiz; i++)
		{
			printf("%6d  %s\n",i,history[i] );
		}
	}
	else if(clear==1)
	{
		while(hsiz>0)
		{
			strcpy(history[hsiz--],"");
		}
	}
	else if(delFlag==1)
	{
		if(offSet>hsiz)
		{
			printf("Invalid offset !\n");
			return -1;
		}
		else{
			for(int i=offSet; i<=hsiz; i++)
			{
				strcpy(history[i] ,history[i+1]);
			}
			strcpy(history[hsiz--],"");
		}
	}
	return 0;
}