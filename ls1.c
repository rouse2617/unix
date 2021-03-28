/*************************************************************************
	> File Name: ls1.c
	> Author:hrp 
	> Mail: 
	> Created Time: 2021年03月26日 星期五 19时48分17秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
void do_ls(char []);
void show_stat(char *name,struct stat *buf);
void mode_to_letters(int mode, char str[]);
char *uid_to_name(uid_t uid);
char *gid_to_name(gid_t gid);

int main(int ac, char *av[])
{
	if ( ac == 1 )
		do_ls( "." );
	else
		while ( --ac ){
			printf("%s:\n", *++av );
			do_ls( *av );
		}
}

void do_ls( char dirname[] )
/*
 *	list files in directory called dirname
 */
{
	DIR		*dir_ptr;		/* the directory */
	struct dirent	*direntp;		/* each entry	 */
    struct stat info;
	if ( ( dir_ptr = opendir( dirname ) ) == NULL )
		fprintf(stderr,"ls1: cannot open %s\n", dirname);
	else
	{
		while ( ( direntp = readdir( dir_ptr ) ) != NULL )
        {   
            show_stat(direntp->d_name,&info);
        }
		closedir(dir_ptr);
	}
}
void show_stat(char *name,struct stat *buf)
{
    if(stat(name,buf) ==  -1)
    perror(name);
    
    char mode_buf[11];
    mode_to_letters(buf->st_mode,mode_buf);
    printf("%s\t",mode_buf);
    printf("%ld\t",buf->st_nlink);
    printf("%s\t",uid_to_name(buf->st_uid));
    printf("%s\t",gid_to_name(buf->st_gid));
    printf("%ld\t",buf->st_size);
    printf("%ld\t",buf->st_mtime);
    printf("%.12s\t", 4+ctime(&buf->st_mtime));
    printf("%s\t\n",name);
   
}

void mode_to_letters(int mode, char str[])
{
	strcpy(str, "----------");		// default = no perms

	if(S_ISDIR(mode)) str[0] = 'd';		// directory?
	if(S_ISCHR(mode)) str[0] = 'c';		// char devices
	if(S_ISBLK(mode)) str[0] = 'b';		// block device

	if(mode & S_IRUSR) str[1] = 'r';	// 3 bits for user
	if(mode & S_IWUSR) str[2] = 'w';
	if(mode & S_IXUSR) str[3] = 'x';

	if(mode & S_IRGRP) str[4] = 'r';	// 3 bits for group
	if(mode & S_IWGRP) str[5] = 'w';
	if(mode & S_IXGRP) str[6] = 'x';

	if(mode & S_IROTH) str[7] = 'r';	// 3 bits for other
	if(mode & S_IWOTH) str[8] = 'w';
	if(mode & S_IXOTH) str[9] = 'x';

}
char *uid_to_name(uid_t uid)
{
    struct passwd * pw_ptr;
    static char numstr[10];
    if((pw_ptr = getpwuid(uid)) == NULL)
    {
        sprintf(numstr,"%d",uid);
    }else
        return getpwuid(uid)->pw_name;
}
char *gid_to_name(gid_t gid)
{
    struct group *getgrgid(), *grp_ptr;
	static char numstr[10];

	if((grp_ptr = getgrgid(gid)) == NULL){
		sprintf(numstr, "%d", gid);
		return numstr;
	}
	else
		return grp_ptr->gr_name;
}


