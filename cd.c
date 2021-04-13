#include "headers.h"
#include "cd.h"

void changedir(char lwd[],char dir[],char str[])
{  	
  if(!strcmp(str,"~"))
  {
	   getcwd(lwd,1000);
	   if(chdir(dir)!=0)
  		{
        		perror(dir);
  		}

  }
  else if(!strcmp(str,"-"))
  {
    if(chdir(lwd)!=0)
                {
                        perror(dir);
                }
  }
  else
  { 
   getcwd(lwd,1000);	  
  if(chdir(str)!=0)
  {
	  
	perror(str);
  }
  }
}
