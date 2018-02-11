#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "helperFuncs.c"

#ifndef BUFSIZE
#define BUFSIZE 1000
#endif

/*
Custom mkdir function - Author : Viresh Gupta (2016118)

Supported switches :
-m : set the mode of the directory created
-p : make the parent directories as well
*/

// does not handle spaces in path name

int parseModes(char mstring[])
{
	int q = 0;
	int i=0;
	if(strlen(mstring)!=3){ return -1; }
	for(i=0; i<3; i++)
	{
		q = q<<3;
		int val=(int)(mstring[i]-'0');
		if(mstring[i]=='\0')
		{
			return -1; // invalid mode string
		}
		else if(val < 0 || val >=8){
			return -1; // not an octal number
		}
		else{
			for(int j=0; (1<<j)<=4; j++)
			{
				int flagBit = (1<<j);
				q = (q) | (flagBit & val);
				// printf("flagBit = %d, val = %d, j=%d, q=%d\n",flagBit,val,j,q);
			}
		}
	}
	return q;
}

int recursiveMkdir(char path[], int mode)
{
	if(strcmp(path,"/")==0){
		return -1;
	}
	int k = mkdir(path,mode);
	if(k<0){
		char path2[BUFSIZE];
		strcpy(path2,path);
		upperDir(path2);
		k = recursiveMkdir(path2,mode);
	}
	else{
		return 0;
	}
	return mkdir(path, mode);
}

int main(int argc, char *argv[]){

	// Parse arguments:
	char path[1000]="";
	int parentFlag = 0, modeF = 0, modeArgs = 0777,fileStart=-1;
	for(int i=1; i<argc; i++)
	{
		int j=1;
		if(argv[i][0]=='-'){
			while(argv[i][j]!='\0'){
				if(argv[i][j]=='m'){
					modeF=1;
				}
				else if(argv[i][j]=='p'){
					parentFlag = 1;
				}
				j++;
			}
		}
		else if(modeF==1){
			// this must be a mode value, treat it as such
			modeF = 0;
			int givenMode = parseModes(argv[i]);
			if(givenMode == -1){
				modeArgs = 0777;
				printf("Not a valid mode !\n");
				return -1;
			}
			else{
				modeArgs = givenMode;
			}
		}
		else{
			fileStart=i;
			break;
		}
	}
	for(int i=fileStart; i<argc; i++)
	{
		strcpy(path,argv[i]);
		int ret;
		if(parentFlag==1){
			ret = recursiveMkdir(path, modeArgs);
		}
		else{
			ret = mkdir(path,modeArgs);
		}
	
		if(ret < 0 )
		{
			printf("mkdir: Cannot create the directory %s !\n",path);
		}
	}
	return 0;
}