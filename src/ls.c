#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE 1

/*
Custom ls function - Author : Viresh Gupta (2016118)

Supported switches : -l -a --colour=[never,always,auto]
-Q : quotes : print with quotes
-a : include all files (including hidden files)
-1 : print one entry on one line
*/

int oneLine=0,allFiles=0,quote=0;

int caseialphasort(const struct dirent **e1, const struct dirent **e2) {
	const char *a = (*e1)->d_name;
	const char *b = (*e2)->d_name;
	return strcasecmp(a, b);
}

int file_select(const struct dirent *entry)
{
	if ((entry->d_name)[0]=='.' && allFiles==0 )
		return (FALSE);
	else
		return (TRUE);
}

int main(int argc, char *argv[]){

	// Parse arguments:
	char path[1000]=".";
	for(int i=1; i<argc; i++)
	{
		if(argv[i][0]=='-'){
			int j=1;
			while(argv[i][j]!='\0'){
				if(argv[i][j]=='Q'){
					quote=1;
				}
				else if(argv[i][j]=='a'){
					allFiles = 1;
				}
				else if(argv[i][j]=='1'){
					oneLine = 1;
				}
				j++;
			}
		}
		else{
			strcpy(path,argv[i]);
		}
	}

	int n;
    struct dirent **dp;
    n = scandir(path, &dp, &file_select, caseialphasort);//opendir(path);
    if(n<0){
    	printf("An error was encountered !\n");
    }
    else{
    	for(int i=0; i<n; i++)
    	{
    		if(quote==1){
    			printf("\"");
    		}
    		printf("%s", dp[i]->d_name);
    		if(quote==1){
    			printf("\"");
    		}
    		if(oneLine==0){
    			printf("  ");
    		}
    		else{
    			printf("\n");
    		}
    		free(dp[i]);
    	}
    	free(dp);
    }
    oneLine==1? :printf("\n");
	return n<0?-1:0;
}