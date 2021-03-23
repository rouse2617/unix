/*************************************************************************
	> File Name: who1.c
	> Author:hrp 
	> Mail: 
	> Created Time: 2021年03月21日 星期日 03时24分12秒
 ************************************************************************/

#include<stdio.h>
#include<utmp.h>
#include<time.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>


void showtime(long timeval);
void show_info(struct utmp * utbufp);
int main()
{
    struct utmp current_rcord;
    int utmpfd;
    
    int reclen = sizeof(current_rcord);
    
    
    if((utmpfd = open(UTMP_FILE,O_RDONLY)) == -1)
    {
        perror(UTMP_FILE); 
  //      exit(1);
    }
    while( read(utmpfd,&current_rcord,reclen) == reclen )
        show_info(&current_rcord);
    close(utmpfd);
    return 0;
}
void show_info(struct utmp * utbufp)
{
    printf("%-8.8s ",utbufp->ut_name);
    printf("%-8.8s ",utbufp->ut_line);
  //printf("%-8.8d ",utbufp->ut_time);
    showtime(utbufp->ut_time);

    printf("\n");
}
void showtime(long timeval)
{
    char *p;
    p = ctime(&timeval);
    printf("%12.12s ",p+4);
}
