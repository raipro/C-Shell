#include "headers.h"

void echo(char str[][1000])
{
  char str1[1000];
  str1[0]='\0'; 
  for(int i=1;i<100;i++)
  { if(str[i][0]!='\0')	  
	  {
		  //printf("%s %s\n",str[i],str1);
		  strcat(str1,str[i]);
		  strcat(str1," ");
	//	  printf("%s %s\n",str[i],str1);
	  }
    	    
  }
  str1[strlen(str1)-1] = '\0';
  printf("%s\n",str1);  
}
