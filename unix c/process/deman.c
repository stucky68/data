#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

void daemonize(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        perror("fork");
        exit(1)
    }
    else if (pid > 0)
    {
        exit(0);
    }

    setsid();

    if (chdir("/" < 0))
    {
        perror("chdir");
        exit(1);
    }

    umask(0);

    close(0);
    open("/dev/null", O_RDWR);
    dup2(0, 1);
    dup2(0, 2);
}
int main(void)
{
    daemonize();
    while (1);
}
