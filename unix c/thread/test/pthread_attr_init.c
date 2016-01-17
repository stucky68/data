#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

void *th_fun(void *arg)
{
    int n = 10;
    while (n--)
    {
        printf("%x  %d\n", (unsigned int)pthread_self(), n);
        sleep(1);
    }
    return (void *)1;
}


int main()
{
    pthread_t tid;
    pthread_attr_t attr;
    int err;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&tid, &attr, th_fun, NULL);

    err = pthread_join(tid, NULL);
    while (1)
    {
        if (err != 0)
        {
            printf("%s\n", strerror(err));
            pthread_exit((void *)1);
        }
    }

    pthread_attr_destroy(&attr);
    return 0;
}
