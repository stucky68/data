#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int agrc, char *agrv[])
{
    int mode = 0;
    if (agrc < 3)
    {
        printf("./mychmod mode filename");
        exit(1);
    }
    mode = atoi(agrv[1]);
    if (chmod(agrv[2], mode))
    {
        perror("chmod");
        exit(1);
    }
    return 0;
}
