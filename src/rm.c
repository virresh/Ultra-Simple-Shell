#include <stdio.h>
#include <string.h>
#include <fcntl.h>           /* Definition of AT_* constants */
#include <unistd.h>


/*
Custom rm function - Author : Viresh Gupta (2016118)

Supported switches :
-i : interactive, prompt before each removal
-d : remove empty directories as well
*/

int main(int argc, char *argv[]){

	// Parse arguments:
	char path[1000]="";
	int interactive = 0, directories = 0, fileListStart=-1;
	for(int i=1; i<argc; i++)
	{
		if(argv[i][0]=='-'){
			int j=1;
			while(argv[i][j]!='\0'){
				if(argv[i][j]=='i'){
					interactive=1;
				}
				else if(argv[i][j]=='d'){
					directories = 1;
				}
				j++;
			}
		}
		else{
			fileListStart = i;
			break;
		}
	}
	if(fileListStart==-1)
	{
		printf("rm: missing operand\n");
		return 0;
	}
	for(int i=fileListStart; i<argc; i++)
	{
		strcpy(path,argv[i]);
		char result[100];
		int  res=1;
		if(interactive==1){
			printf("Do you want to remove %s ? (y / n)\n", path);
			int i=scanf("%s",result);
			// printf("%s\n",result);
			if(result[0]=='n' || result[0]=='N'){
				res=0;
			}
		}

		if(res==1){
			res = unlink(path);
			if(res<0 && directories==1)
			{
				res = unlinkat(AT_FDCWD,path,AT_REMOVEDIR);
			}
		}
		if(res < 0){
			printf("Error Occurred while attempting to remove %s\n",path);
			return res;
		}
	}

	return 0;
}