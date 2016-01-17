#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

int main(void)
{
    int fd[2];
    char str[1024] = "Hello World";
    pid_t pid;

    //fd[0] 读端
    //fd[1] 写端
    if (pipe(fd) < 0)
    {
        perror("pipe");
        exit(1);
    }
    pid = fork();

    if (pid > 0)
    {
       //in parent
       close(fd[0]);
       sleep(5);
       write(fd[1], str, strlen(str));
       wait(NULL);
       close(fd[1]);
    }
    else if (pid == 0)
    {
        int flags, len;
        close(fd[1]);
        flags = fcntl(fd[0], F_GETFL);
        flags |= O_NONBLOCK;
        fcntl(fd[0], F_SETFL, flags);
tryagain:
        len = read(fd[0], str, sizeof(str));
        if (len == -1)
        {
            if (errno == EAGAIN)
            {
                write(STDOUT_FILENO, "try again\n", 10);
                sleep(1);
                goto tryagain;
            }
            else
            {
                perror("read");
                exit(1);
            }
        }
        write(STDOUT_FILENO, str, strlen(str));
        close(fd[0]);
    }
    else
    {
        perror("fork");
        exit(1);
    }
    return 0;
}
