#include "ls.h"
#include "headers.h"
void eval(char str[1000],DIR *dir1,int flaga,int flagl)
{
	struct dirent *entry;
	if(flaga==1 && flagl==1)
	{
		while((entry = readdir(dir1))!=NULL)
		{
			char str1[10000];
			char str2[10000];
			strcpy(str1,entry->d_name);
			strcpy(str2,str);
			if(str2[strlen(str2)-1] != '/')
				strcat(str2,"/");
			strcat(str2,str1);
			struct stat sb;
			if(stat(str2,&sb)==0)
			{
				if(S_ISDIR(sb.st_mode))
					printf("d");
				if(S_ISREG(sb.st_mode))
					printf("-");	
				if(sb.st_mode & S_IRUSR)
					printf("r");
				else	
					printf("-");	
				if(sb.st_mode & S_IWUSR)
					printf("w");
				else
					printf("-");
				if(sb.st_mode & S_IXUSR)
					printf("x");
				else
					printf("-");
				if(sb.st_mode & S_IRGRP)
					printf("r");
				else
					printf("-");
				if(sb.st_mode & S_IWGRP)
					printf("w");
				else
					printf("-");
				if(sb.st_mode & S_IXGRP)
					printf("x");
				else
					printf("-");
				if(sb.st_mode & S_IROTH)
					printf("r"); 
				else
					printf("-"); 
				if(sb.st_mode & S_IWOTH)
					printf("w");
				else
					printf("-");
				if(sb.st_mode & S_IXOTH)
					printf("x");
				else
					printf("-");
				struct passwd *pwd = getpwuid(sb.st_uid);
				struct group *grp = getgrgid(sb.st_gid);
				printf(" %u %s %s %ld %.12s %s\n",(unsigned int)sb.st_nlink,pwd->pw_name,grp->gr_name,sb.st_size,&ctime(&sb.st_mtime)[4],str1);	
			}
			else
			{
				perror("stat");
			}
		}	

	}
	else if(flaga==1 && flagl==0)
	{
		while((entry = readdir(dir1))!=NULL)
		{
			printf("%s\n",entry->d_name);
		}
	}
	else if(flaga==0 && flagl==0)
	{
		while((entry = readdir(dir1))!=NULL)
		{
			if(entry->d_name[0]!='.')
			{
				printf("%s\n",entry->d_name);
			}
		}
	}
	else if(flaga==0 && flagl==1)
	{
		while((entry = readdir(dir1))!=NULL)
		{
			if(entry->d_name[0]!='.')
			{
				char str1[10000];
				char str2[10000];
				strcpy(str1,entry->d_name);
				strcpy(str2,str);
				if(str2[strlen(str2)-1] != '/')
					strcat(str2,"/");
				strcat(str2,str1);
				struct stat sb;
				if(stat(str2,&sb)==0)
				{
					if(S_ISDIR(sb.st_mode))
						printf("d");
					if(S_ISREG(sb.st_mode))
						printf("-");
					if(sb.st_mode & S_IRUSR)
						printf("r");
					else
						printf("-");
					if(sb.st_mode & S_IWUSR)
						printf("w");
					else
						printf("-");
					if(sb.st_mode & S_IXUSR)
						printf("x");
					else
						printf("-");
					if(sb.st_mode & S_IRGRP)
						printf("r");
					else
						printf("-");
					if(sb.st_mode & S_IWGRP)
						printf("w");
					else
						printf("-");
					if(sb.st_mode & S_IXGRP)
						printf("x");
					else
						printf("-");
					if(sb.st_mode & S_IROTH)
						printf("r");
					else
						printf("-");
					if(sb.st_mode & S_IWOTH)
						printf("w");
					else
						printf("-");
					if(sb.st_mode & S_IXOTH)
						printf("x");
					else
						printf("-");
					struct passwd *pwd = getpwuid(sb.st_uid);
					struct group *grp = getgrgid(sb.st_gid);
					printf(" %u %s %s %ld %.12s %s\n",(unsigned int)sb.st_nlink,pwd->pw_name,grp->gr_name,sb.st_size,&ctime(&sb.st_mtime)[4],str1);
				}
				else
				{
					perror("stat");
				}
			}
		}
	}
}
	void list(char str[][1000])
	{
		int flagl = 0;
		int flaga = 0;
		for(int i=1;i<100;i++)
		{
			if(!strcmp(str[i],"-l"))
			{
				flagl = 1;
			}
			else if(!strcmp(str[i],"-a"))
			{
				flaga = 1;
			}
			else if(!strcmp(str[i],"-al"))
			{
				flaga = 1;
				flagl = 1;
			}
			else if(!strcmp(str[i],"-la"))
			{
				flaga = 1;
				flagl = 1;
			}
		}
		int flag = 0;
		for(int i=1;i<100;i++)
		{
			if(str[i][0]!='\0' && str[i][0]!='-')
			{
				flag = 1;	
					DIR *dir1;
			//	struct dirent *entry;
				if((dir1 = opendir(str[i])) == NULL)
					perror(str[i]);
				else
				{
					printf("%s:\n",str[i]);
					eval(str[i],dir1,flaga,flagl);
					closedir(dir1);		
				}                  
			}	
		}
		if(flag ==0)
		{
			        DIR *dir1;
				char s[1000]= ".";
                                if((dir1 = opendir(s)) == NULL)
                                        perror(s);
                                else
                                {
                                        eval(s,dir1,flaga,flagl);
                                        closedir(dir1);
                                }

}
	}
