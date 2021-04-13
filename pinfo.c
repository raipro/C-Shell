#include "headers.h"
#include "pinfo.h"

void pinfo(long long int pid)
{
 printf("pid -- %lld\n",pid);
 char buf[1024],str1[100],str2[100];
 sprintf(str2,"/proc/%lld/stat",pid);
 FILE* file = fopen(str2,"r");
 if(file  == NULL)
 {
	 perror("file opening");
	 return;
 }
 
 char buf1[1000];
 while(fgets(buf1,sizeof(buf1), file))
 {
	char *comm;
 	char s[2] =" ";
 	comm = strtok(buf1,s);
	int j=0;
	while(comm!=NULL)
	{
		if(j==2)
		{
	  		printf("Process Status -- %s\n",comm);
		}
		if(j==22)
		{
			printf("memory -- %s\n",comm);
		}	
		j++;
		comm = strtok(NULL,s);	
	}	
 }   
 fclose(file);
 sprintf(str1,"/proc/%lld/exe",pid);
 int len;
 len = readlink(str1,buf, sizeof(buf) - 1);
 if(len!=-1)
 {
	 buf[len] = '\0';
	 printf("Executable Path -- %s\n",buf);
 }
 else
 {
	 perror("readlink");
 }
}
