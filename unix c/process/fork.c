#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main(void)
{
    pid_t pid;

    pid = fork();

    if (pid > 0)
    {
        while(1)
        {
            printf("I am parent\n");
            printf("my pid = %d\n", getpid());
            printf("my parent pid = %d\n", getppid());
            sleep(1);
        }
    }
    else if (pid == 0)
    {
        while(1)
        {
            printf("I am child\n");
            printf("my pid = %d\n", getpid());
            printf("my parent pid = %d\n", getppid());
            sleep(1);
        }
    }
    else
    {
        perror("fork");
        exit(1);
    }
    return 0;
}
