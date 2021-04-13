#include "prompt.h"
#include "headers.h"

char dir1[1000];
void prompt(char dir[]) 
{
    char hname[1000];
    char uname[1000];
    gethostname(hname,1000);
    getlogin_r(uname,1000);
    getcwd(dir1,1000);
    if(strlen(dir1)>=strlen(dir))
    {
	    int flag=0;
    for(int i=0;i<strlen(dir);i++)
    {
	    if(dir1[i]!=dir[i])
	    {
		flag=1;
	    }
    }
    if(flag==0)
    {
    printf("<%s@%s:~%s>",uname,hname,&dir1[strlen(dir)]);
    }
    else
    printf("<%s@%s:%s>",uname,hname,dir1);
    }
    else
    printf("<%s@%s:%s>",uname,hname,dir1);
}
