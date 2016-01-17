#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define SIZE 0X100000

int print_ntimes(char *str)
{
    //printf("%s\n", str);
    sleep(1);
    return 0;
}

void *th_fun(void *arg)
{
    int n = 3;
    while (n--)
    {
        print_ntimes("hello jx\n");
    }
}

int main()
{
    pthread_t tid;
    pthread_attr_t attr;
    int err, detachstate, i = 1;
    size_t stacksize;
    void *stackaddr;

    pthread_attr_init(&attr);
    pthread_attr_getstack(&attr, &stackaddr, &stacksize);
    //获取默认栈的地址和大小
    printf("stackaddr = %p stacksize = %x\n", stackaddr, (int)stacksize);
    pthread_attr_getdetachstate(&attr, &detachstate);
    if (detachstate == PTHREAD_CREATE_DETACHED)
    {
        printf("thread detached\n");
    }
    else if (detachstate == PTHREAD_CREATE_JOINABLE)
    {
        printf("thread join\n");
    }
    else
    {
        printf("thread unkown\n");
    }
    //设置线程分离属性
    //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    while (1)
    {
        //在堆上申请一块1M的空间，设置线程栈的起始地址和大小
        stackaddr = malloc(SIZE);
        if (stackaddr == NULL)
        {
            perror("malloc");
            exit(1);
        }
        stacksize = SIZE;
        pthread_attr_setstack(&attr, stackaddr, stacksize);
        err = pthread_create(&tid, &attr, th_fun, NULL);
        if (err != 0)
        {
            printf("%s\n", strerror(err));
            exit(1);
        }
        printf("%d\n", i++);
    }
    pthread_attr_destroy(&attr);

    return 0;
}
