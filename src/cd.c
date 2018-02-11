#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifndef BUFSIZE
#define BUFSIZE 1000
#endif

extern void upperDir(char p[]);
extern void slashTerm(char path[]);

/*
Custom cd function - Author : Viresh Gupta (2016118)

Supported switches :
-P : resolve all symlinks and go to absolute directory
-L : do not resolve any symlinks (default)
*/

void parseDir(char inputPath[], char finalPath[], char initwd[])
{
	int prevBreak=0, i=0;
	if(inputPath[0]=='/'){
		//absolute path, send as it is
		strcpy(finalPath,inputPath);
		slashTerm(finalPath);
		return;
	}
	else if(strncmp(inputPath,"~/",2)==0){
		strcpy(finalPath,initwd);
		prevBreak = 2;
	}
	for(i=0; inputPath[i]!='\0'; i++)
	{
		if(inputPath[i]=='/' && prevBreak < i)
		{
			char x[BUFSIZE]="";
			strncpy(x,&inputPath[prevBreak],i-prevBreak);
			if(strcmp(x,"..")==0){
				upperDir(finalPath);
			}
			else if(strcmp(x,".")==0){
				// do nothing
			}
			else{
				strcat(finalPath,x);
				strcat(finalPath,"/");
			}
			prevBreak = i+1;
		}
	}
	if(prevBreak!=i){
		char x[BUFSIZE]="";
		strncpy(x,&inputPath[prevBreak],i-prevBreak);
		strcat(finalPath,x);
		strcat(finalPath,"/");
	}

	slashTerm(finalPath);
}

void changedir(char *ARGS[], char cwd[], char initwd[])
{
	int p=0; // Resolve symbolic link if p = 1
	if(ARGS[1]==NULL){
		chdir(initwd);
		strcpy(cwd,initwd);
		return;
	}
	if(ARGS[1][0]=='-'){
		if(strcmp(ARGS[1],"-P")==0){
			p=1;
		}
		else if(strcmp(ARGS[1],"-L")==0){
			// do nothing
		}
		else{
			printf("Unknown Parameter.\n");
			return;
		}
		for(int i=1; ARGS[i]!=NULL; i++)
		{
			// strcpy(ARGS[i],ARGS[i+1]);
			ARGS[i]=ARGS[i+1];
		}
	}
	if(ARGS[1]==NULL){	
		return;
	}
	if( strcmp(ARGS[1],"~")==0){
		chdir(initwd);
		strcpy(cwd,initwd);
	}
	else if(strcmp(ARGS[1],".")==0){
	}
	else if(strcmp(ARGS[1],"..")==0){
		upperDir(cwd);
		chdir(cwd);
	}
	else if(strcmp(ARGS[1],"~")==0){
		chdir(initwd);
		strcpy(cwd,initwd);
	}
	else{
		char tempPath[BUFSIZE];
		if(ARGS[1][0]=='/')
		{
			// is an absolute path, so handle as it is
			strcpy(tempPath,ARGS[1]);
			slashTerm(tempPath);
		}
		else{
			char argPathConcat[BUFSIZE];
			int i=2;
			strcpy(argPathConcat,ARGS[1]); 
			while(ARGS[i]!=NULL)
			{
				strcat(argPathConcat," ");
				strcat(argPathConcat,ARGS[i]);
				i++;
			}
			strcpy(tempPath,cwd);
			parseDir(argPathConcat,tempPath,initwd);
			// printf("%s <--------- Parsed\n",tempPath );
		}
		if(chdir(tempPath)<0)
		{
			printf("Directory %s not found!\n",tempPath);
		}
		else{
			strcpy(cwd,tempPath);
		}
	}
	if(p==1){
		getcwd(cwd,BUFSIZE);
	}
}