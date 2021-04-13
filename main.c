#include "cd.h"
#include "prompt.h"
#include "headers.h"
#include "echo.h"
#include "ls.h"
#include "pinfo.h"
#include "redirect.h"
#include "cexec.h"
#include<sys/wait.h>
#include<signal.h>
char dir[10000];
//char str[100][1000];
char lwd[1000];
int main()
{
	if(getcwd(dir,10000)==NULL)
	{
		perror("shell directory");
		return 0;
	}
	strcpy(lwd,"-");
//	setpgid(getpid(),getpid());
//	tcsetpgrp(STDIN_FILENO,getpid());	
	while(1)
	{
	//	fflush(stdin);
	//	fflush(stdout);
		signal(SIGINT,SIG_IGN);
		prompt(dir);
		// TAKE INPUT HERE
		char *buf = NULL;
		size_t bsize = 0;
		getline(&buf,&bsize,stdin);
		if(feof(stdin))
		{
			printf("\n");
			kill(getpid(),9);
		}
		//following line has been added to ignore '\n' from getline
		if(buf[strlen(buf)-1] == '\n')
		buf[strlen(buf)-1] = '\0';
		char *comm;
		char s[2] =";";
		comm = strtok(buf,s);
		while(comm != NULL)
		{
			//printf("sasti");
			int pipei[100];
			int pc = 0;
			char str[100][1000];
			memset(str,'\0',100000);
			char str1[100][1000];
			memset(str1,'\0',100000);
			int j1=0;
			int j=0;
			for(long long int i=0;i<strlen(comm);i++)
			{
				if(comm[i]==' '||comm[i]=='\t')
				{
					continue;
				}
				str[j][j1] = comm[i];
				j1++;
				if(comm[i+1]==' '||comm[i+1]=='\t')
				{
					str[j][j1] = '\0';
					if(!strcmp(str[j],"|"))
					{
						pipei[pc] = j;
						pc++;
					}
					j++;	
					j1=0;
				}
			}
			//implementing piping
			if(pc==0)
			{
			if(redirect(dir,str)==0)
			{
				cexec(lwd,dir,str);		
			}
			}
			else
			{
				int pipes[2*pc];
				for(int i = 0;i < pc ;i++)
				{
					pipe(pipes + (2*i));
				}
				int cpy = dup(STDOUT_FILENO);
				dup2(pipes[1],STDOUT_FILENO);	
				for(int i=0;i<pipei[0];i++)
				{
					strcpy(str1[i],str[i]);
				}
				  if(redirect(dir,str1)==0)
                        	{
					//printf("%s %s\n",str1[0],str1[1]);
                        		cexec(lwd,dir,str1);
                        	}
				dup2(cpy,STDOUT_FILENO);
				for(int i=1;i<=pc-1;i++)
				{
					int cpy1 = dup(STDIN_FILENO);
					int cpy2 = dup(STDOUT_FILENO);
					dup2(pipes[(2*i)-2],STDIN_FILENO);
					dup2(pipes[((2*i)+1)],STDOUT_FILENO);
					close(pipes[(2*i)-1]);
		//			close(pipes[(2*i)-2]);
				//	close(pipes[(2*i)+1]);
					memset(str1,'\0',100000);
					int c = 0;
					 for(int j=pipei[i-1]+1;j<pipei[i];j++)
                                	{
                                        strcpy(str1[c],str[j]);
				//	printf("%s\n",str1[c]);
					c++;
                                	}
                                  	if(redirect(dir,str1)==0)
                                	{
                                        cexec(lwd,dir,str1);
                                	}
					dup2(cpy1,STDIN_FILENO);
					dup2(cpy2,STDOUT_FILENO);
				}
					int cpy1 = dup(STDIN_FILENO);
					memset(str1,'\0',100000);
                                	dup2(pipes[(2*pc)-2],STDIN_FILENO);
					int c=0;
					//close(pipes[(2*pc)-2]);
					close(pipes[(2*pc)-1]);		
                                	for(int i=pipei[pc-1] + 1;str[i][0]!='\0';i++)
                                	{
                                        	strcpy(str1[c],str[i]);
						c++;
                                	}
					close(pipes[2*pc-1]);
                                  	if(redirect(dir,str1)==0)
                                	{
                                        	cexec(lwd,dir,str1);
                                	}
                                	dup2(cpy1,STDIN_FILENO);
				
		//		for(int i=0;i<2*pc;i++)
		//		{
		//			close(pipes[i]);
		///		}

			}		
			comm = strtok(NULL,s);
		}
		free(buf);

	}
	return 0;
}
