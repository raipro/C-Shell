#include "headers.h"
#include "redirect.h"
#include "cexec.h"
int redirect(char dir[10000],char str[][1000])
{
	int flag = 0;
	char str1[100][1000];
	int j=0;
	 memset(str1,'\0',100000);
//	printf("dumpy");
	for(int i = 0;str[i][0]!='\0';i++)
	{
		//printf("%s",str[i]);
		//args[j] = &str[j][0];
		//j++;
		if(!strcmp(str[i],">") || !strcmp(str[i],">>"))
		{
//			printf("humpy");
			flag = 1;
			int fd = open(str[i+1],O_RDWR | O_CREAT ,0644);
			if(!strcmp(str[i],">>"))
			{
				lseek(fd,0,SEEK_END);
			}
			int cpy1 = -1;
                        if(!strcmp(str[i+2],">"))
                        {
                                int fd1 = open(str[i+3],O_RDWR | O_CREAT ,0644);
                                cpy1 = dup(STDOUT_FILENO);
                                dup2(fd1,STDOUT_FILENO);
                        }
                        else if(!strcmp(str[i+2],">>"))
                        {
                                int fd1 = open(str[i+3],O_RDWR | O_CREAT ,0644);
                                lseek(fd1,0,SEEK_END);
                                cpy1 = dup(STDOUT_FILENO);
                                dup2(fd1,STDOUT_FILENO);
                        }
	//		printf("%s",str[i+1]);
			if(fd<0)
			{
				perror("output file");
				return 0;
			}
			int cpy;
			if(cpy1==-1)
			{
			  cpy = dup(STDOUT_FILENO);	
			if(dup2(fd,STDOUT_FILENO)==-1)
			{
				perror("output file");
				return 0;
			}
			}
			close(fd);
		//	int forkr = fork();
		//	if(forkr==0)
		//	{
		        cexec(dir,str1);
			//dup2(cpy,STDOUT_FILENO);
		//	}
		///	else
		//	{
			//dup2(cpy,STDOUT_FILENO);
			if(cpy1!=-1)
                        {
                                dup2(cpy1,STDOUT_FILENO);
                        }
			else
			{
				 dup2(cpy,STDOUT_FILENO);
			}	
                        break;
		//	}
			//close(fd);		
		}
		else if(!strcmp(str[i],"<"))
		{
			flag = 1;
			int fd = open(str[i+1],O_RDONLY);
			int cpy1 = -1;
			if(!strcmp(str[i+2],">"))
			{
				int fd1 = open(str[i+3],O_RDWR | O_CREAT ,0644);
				cpy1 = dup(STDOUT_FILENO);
				dup2(fd1,STDOUT_FILENO);
			}
			else if(!strcmp(str[i+2],">>"))
			{
				int fd1 = open(str[i+3],O_RDWR | O_CREAT ,0644);
				lseek(fd1,0,SEEK_END);
				cpy1 = dup(STDOUT_FILENO);
				dup2(fd1,STDOUT_FILENO);
			}
			int cpy = dup(STDIN_FILENO);
			if(fd<0)
			{
				perror("input file");
				return 0;
			}

			 if(dup2(fd,STDIN_FILENO)==-1)
                        {
                                perror("input file");
                                return 0;
                        }
			close(fd);	
                        cexec(dir,str1);
			dup2(cpy,STDIN_FILENO);
			if(cpy1!=-1)
			{
				dup2(cpy1,STDOUT_FILENO);
			}	
                        break;
		}
		else
		{ 
		  for(int i1=0;str[j][i1]!='\0';i1++)
		{	  
		  str1[j][i1] = str[j][i1];
		}
		//  printf("%s",str1[j]);
		  j++;
		}

	}
	return flag;
}
