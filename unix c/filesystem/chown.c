#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int agrc, char *agrv[])
{
    int uid = 0;
    int gid = 0;
    if (agrc < 4)
    {
        printf("./mychown uid gid");
        exit(1);
    }
    uid = atoi(agrv[2]);
    gid = atoi(agrv[3]);
    if (chown(agrv[1], uid, gid) > 0)
    {
        perror("chown");
        exit(1);
    }
    return 0;
}
