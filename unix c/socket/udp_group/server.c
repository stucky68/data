#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include <net/if.h>

#define SERVER_PORT 8000
#define CLIENT_PORT 9000
#define MAXLEN 1024
#define GROUP "239.0.0.2"

int main()
{
    struct sockaddr_in serveraddr, clientaddr;
    int sockfd, i;
    char buf[MAXLEN];
    socklen_t clientlen;
    struct ip_mreqn group;

    //构造用于udp通信的套接字
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVER_PORT);

    bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    //设置组地址
    inet_pton(AF_INET, GROUP, &group.imr_multiaddr);
    //本地任意ip
    inet_pton(AF_INET, "0.0.0.0", &group.imr_address);
    //eth0 --> 编号  命令:ip ad
    group.imr_ifindex = if_nametoindex("eth0");

    setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF, &group, sizeof(group));

    //构造 client IP+端口号
    bzero(&clientaddr, sizeof(clientaddr));
    clientaddr.sin_family = AF_INET;
    inet_pton(AF_INET, GROUP, &clientaddr.sin_addr);
    clientaddr.sin_port = htons(CLIENT_PORT);
    while (1)
    {
        fgets(buf, sizeof(buf), stdin);
        sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&clientaddr, sizeof(clientaddr));
    }

    close(sockfd);
    return 0;
}
