/*************************************************************************
	> File Name: cp.c
	> Author:hrp 
	> Mail: 
	> Created Time: 2021年03月22日 星期一 04时52分24秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#define BUFFERSIZE 4096
#define COPYMODE 0644
void oops(char *s1,char *s2);
 int main(int ac,char *av[])
 {
     int in_fd,out_fd,n_chars;
     char buf[BUFFERSIZE];
     if(ac != 3)
     {
         fprintf(stderr,"usage:%s source destination \n",*av);
         return -1;
     }


     if((in_fd = open(av[1],O_RDONLY)) == -1)
     {
        oops("read error\n ",av[1]); 
     }

     if((out_fd = creat(av[2],COPYMODE)) == -1)
     {
        oops("write error!!!! \n  ",av[2]);
     }
     
     while((n_chars = read(in_fd,buf,BUFFERSIZE)) > 0)
        if(write(out_fd,buf,n_chars) != n_chars)
              oops("write Error to \n ",av[2]);

    if(n_chars == -1)
        oops("Read error from ",av[1]);

    if(close(in_fd) == -1 || close(out_fd)==-1)
        oops("Error closing file","  ") ;

     return 0; 
 }

void oops(char * s1,char *s2)
{
    fprintf(stderr,"Error: %s",s1);
    perror(s2);

}


