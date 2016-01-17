#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <net/if.h>

#define SERVER_PORT 8000
#define MAXLEN 4096
#define CLIENT_PORT 9000
#define GROUP "239.0.0.2"

int main(int argc, char *argv[])
{
    struct sockaddr_in localaddr;
    int confd;
    ssize_t len;
    char buf[MAXLEN];
    struct ip_mreqn group;
    //1.创建一条socket
    confd = socket(AF_INET, SOCK_DGRAM, 0);
    //2.初始化本地地址
    bzero(&localaddr, sizeof(localaddr));
    localaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "0.0.0.0", &localaddr.sin_addr);
    localaddr.sin_port = htons(CLIENT_PORT);

    bind(confd, (struct sockaddr *)&localaddr, sizeof(localaddr));

    inet_pton(AF_INET, GROUP, &group.imr_multiaddr);
    inet_pton(AF_INET, "0.0.0.0", &group.imr_address);
    group.imr_ifindex = if_nametoindex("eth0");

    //设置client 加入多播组
    setsockopt(confd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(group));


    //3.请求服务器处理数据
    while (1)
    {
        len = recvfrom(confd, buf, sizeof(buf), 0,  NULL, 0);
        write(STDOUT_FILENO, buf, len);
    }
    //4.关闭socket
    close(confd);
    return 0;
}
