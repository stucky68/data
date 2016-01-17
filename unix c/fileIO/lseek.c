#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    int fd = open("abc", O_RDWR);
    if (fd < 0)
    {
        perror("open abc");
        exit(-1);
    }
    lseek(fd, 0x1000, SEEK_SET);
    write(fd, "H", 1);
    close(fd);

    //获取文件大小
    fd = open("hello", O_RDWR);
    if (fd < 0)
    {
        perror("open abc");
        exit(-1);
    }
    printf("hello size = %d\n", lseek(fd, 0, SEEK_END));
    close(fd);
    return 0;
}
