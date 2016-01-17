#include "wrap.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <netinet/in.h>

#define SERVER_PORT 8000
#define MAXLEN 1024

typedef struct
{
    struct sockaddr_in addr;
    int confd;
}s_info;


void *do_work(void *arg)
{
    s_info *ts = (s_info *)arg;
    char buf[MAXLEN], ipstr[INET_ADDRSTRLEN];
    int len, i = 0;
    while (1)
    {
        len = Read(ts->confd, buf, sizeof(buf));
        if (strcmp(buf, "xt") == 0)
        {
            continue;
        }
        if (len == 0)
        {
            printf("close client");
            break;
        }
        inet_ntop(AF_INET, &ts->addr.sin_addr, ipstr, sizeof(ipstr));
        printf("client ip = %s\t client port = %d\n", ipstr, ntohs(ts->addr.sin_port));

        while (i < len)
        {
            buf[i] = toupper(buf[i]);
            ++i;
        }
        Write(ts->confd, buf, strlen(buf));
    }
    Close(ts->confd);
}

int main(void)
{
    struct sockaddr_in serveraddr, clientaddr;
    s_info clientlist[100];
    pthread_t tid;
    pthread_attr_t attr;
    int serverfd, clientfd, i = 0;
    socklen_t clientlen;
    //初始化线程属性
    pthread_attr_init(&attr);
    //设置为分离态
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    //创建服务器的socket
    serverfd = Socket(AF_INET, SOCK_STREAM, 0);
    //绑定服务器IP和端口
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVER_PORT);
    Bind(serverfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    //listen
    Listen(serverfd, SOCK_STREAM);
    while (1)
    {
        //accept
        clientlen = sizeof(clientaddr);
        clientfd = Accept(serverfd, (struct sockaddr *)&clientaddr, &clientlen);
        clientlist[i].confd = clientfd;
        clientlist[i].addr = clientaddr;
        pthread_create(&tid, &attr, do_work, &clientlist[i]);
        ++i;
    }
    Close(serverfd);
    pthread_attr_destroy(&attr);
    return 0;
}
