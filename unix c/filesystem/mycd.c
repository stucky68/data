#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
    char buf[1024];
    if (argc < 2)
    {
        printf("./mycp dir");
    }
    if (strcmp(argv[1], "~") == 0)
    {
        if (chdir(getenv("HOME")) > 0)
        {
            perror("chdir");
        }
    }
    else
    {
        if (chdir(argv[1]) > 0)
        {
            perror("chdir");
        }
    }
    printf("%s\n", getcwd(buf, 1024));
    printf("%s\n", buf);
    return 0;
}
