#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

void sys_err(char *str, int exitno)
{
    perror(str);
    exit(exitno);
}

int main(int argc, char *argv[])
{
    int fd;
    char buf[1024] = "Hell xwp\n";
    if (argc < 2)
    {
        printf("./a.out fifoname\n");
        exit(1);
    }
    fd = open(argv[1], O_WRONLY);
    if (fd < 0)
    {
        sys_err("open", 1);
    }
    write(fd, buf, strlen(buf));
    close(fd);
    return 0;
}
