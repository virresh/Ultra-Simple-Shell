void upperDir(char p[])
{
	// Converte /abc/def/efg/ -> /abc/def/
	int i=0;
	while(p[i]!='\0'){i++;}
	int j=i-1;
	while(j>=0){
		if(p[j]=='/' && j!=i-1){
			p[j+1]='\0';
			break;
		}
		j--;
	}
}

void slashTerm(char path[])
{
	int k=strlen(path);
	if(path[k-1]!='/'){
		path[k]='/';
		path[k+1]='\0';
	}
}