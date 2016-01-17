#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <error.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

#define MAX_PATH 1024

int i = 0;
void dirwalk(char *dir, void(*fnc)(char *))
{
    DIR *pd;
    struct dirent *pdir;
    char name[MAX_PATH];


    if ((pd = opendir(dir)) == NULL)
    {
        perror("opendir");
    }

    while ((pdir = readdir(pd)) != NULL)
    {
        //判断是否为当前目录或者上级目录
        if (strcmp(pdir->d_name, ".") == 0 || strcmp(pdir->d_name, "..") == 0 )
        {
            continue;
        }
        if ( (strlen(dir) + strlen(pdir->d_name) + 2) > MAX_PATH)
        {
            printf("PATH too long");
        }
        else
        {
            //将目录添加到文件名中
            sprintf(name, "%s/%s", dir, pdir->d_name);
            //回调函数
            (*fnc)(name);
        }
    }
    closedir(pd);

}

void fsize(char *name)
{
    struct stat buf;

    if (stat(name, &buf) > 0)
    {
        perror("stat");
    }

    if ((buf.st_mode & S_IFMT) == S_IFDIR)
        dirwalk(name, fsize);
    //打印文件的大小和名称
    //printf("%8ld %s\n", buf.st_size, name);
     else   ++i;

}

int main(int argc, char **agrv)
{
    if (argc == 1)
    {
        fsize("/home/");
        printf("%d\n", i);
    }
    while (--argc)
    {
        fsize(*++agrv);
    }
    return 0;
}
