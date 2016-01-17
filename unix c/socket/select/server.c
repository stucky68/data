#include "wrap.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ctype.h>
#include <sys/select.h>

#define SERVER_PORT 8000
#define MAXLEN 4096

int main(void)
{
    struct sockaddr_in serveraddr, clientaddr;
    int sockfd, confd, len, i = 0, maxfd, maxi, nready;
    char ipstr[INET_ADDRSTRLEN], buf[MAXLEN];
    int client[FD_SETSIZE];
    fd_set allset, rset;
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

    maxfd = sockfd;
    maxi = -1;

    for (i = 0; i < FD_SETSIZE; ++i)
    {
        client[i] = -1;
    }

    FD_ZERO(&allset);
    FD_SET(sockfd, &allset);

    while (1)
    {
        rset = allset;
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);
        if (nready < 0)
        {
            perr_exit("select error");
        }
        if (FD_ISSET(sockfd, &rset))
        {
            addrlen = sizeof(clientaddr);
            confd = Accept(sockfd, (struct sockaddr *)&clientaddr, &addrlen);
            //输出客户端IP地址和端口号
            inet_ntop(AF_INET, &clientaddr.sin_addr, ipstr, sizeof(ipstr));
            printf("client ip = %s\tclient port = %d\n", ipstr, ntohs(clientaddr.sin_port));

            for (i = 0; i < FD_SETSIZE; ++i)
            {
                if (client[i] < 0)
                {
                    client[i] = confd;
                    break;
                }
            }
            if (i == FD_SETSIZE)
            {
                fputs("too many clients\n", stderr);
            }
            //将一个新的文件描述符添加到监控信号集
            FD_SET(confd, &allset);
            if (confd > maxfd)
            {
                maxfd = confd;
            }
            if (i > maxi)
            {
                maxi = i;
            }

            if (--nready == 0)
            {
                continue;
            }
        }

        for (i = 0; i <= maxi; ++i)
        {
            if ((confd = client[i]) < 0)
            {
                continue;
            }
            if (FD_ISSET(confd, &rset))
            {
                //和客户端交互数据操作confd
                //5.处理客户端请求
                len = Read(confd, buf, sizeof(buf));
                if (len == 0)
                {
                    //当client关闭链接的时候，关闭链接 解除监控此文件描述符
                    Close(confd);
                    FD_CLR(confd, &allset);
                    client[i] = -1;
                }
                else
                {
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
