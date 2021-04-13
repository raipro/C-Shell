#include "cexec.h"
#include "cd.h"
#include "headers.h"
#include "echo.h"
#include "ls.h"
#include "pinfo.h"
#include<sys/wait.h>
#include<signal.h>
int forkr;
char str2[100][1000];
int count = 0;
int chpid[200];
int status1[200];
char comm[200][1000];
int fpid = 0;
char fcomm[200];
void stopsignal(int signo)
{
// signal(SIGTTOU, SIG_IGN);	
 setpgid(fpid,fpid);
 signal(SIGTTOU, SIG_IGN);
 tcsetpgrp(0, getpgid(getpid()));
 signal(SIGTTOU, SIG_DFL);
 kill(fpid,19);
 chpid[count] = fpid;
 status1[count] = 2;
 strcpy(comm[count],fcomm);
 count++;
}
void catchsignal(int signo)
{
 //wait(NULL);
 pid_t pid;
 int status;
 if((pid = waitpid(-1, &status,WNOHANG | WUNTRACED | WCONTINUED)) != -1)
 {	 
 for(int i=0;i<count;i++)
 {
	if(pid == chpid[i])
	{
		if(WIFEXITED(status) || WIFSIGNALED(status))
		{	status1[i] = 1;
			char str3[1500];
 			sprintf(str3,"\n%s with pid %d exited normally\n",comm[i],(int)pid);
			write(2,str3,strlen(str3));
		}
		else if(WIFSTOPPED(status))
		{
			status1[i] = 2;
		}
		else if(WIFCONTINUED(status))
		{
			status1[i] = 0;
		}
	}
 }	 
 } 
}
void cexec(char lwd[1000],char dir[10000],char str2[][1000])
{
			if(!strcmp(str2[0],"cd"))
			{
				changedir(lwd,dir,str2[1]);
			}	
			else if(!strcmp(str2[0],"echo"))
			{
				echo(str2);
			}
			else if(!strcmp(str2[0],"pwd"))
			{
				char dir1[10000];
				if(getcwd(dir1,10000)==NULL)
				{
					perror("pwd");
				//	continue;
				}
				else
				{
				printf("%s\n",dir1);
				}
			}	
			else if(!strcmp(str2[0],"ls"))
			{
				list(str2);				
			}
			else if(!strcmp(str2[0],"pinfo"))
			{
				
			if(str2[1][0]=='\0')	
			 pinfo(getpid());
			else
			{long long int x;
			 sscanf(str2[1],"%lld",&x);	
			 pinfo(x);
			}	 
			}
			else if(!strcmp(str2[0],"setenv"))
			{
				if(str2[1][0]=='\0' || str2[3][0]!='\0')
				{
					printf("invalid command");
				}
				else
				{
					if(setenv(str2[1],str2[2],1)==-1)
						{
						perror("envvariable");	
						}
				}
				
			}
			else if(!strcmp(str2[0],"unsetenv"))
			{
				if(str2[1][0] == '\0' || str2[2][0]!='\0')
				{
					printf("invalid command");
				}
				else
				{
					if(unsetenv(str2[1])==-1)
                                                {
                                                perror("envvariable");
                                                }
				}
			}
			else if(!strcmp(str2[0],"jobs"))
			{
				char str3[100];
				int jn = 0;
				for(int i=0;i<count;i++)
				{
			//	 if(kill(pid,0)!=0)
			//	 {
				 if(status1[i]==0)
				 {
					 jn++;
					 printf("[%d] Running %s [%d]\n",jn,comm[i],chpid[i]);
				 }
				 else if(status1[i]==2)
				 {
					  jn++;
					  printf("[%d] Stopped %s [%d]\n",jn,comm[i],chpid[i]);
				 }
			//	 }				 
				}	
			}
			else if(!strcmp(str2[0],"kjob"))
			{
			int x,y;	
			sscanf(str2[2],"%d",&y);
			sscanf(str2[1],"%d",&x);
			if(x<1 || x > count)
			{
				printf("invalid job number");
				return;
			}
			if(y<1 || y>31)
			{
				printf("invalid signal number");
				return;
			}
			int jn = 0;
			for(int i=0;i<count;i++)
			{
				if(status1[i]==0 || status1[i]==2)
				{
					jn++;
					if(jn==x)
					{
						x = i;
					}
				}
			}
			if(kill(chpid[x],y)==-1)
			{
				perror("kill");
				return;
			}
			else
			{
				signal(SIGCHLD,catchsignal);
				//if(y==9)
				//{
			//		status1[x] = 1;
			//	}
			}
			}
			else if(!strcmp(str2[0],"overkill"))
			{
				for(int i=0;i<count;i++)
				{
					if(status1[i]==0 || status1[i]==2)
					{
					//	signal(SIGCHLD,catchsignal);
						kill(chpid[i],15);
						//status1[i] = 1;
					}
				}
			}
			else if(!strcmp(str2[0],"quit"))
			{
				kill(getpid(),9);
			}
			else if(!strcmp(str2[0],"bg"))
			{
				int jn;
				sscanf(str2[1],"%d",&jn);
				int flag = 0;
				for(int i=0;i<count;i++)
				{
					if(status1[i]==0 || status1[i]==2)
					{
						jn--;
						if(jn==0)
						{
							flag = 1;
							if(status1[i]==2)
							{
							status1[i] = 0;
							if(kill(chpid[i],18)==-1)
							{
								perror("send signal");
							}
							}
						}
					}
				}
				if(flag==0)
				{
					printf("invalid job number");
				}
			}
			else if(!strcmp(str2[0],"fg"))
			{
				int jn;
                                sscanf(str2[1],"%d",&jn);
                                int flag = 0;
                                for(int i=0;i<count;i++)
                                {
                                        if(status1[i]==0 || status1[i]==2)
                                        {
                                                jn--;
                                                if(jn==0)
                                                {
                                                        flag = 1;
							signal(SIGTTOU, SIG_IGN);
							tcsetpgrp(0, getpgid(chpid[i]));
							signal(SIGTTOU, SIG_DFL);
							 if(status1[i]==0 || status1[i]==2)
                                                        {
                                                        if(kill(chpid[i],18)==-1)
                                                        {
                                                                perror("kill");
                                                        }
                                                        } 
							if(waitpid(chpid[i], NULL, WUNTRACED)==-1)
							{
							perror("waitpid");
							}
							signal(SIGTTOU, SIG_IGN);
							tcsetpgrp(0, getpgid(getpid()));
							signal(SIGTTOU, SIG_DFL);
						        fpid = chpid[i];
							strcpy(fcomm,comm[i]);
							status1[i] = 1;   	
                                                }
                                        }
                                }
                                if(flag==0)
                                {
                                        printf("invalid job number");
                                }
			}
			else
			{
				char *args[100];
				int i=0;
				int flag = 0;
				while(str2[i][0]!='\0')
				{
					args[i] = &str2[i][0];
					if(str2[i][0]=='&'&& str2[i+1][0]=='\0')
					{
					flag = 1;
					args[i] = NULL;	
					}
					i++;
				}
				args[i] = NULL;
				signal(SIGTSTP,stopsignal);
				//signal(SIGCHLD,catchsignal);
				if(flag==0)
				{
				forkr = fork();
				fpid = forkr;
				strcpy(fcomm,str2[0]);	
				if(forkr<0)
				{
					perror("fork");
				//	continue;
				}
	 			else if(forkr==0)
				{
					
				  signal(SIGINT, SIG_DFL);
			  	  signal(SIGTSTP, SIG_DFL);	  
				 if(execvp(args[0], args)==-1)
                                {
                                        //perror("execvp");
					printf("invalid command\n");
                                  //      continue;
                                }	
				}
				else
				{
				//	signal(SIGCHLD,SIG_IGN);
					waitpid(forkr,NULL,WUNTRACED);
				//	signal(SIGCHLD,catchsignal);
				}
				}
				else if(flag==1)
				{
                                forkr = fork();
                                if(forkr<0)
                                {
                                        perror("fork");
                                }
                                else if(forkr==0)
                                {	
				if(setpgid(0,0)==-1)
				{
					perror("setpgid");
				}
                                if(execvp(args[0], args)==-1)
				{
					//perror("execvp");
					printf("invalid command\n");
				}
                                }
                                else
                                {
				 setpgid(forkr,forkr);	
				 chpid[count] = forkr;
				 status1[count] = 0;
				 strcpy(comm[count],str2[0]);
                                 count++;		 		 	 	 
				 signal(SIGCHLD,catchsignal);
                                 if(tcsetpgrp(STDIN_FILENO,getpgrp())==-1)
				{
					perror("tcsetpgrp");
				//	continue;
				}
                                }

				}
				
			}
}	
