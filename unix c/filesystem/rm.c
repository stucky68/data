#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <error.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

#define MAX_PATH 1024

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
        if (strcmp(pdir->d_name, ".") == 0 || strcmp(pdir->d_name, "..") == 0)
        {
            continue;
        }
        if ( (strlen(dir) + strlen(pdir->d_name) + 2) > MAX_PATH)
        {
            printf("PATH too long");
        }
        else
        {
            sprintf(name, "%s/%s", dir, pdir->d_name);
            (*fnc)(name);
        }
    }
    closedir(pd);
    rmdir(dir);
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
    unlink(name);
}

int main(int argc, char **agrv)
{
    if (argc == 1)
    {
        fsize(".");
    }
    while (--argc)
    {
        fsize(*++agrv);
    }
    return 0;
}
