#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ctype.h>

#define SERVER_PORT 8000
#define MAXLEN 4096

int main(void)
{
    struct sockaddr_in serveraddr, clientaddr;
    int sockfd, addrlen, confd, len, i = 0;
    char ipstr[128], buf[MAXLEN];
    //1.socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //2.bind
    bzero(&serveraddr, sizeof(serveraddr));
    //地址族协议IPv4
    serveraddr.sin_family = AF_INET;
    //IP地址
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVER_PORT);
    bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    //3.listen
    listen(sockfd, 128);
    //4.accept
    while (1)
    {
        addrlen = sizeof(clientaddr);
        confd = accept(sockfd, (struct sockaddr *)&clientaddr, &addrlen);
        //输出客户端IP地址和端口号
        inet_ntop(AF_INET, &clientaddr.sin_addr, ipstr, sizeof(ipstr));
        printf("client ip = %s\tclient port = %d\n", ipstr, ntohs(clientaddr.sin_port));
        //和客户端交互数据操作confd
        //5.处理客户端请求
        len = read(confd, buf, sizeof(buf));
        while (i < len)
        {
            buf[i] = toupper(buf[i]);
            ++i;
        }
        write(confd, buf, strlen(buf));
        close(confd);
    }
    close(sockfd);
    return 0;
}
