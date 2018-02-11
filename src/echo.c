#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#ifndef BUFSIZE
#define BUFSIZE 1000
#endif

/*
Custom echo function - Author : Viresh Gupta (2016118)

Supported switches :
-n : suppress newline at the ending
-e : escape
*/

void parseArgs(char expandedStr[])
{
	char origStr[BUFSIZE];
	int ctrl=0,j=0,i=0;
	strcpy(origStr,expandedStr);
	strcpy(expandedStr,"");
	for (i = 0; origStr[i]!='\0'; ++i)
	{
		if(origStr[i]=='\\'){
			ctrl=1;
		}
		else if(ctrl==1){
			// potential escaped character
			switch(origStr[i]){
				case 'b': expandedStr[j++]='\b'; break;
				case 'n': expandedStr[j++]='\n'; break;
				case 't': expandedStr[j++]='\t'; break;
				case 'v': expandedStr[j++]='\v'; break;
				case 'a': expandedStr[j++]='\a'; break;
				case 'r': expandedStr[j++]='\r'; break;
				case '\\': expandedStr[j++]='\\'; break;
				case '0': expandedStr[j++]='\0'; break;
				default : expandedStr[j++]='\\';expandedStr[j++]=origStr[i]; break;
			}
			ctrl=0;
		}
		else{
			expandedStr[j]=origStr[i];
			j++;
		}
	}
	expandedStr[j]='\0';
}

void echo(int argc, char *argv[]){

	// Parse arguments:
	char buffer[1000]="";
	int newL = 0, escape = 0, startLoc=-1;
	for(int i=1; i<argc; i++)
	{
		if(argv[i][0]=='-'){
			int j=1;
			while(argv[i][j]!='\0'){
				if(argv[i][j]=='n'){
					newL=1;
				}
				else if(argv[i][j]=='e'){
					escape = 1;
				}
				j++;
			}
		}
		else{
			startLoc = i;
			break;
		}
	}

	for(int i=startLoc; i<argc && i>0; i++)
	{
		strcat(buffer,argv[i]);
		if(i!=argc-1)
			strcat(buffer," ");
	}
	if(escape==1){
		parseArgs(buffer);
	}
	printf("%s",buffer);
	if(newL!=1){
		printf("\n");
	}
}