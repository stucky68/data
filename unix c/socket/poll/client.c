#include "wrap.h"
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVER_PORT 8000
#define MAXLEN 4096

int ConnectSerevr()
{
    struct sockaddr_in serveraddr;
    int confd;
    char ipstr[] = "192.168.153.128", buf[4096];
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

    confd = ConnectSerevr();
    while (1)
    {
        fgets(buf, sizeof(buf), stdin);
        Write(confd, buf, strlen(buf));
        len = Read(confd, buf, sizeof(buf));
        Write(STDOUT_FILENO, buf, len);
    }
    //5.关闭socket
    Close(confd);
    return 0;
}
