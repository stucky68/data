#include "Packet.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

void sys_err(char *str, int exitno)
{
    perror(str);
    exit(exitno);
}

//登录
int Login(int fd, char *LoginName)
{
    Packet packet;
    packet.id = 1;
    strcpy(packet.src, LoginName);
    strcpy(packet.dest, "server");
    strcpy(packet.data, "Login");
    if (write(fd, &packet, sizeof(Packet)) < 0)
    {
        sys_err("Login", 1);
    }
    return 0;
}

//退出
void Logout(int fd, char *LoginName)
{
    Packet packet;
    packet.id = 3;
    strcpy(packet.src, LoginName);
    strcpy(packet.dest, "0");
    sprintf(packet.data, "%s Logout\n", LoginName);
    if (write(fd, &packet, sizeof(Packet)) < 0)
    {
        sys_err("Logout", 1);
    }
    exit(1);
}

//从标准输入读取要发送的内容 发送到公用管道
void send(int fd, char *LoginName)
{
    Packet packet;
    char buf[105] = {0};
    int len;
    len = read(STDIN_FILENO, buf, 105);
    if (len < 0)
    {
        if (errno == EAGAIN)
        {
            return;
        }
    }
    else
    {
        //如果等于exit则退出
        if (strncmp(buf, "exit", 4) == 0)
        {
            Logout(fd, LoginName);
        }
        //要发送的内容 格式为: xxxx:data
        packet.id = 2;
        strcpy(packet.src, LoginName);
        strncpy(packet.dest, buf, 4);
        strncpy(packet.data, buf+5, 100);
        if (write(fd, &packet, sizeof(Packet)) < 0)
        {
            sys_err("send failed", 1);
        }
    }
}

//从私有管道中接收服务器传来的信息
void recv(int fd)
{
    Packet packet;
    int len;
    len = read(fd, &packet, sizeof(Packet));
    if (len < 0)
    {
        if (errno == EAGAIN)
        {
            return;
        }
    }
    else
    {
       printf("%s:%s", packet.dest, packet.data);
    }
}

void work(int fd, char *LoginName)
{
    int myfd, flags, readfd;
    //等待打开自己的私有管道
    while (1)
    {
        myfd = open(LoginName, O_RDWR);
        if (myfd > 0)
        {
            break;
        }
    }
    //设置标准输入为非阻塞
    flags = fcntl(STDIN_FILENO, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(STDIN_FILENO, F_SETFL, flags);
    //将FIFO管道设置为非阻塞
    flags = fcntl(myfd, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(myfd, F_SETFL, flags);
    while (1)
    {
        send(fd, LoginName);
        recv(myfd);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Client LoginName\n");
        exit(1);
    }
    int fd;
    fd = open("serverfifo", O_RDWR);
    if (fd < 0)
    {
        sys_err("open", 1);
    }
    Login(fd, argv[1]);
    work(fd, argv[1]);
    close(fd);
    return 0;
}
