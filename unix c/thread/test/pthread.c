#include <pthread.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>


void *th_fun(void *arg)
{
    int *p = (int *)arg;
    printf("thread PID = %x\n", getpid());
    printf("thread ID = %x\n", (unsigned int)pthread_self());
    printf("thread *arg = %d\n", *p);
    while(1);

}

int main(void)
{
    pthread_t tid;
    int n = 10;
    pthread_create(&tid, NULL, th_fun, (void *)&n);
    printf("main thread ID = %x\n", (unsigned int)pthread_self());
    printf("main chiled thread ID = %x\n", (unsigned int)tid);
    printf("main PID = %x\n", getpid());
    while(1);
    return 0;
}
