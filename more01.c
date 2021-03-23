/*************************************************************************
	> File Name: more01.c
	> Author:hrp 
	> Mail: 
	> Created Time: 2021年03月14日 星期日 21时55分30秒
 ************************************************************************/
#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#define PAGELEN 24
#define LINELEN 512
#define QUIT     -1
#define NEXTLINE  1
void domore(FILE *);
int see_more(FILE *cmd);
int main(int ac,char *av[])
{
    FILE *fp;
    if(ac == 1)
        domore(stdin);
    else 
        while(--ac)
        {
            if((fp = fopen(*++av,"r")) != NULL )
            {
                domore(fp);
                fclose(fp);
            }
        }
}
void domore(FILE *fp)
{
    int numLINE = 1;
    int reply;
    char line[LINELEN];
    FILE * fp_tty;
    fp_tty = fopen("/dev/tty","r");
    while(fgets(line,LINELEN,fp))
    {
        if(numLINE == PAGELEN)
        {
           reply = see_more(fp_tty);
           if(reply == 0)
                break;
            
            numLINE-= reply;

        }
        if( fputs(line,stdout) == EOF )
        {
            printf("file is end");
            // exit(1);    
        }
        numLINE++;
    }

}
int see_more( FILE *cmd )
{
    int c ;

    
    printf("\033[7m more ? \033[m   ");
   /* while( (c = getchar()) != EOF )
    {
        if( c == 'q' )
            return 0;
        if( c == ' ' )
            return PAGELEN;
        if( c == '\n' )
            return 1;
        
    }*/
    c = getc(cmd);
   
    
    switch (c)
    {
        case '\n':
            return NEXTLINE;
        case ' ':
            return PAGELEN;
        case 'q':
            return QUIT;
        default:
            return 0;
    };
    return 0;
}
Status


