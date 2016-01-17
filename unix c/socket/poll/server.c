#include "wrap.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ctype.h>
#include <poll.h>

#define SERVER_PORT 8000
#define MAXLEN 4096
#define OPEN_MAX 1024

int main(void)
{
    struct sockaddr_in serveraddr, clientaddr;
    int sockfd, confd, len, i = 0, maxi, nready;
    char ipstr[INET_ADDRSTRLEN], buf[MAXLEN];
    struct pollfd client[OPEN_MAX];
    socklen_t addrlen;
    //1.socket
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    //2.bind
    bzero(&serveraddr, sizeof(serveraddr));
    //地址族协议IPv4
    serveraddr.sin_family = AF_INET;
    //IP地址
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVER_PORT);
    Bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    //3.listen
    Listen(sockfd, 128);

    client[0].fd = sockfd;
    client[0].events = POLLRDNORM;
    maxi = 0;


    for (i = 1; i < OPEN_MAX ;++i)
    {
        client[i].fd = -1;
    }

    while (1)
    {
        nready = poll(client, maxi+1, -1);
        if (nready < 0)
        {
            perr_exit("select error");
        }
        if (client[0].revents & POLLRDNORM)
        {
            addrlen = sizeof(clientaddr);
            confd = Accept(sockfd, (struct sockaddr *)&clientaddr, &addrlen);
            //输出客户端IP地址和端口号
            inet_ntop(AF_INET, &clientaddr.sin_addr, ipstr, sizeof(ipstr));
            printf("client ip = %s\tclient port = %d\n", ipstr, ntohs(clientaddr.sin_port));

            for (i = 0; i < OPEN_MAX; ++i)
            {
                if (client[i].fd < 0)
                {
                    client[i].fd = confd;
                    break;
                }
            }
            if (i == FD_SETSIZE)
            {
                fputs("too many clients\n", stderr);
            }

            client[i].events = POLLRDNORM;
            if (i > maxi)
            {
                maxi = i;
            }

            if (--nready == 0)
            {
                continue;
            }
        }

        for (i = 1; i <= maxi; ++i)
        {
            if ((confd = client[i].fd) < 0)
            {
                continue;
            }
            if (client[i].revents & POLLRDNORM)
            {
                //5.处理客户端请求
                len = Read(confd, buf, sizeof(buf));
                if (len == 0)
                {
                    //当client关闭链接的时候，关闭链接 解除监控此文件描述符
                    Close(confd);
                    client[i].fd = -1;
                }
                else
                {
                    i = 0;
                    while (i < len)
                    {
                        buf[i] = toupper(buf[i]);
                        ++i;
                    }
                    Write(confd, buf, strlen(buf));
                }
                if (--nready == 0)
                {
                    break;
                }
            }
        }
    }
    close(sockfd);
    return 0;
}
