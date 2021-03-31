/*************************************************************************
	> File Name: cp.c
	> Author:hrp 
	> Mail: 
	> Created Time: 2021年03月22日 星期一 04时52分24秒
add -i command
可以复制到制定的目录里

 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>



#include<sys/stat.h>

#include<string.h>
#define BUFFERSIZE 4096
#define COPYMODE 0644
void read_write(char *src,char *dst);
void oops(char *s1,char *s2);
char buf[BUFFERSIZE];
 int main(int ac,char *av[])
 {
     int in_fd,out_fd,n_chars;
     
     
     if(ac == 0)
     {
         fprintf(stderr,"usage:%s source destination \n",*av);
         return -1;
     }
    
     //add -i command
     if(strcmp(av[1],"-i") == 0)  
     {
         char *p = av[1];
         int c;
         if(access(av[3],F_OK)==0 )
         {
           
	    printf("文件已经存在，是否覆盖 y/n   : ");
    //    c= getchar();
 	//    printf("%c",c);
	    if( (c = getchar()) == 'y')
	      {
	    	printf("yes");
        	read_write(av[2],av[3]);	
	      }
            return -1;
         }
     }
     read_write(av[1],av[2]);
     return 0; 
 }

void oops(char * s1,char *s2)
{
    fprintf(stderr,"Error: %s",s1);
    perror(s2);
}
void read_write(char *src,char *dst)
{
		
     
	DIR		*dir_ptr;		/* the directory */
	struct dirent	*direntp;		/* each entry	 */
    struct stat info;
    int in_fd,out_fd,n_chars;
     if((in_fd = open(src,O_RDONLY)) == -1)
     {
        oops("read error\n ",src); 
     }
	
    if(stat(dst,&info) == -1)
    {
        perror(dst);

    }  
    
    if(S_ISDIR(info.st_mode))  //判断是否为文件夹
    {
        
        strcat(dst,src);
    //    printf("%s\n",dst);
        if((out_fd = creat(dst,COPYMODE)) == -1)
        {
            oops("write error!!!! \n  ",dst);

        }
    }
    else if((out_fd = creat(dst,COPYMODE)) == -1)
    {
        oops("write error!!!! \n  ",dst);

    }
     
     while((n_chars = read(in_fd,buf,BUFFERSIZE)) > 0)
        if(write(out_fd,buf,n_chars) != n_chars)
           oops("write Error to \n ",dst);

     if(n_chars == -1)
        oops("Read error from ",src);

     if(close(in_fd) == -1 || close(out_fd)==-1)
        oops("Error closing file","  ") ;

}









