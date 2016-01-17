#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>

#define SERVER_PORT 8000
#define MAXLEN 1024

int main()
{
    struct sockaddr_in serveraddr, clientaddr;
    int sockfd, i;
    char buf[MAXLEN], ipstr[INET_ADDRSTRLEN];
    socklen_t clientlen;
    ssize_t len;

    //构造用于udp通信的套接字
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVER_PORT);

    bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    while (1)
    {
        clientlen = sizeof(clientaddr);
        len = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&clientaddr, &clientlen);
        inet_ntop(AF_INET, &clientaddr.sin_addr, ipstr, sizeof(ipstr));
        printf("client IP %s\tPORT%d\n", ipstr, ntohs(clientaddr.sin_port));
        i = 0;
        while (i < len)
        {
            buf[i] = toupper(buf[i]);
            ++i;
        }
        sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&clientaddr, clientlen);
    }

    close(sockfd);
    return 0;
}
