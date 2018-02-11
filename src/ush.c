#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define _GNU_SOURCE
#include <stdlib.h>
#include <sys/wait.h>
#include "cd.c"
#include "helperFuncs.c"
#include "echo.c"
#include "history.c"

#define BUFSIZE 1000

extern int execvpe(const char *file, char *const argv[], char *const envp[]);

char history[BUFSIZE][BUFSIZE];
int hsiz = 0;
char cwd[BUFSIZE],initwd[BUFSIZE];

struct alias{
	char oldname[100];
	char newname[100];
};

/*
Custom shell - Author : Viresh Gupta (2016118)
OS Assignment 0
*/

void printhelp(){
	printf("Welcome to Ultra Simple Shell\n");
	printf("Author : Viresh Gupta\n");
	printf("Cool Commands : \n");
	printf("cd <location> : \n\t<location> has . and .. as special parameters\n");
	printf("Detects ^D to end the shell session.\n");
}

void loadHistory(char initwd[])
{
	hsiz = 0;
	for(int i=0; i<BUFSIZE; i++)
	{
		strcpy(history[i],"");
	}
	char filePath[BUFSIZE];
	strcpy(filePath,initwd);
	strcat(filePath,".ush_history");
	FILE *fp = fopen (filePath,"r");
	if(fp==NULL){
		// no history to load
	}
	else{
		char str[BUFSIZE];
		while((fgets(str, BUFSIZE, fp)) != NULL)
		{
			str[strlen(str)-1]='\0';
			// printf("%s\n", str);
			// fgets(str2,BUFSIZE,fp);
			// printf("%s\n", str2);
			if(strcmp(str,"\n")!=0){
				strcpy(history[++hsiz],str);
			}
		}
	}
	
}

void saveHistory(int offset,char initwd[])
{
	if(offset <=0){ offset = 1;}
	char filePath[BUFSIZE];
	strcpy(filePath,initwd);
	strcat(filePath,".ush_history");
	FILE * fp = fopen (filePath,"w");
	for(int i=offset; i<=hsiz; i++)
	{
		fprintf (fp, "%s\n",history[i]);
	}
}

void saveCmd(char cmd[])
{
	if(hsiz==BUFSIZE-1){
		saveHistory(BUFSIZE/2, initwd);
		loadHistory(initwd);
	}
	strcpy(history[++hsiz],cmd);
}
// void upperDir(char p[]);

int main(int argc, char *argv[], char * envp[])
{
	int exitCode=0, suppress=0;
	printf("\e[92m                           \n _     _   _____   _     _ \n(_)   (_) (_____) (_)   (_)\n(_)   (_)(_)___   (_)___(_)\n(_)   (_)  (___)_ (_______)\n(_)___(_)  ____(_)(_)   (_)\n (_____)  (_____) (_)   (_)\n                           \n                            \n");
	printf("Welcome to the Ultra Simple Shell.\n    Made by Viresh Gupta.\n\n\e[0m");
	// getcwd(initwd,1000);
	// Set the directory of the Shell Executable
	readlink("/proc/self/exe", initwd, BUFSIZE);
	upperDir(initwd);
	slashTerm(initwd);

	// Set the current working directory from environment varialbe PWD
	for (int i = 0; envp[i] != NULL; i++)
    {    
        if (strncmp(envp[i],"PWD=",4)==0)
        {
        	strcpy(cwd,&envp[i][4]);
    		slashTerm(cwd);
        	break;
        }
    }

	struct alias aliases[50] = {
		{"ll", "ls -1a"}
	};

	loadHistory(initwd);

	while (1) {
		char cmd[BUFSIZE]="",cmdBackup[BUFSIZE]="";
		// getcwd(cwd,BUFSIZE);
		char fileLoc[BUFSIZE] = "";	// allows the execvp to search in all directories in the path from which it was invoked.
		char *ARGS[100]; //Maximum 99 arguments, 0 reserved for executable binary path
		printf("\e[92mUSH\e[0m \e[96m(%s)\e[0m  \e[91m>>>\e[0m ",cwd);
		// char whitespace;
		// scanf("%[^\n]%c",cmd,&whitespace);
		if(fgets(cmd,BUFSIZE,stdin)==NULL){
			break;
			continue;
		}
		if(strcmp("\n",cmd)==0){
			continue;
		}
		else if(strcmp("",cmd)==0){
			continue;
		}
		cmd[strlen(cmd)-1] = '\0';

		strcpy(cmdBackup,cmd);

		for(int i=0; strcmp(aliases[i].oldname,"")!=0; i++){
			if(strcmp(aliases[i].oldname,cmd)==0){
				strcpy(cmd,aliases[i].newname);
				break;
			}
		}

		char* tok;
		int numArgs = 0;
		tok = strtok(cmd, " ");
		while (tok != 0) {
			ARGS[numArgs] = tok;
			numArgs++;
			tok = strtok(0, " ");
		}

		ARGS[numArgs] = NULL;

		if(strcmp("exit",ARGS[0])==0){
			for(int i=1; ARGS[i]!=NULL; i++)
			{
				if(ARGS[i][0]=='-'){
					for(int k=1; ARGS[i][k]!='\0'; k++)
					{
						if(ARGS[i][k]=='s'){
							suppress=1;
						}
					}
				}
				else{
					exitCode = atoi(ARGS[i]);
					break;
				}
			}
			saveCmd(cmdBackup);
			break;
		}
		else if(strcmp("cd",ARGS[0])==0){
			changedir(ARGS,cwd,initwd);
			saveCmd(cmdBackup);
			continue;
		}
		else if(strcmp("pwd",ARGS[0])==0){
			int p=0;	// resolve path's symlink if p=1
			for(int i=1; ARGS[i]!=NULL; i++)
			{
				if(ARGS[i][0]=='-'){
					for(int k=1; ARGS[i][k]!='\0'; k++)
					{
						if(ARGS[i][k]=='L'){
							p=0;
						}
						else if(ARGS[i][k]=='P'){
							p=1;
						}
					}
				}
			}
			char buf[1024]="";
			if(p==1){
				getcwd(buf,BUFSIZE);
			}
			else{
				strcpy(buf,cwd);
			}
			printf("%s\n", buf);
			saveCmd(cmdBackup);
			continue;
		}
		else if(strcmp("echo",ARGS[0])==0){
			echo(numArgs, ARGS);
			saveCmd(cmdBackup);
			continue;
		}
		else if(strcmp("history",ARGS[0])==0){
			showhistory(numArgs,ARGS);
			saveCmd(cmdBackup);
			continue;
		}
		else if(strcmp("help",ARGS[0])==0){
			printhelp();
			saveCmd(cmdBackup);
			continue;
		}
		else if(strcmp("evars",ARGS[0])==0){
			int i;
			for (i = 0; envp[i] != NULL; i++)
		    {    
		        printf("\n%s", envp[i]);
		    }
		    saveCmd(cmdBackup);
			continue;
		}
		else{
			strcpy(fileLoc,initwd);
			strcat(fileLoc,ARGS[0]);
		}

		int pid = fork();
		if(pid==0){
			execve(fileLoc,ARGS,envp);
			printf("Unknown Command |\e[91m%s\e[0m|.... This shell isn't that Intelligent yet !\n",ARGS[0]);
			exit(0);
		}
		else{
			wait(NULL);
			saveCmd(cmdBackup);
		}
	}
	if(suppress==0)
		printf("Thank You !!!\n");
	saveHistory(1,initwd);
	return exitCode;
}