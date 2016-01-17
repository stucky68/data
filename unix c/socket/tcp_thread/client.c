#include "wrap.h"
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>

#define SERVER_PORT 8000
#define MAXLEN 4096

int g_flag = 1;

void *do_test(void *arg)
{
    int *fd = (int *)arg;
    int count = 0;
    ssize_t len = 0;
    while (1)
    {
        if (write(*fd, "xt", strlen("xt")) < 0)
        {
            ++count;
        }
        else
        {
            count = 0;
        }
        if (count == 5)
        {
            printf("Disconneted from the server\n");
            g_flag = 0;
            write(STDIN_FILENO, "1", strlen("1"));
            sleep(3);
            write(*fd, "1", strlen("1"));
            break;
        }
        sleep(1);
    }
}

int ConnectSerevr()
{
    struct sockaddr_in serveraddr;
    int confd;
    char ipstr[] = "192.168.95.134", buf[4096];
    //1.创建一条socket
    confd = socket(AF_INET, SOCK_STREAM, 0);
    //2.初始化服务器地址
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, ipstr, &serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVER_PORT);
    //3.链接服务器
    Connect(confd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    return confd;
}

int main(int argc, char *argv[])
{
    int len, confd;
    char buf[MAXLEN];
    sigset_t mask;
    pthread_t tid;
    pthread_attr_t attr;

    //屏蔽信号
    sigemptyset(&mask);
    sigaddset(&mask, SIGPIPE);
    pthread_sigmask(SIG_BLOCK, &mask, NULL);
    //初始化线程属性
    pthread_attr_init(&attr);
    //设置为分离态
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    confd = ConnectSerevr();
    pthread_create(&tid, &attr, do_test, &confd);
    while (g_flag)
    {
        Read(STDIN_FILENO, buf, sizeof(buf));
        printf("%s\n", buf);
        Write(confd, buf, strlen(buf));
        len = Read(confd, buf, sizeof(buf));
        Write(STDOUT_FILENO, buf, len);
    }
    //5.关闭socket
    Close(confd);
    pthread_attr_destroy(&attr);
    return 0;
}
