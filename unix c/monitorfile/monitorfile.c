#include "list.h"
#include "monitorfile.h"
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>

#define MAX_PATH 1024

int g_first = 0;
void sys_err(char *str)
{
    write(1, str, strlen(str));
    exit(1);
}
//创建守护进程
void demon()
{
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        sys_err("fork");
    }
    else if (pid > 0)
    {
        exit(0);
    }
    setsid();

    if (chdir("/tmp/") < 0)
    {
        sys_err("chdir");
    }
    umask(0);

    close(0);
    close(1);
    close(2);
    open("filechangedlog", O_RDWR | O_CREAT, 0666);
    open("errorlog", O_RDWR | O_CREAT, 0666);
}

ListNode* Find(List *lp, char *name)
{
    ListNode *p = lp->head;
    for (; p;p = p->next)
    {
        if (strcmp(name, p->data.name) == 0)
        {
            return p;
        }
    }
    return NULL;
}

void dirwalk(List *lp, char *path, void (*fun)(List *lp, char *name))
{
    char name[MAX_PATH];
    struct dirent *dir;
    DIR *pdir;

    if ( (pdir = opendir(path)) == NULL)
    {
        sys_err("opendir");
    }

    while ( (dir = readdir(pdir)) != NULL )
    {
        //将某些目录和文件剔除
        if (strncmp(dir->d_name, ".", 1) == 0 || strcmp(dir->d_name, "..") == 0)
        {
            continue;
        }
        if (strlen(path) + strlen(dir->d_name) + 2 > sizeof(name))
        {
            sprintf(name, "%s %s too long", path, dir->d_name);
            sys_err(name);
        }
        else
        {
            sprintf(name, "%s/%s", path, dir->d_name);
            (*fun)(lp, name);
        }

    }
    closedir(pdir);
}

void fize(List *lp, char *name)
{
    struct stat buf;
    File data;

    if (stat(name, &buf) < 0)
    {
        sys_err("stat");
    }
    if ((buf.st_mode & S_IFMT) == S_IFDIR)
    {
        dirwalk(lp, name, fize);
    }
    else
    {
        data.mtime = buf.st_mtime;
        strcpy(data.name, name);
        //第一次 直接将节点插入
        if (g_first == 0)
        {
            ListInsertHead(lp, &data);
        }
        else
        {
            ListNode *p = Find(lp, data.name);
            char buffer[1024];
            //返回NULL 不存在 直接插入
            if (p == NULL)
            {
                sprintf(buffer, "%s crate %s\n", data.name, ctime(&data.mtime));
                write(0, buffer, strlen(buffer));
                ListInsertHead(lp, &data);
            }
            else
            {
                //时间被改变
                if (data.mtime != p->data.mtime)
                {
                    sprintf(buffer, "%s changed %s\n", data.name, ctime(&data.mtime));
                    write(0, buffer, strlen(buffer));
                }
            }
        }
    }
}

//检测文件是否存在
void CheckFile(List *lp)
{
    ListNode *p = NULL;
    struct stat buf;
    char buffer[1024];
    time_t t;
    int i = 1;
    for (p = lp->head->next; p; p = p->next, ++i)
    {
        if (stat(p->data.name, &buf) < 0)
        {
            //如果不存在 直接删除节点
            if (errno == ENOENT)
            {
                t = time(NULL);
                sprintf(buffer, "%s delete %s\n", p->data.name, ctime(&t));
                write(0, buffer, strlen(buffer));
                ListDelete(lp, i);
            }
        }
    }
}
void monitorfile()
{
    ListNode *p = NULL;
    List *lp = InitList();
    demon();
    fize(lp, "/home/stucky/");
    g_first = 1;
    while (1)
    {
        sleep(3);
        fize(lp, "/home/stucky/");
        CheckFile(lp);
    }
    ListDestroy(lp);
}
