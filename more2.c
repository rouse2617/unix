#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <term.h>
#include <ncurses.h>

typedef int Status;
int LINELEN,PAGELEN; //全局变量，在GetTermInfo中获得

/*more的具体实现*/
Status DoMore(FILE *);

/*用户命令输入处理*/
Status SeeMore(FILE *);

/*实现回显功能关闭和非标准行处理*/
Status EchoSet(struct termios *,FILE *);

/*恢复初始设置*/
Status EchoBack(struct termios *,FILE *);
/*实现清屏,光标定位*/
Status GetTermInfo();

Status DoMore(FILE *fp)
{
	struct termios *initialrsettings,*newrsettings;
	initialrsettings = (struct termios *)malloc(sizeof(struct termios));
	newrsettings = (struct termios *)malloc(sizeof(struct termios));
	char line[LINELEN];
	int num_of_lines = 0;
	int user_action;
	FILE *fp_tty;
	fp_tty = fopen("/dev/tty","r");
	if(NULL == fp_tty)
	{
	//	exit(1);
	}
	tcgetattr(fileno(fp_tty),initialrsettings);
	*newrsettings = *initialrsettings;//此处不要用指针赋值,否则无法恢复初始设置.
	while(fgets(line,LINELEN,fp))
	{
		if(num_of_lines == PAGELEN)
		{
			EchoSet(newrsettings,fp_tty);
			user_action = SeeMore(fp_tty);
			if(user_action==0)
			{
				EchoBack(initialrsettings,fp_tty); //恢复终端初始设置
				printf("\n");
				break;
			}
			num_of_lines = num_of_lines - user_action;
		}
		if(fputs(line,stdout)==EOF)
		{
		//	exit(1);
		}
		num_of_lines++;

	}
	EchoBack(initialrsettings,fp_tty);//恢复终端初始设置
}

int SeeMore(FILE *cmd)
{
	char c;
	printf("\033[7m more? \033[m");
	while((c=fgetc(cmd))!=EOF)
	{
		if(c == 'q')
		{
			return 0;
		}
		if(c == ' ')
		{
			return PAGELEN;
		}
		if(c == '\n')
		{
			return 1;
		}
	}
	return 0;

}
/*关闭会显.需要termios.h*/
Status EchoSet(struct termios *newrsettings,FILE *fp_tty)
{
	if(NULL == newrsettings||NULL == fp_tty)
	{
		return 1;
	}
	(*newrsettings).c_lflag &= ~ECHO;//关闭回显
	(*newrsettings).c_lflag &= ~ICANON;//以下三行--无需输入回车即可执行用户命令
	(*newrsettings).c_cc[VMIN] = 1;
	(*newrsettings).c_cc[VTIME] = 0;
	tcsetattr(fileno(fp_tty),TCSAFLUSH,newrsettings);
//	free(newrsettings); //运行此行代码出错,因为 tcsetattr调用完毕后自动free.
//	newrsettings = NULL;
	return 0;
}
/*恢复回显功能*/
Status EchoBack(struct termios *initialrsettings,FILE *fp_tty)
{
	if(NULL == initialrsettings||NULL == fp_tty)
	{
		return 1;
	}
	tcsetattr(fileno(fp_tty),TCSANOW,initialrsettings);
	return 0;

}

Status GetTermInfo()
{
	char *clear,*cursor;
	setupterm(NULL,fileno(stdout),(int *)0);
	clear = (char *)tigetstr("clear");
	cursor = (char *)tigetstr("cup");
        PAGELEN = tigetnum("lines")-1;
        LINELEN = tigetnum("cols");
	putp(clear);
	putp(tparm(cursor,0,0));
	return 0;
}

int main(int ac,char *av[])
{
	FILE *fp;
	if(ac == 1)
	{
		GetTermInfo();
		DoMore(stdin);
	}else
	{
		while(--ac)
		{
			if((fp=fopen(*++av,"r"))!=NULL)
			{
				GetTermInfo();
				DoMore(fp);
				fclose(fp);
			}else
			{
			//	exit(1);
			}

		}
		
	}
	return 0;
}

