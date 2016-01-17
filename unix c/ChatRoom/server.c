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

int find(char *Name, char LoginList[100][5], int LoginNum)
{
    int i = 0;
    for (; i < LoginNum; ++i)
    {
        if (strcmp(LoginList[i], Name) == 0)
        {
            return i;
        }
    }
    return -1;
}

void send(const Packet *packet)
{
    int fd;
    fd = open(packet->dest, O_RDWR);
    printf("%s send\n", packet->dest);
    if (fd < 0)
    {
        sys_err("send", 1);
    }
    if (write(fd, packet, sizeof(Packet)) < 0)
    {
        sys_err("write", 1);
    }
}

void recv(int fd)
{
    Packet packet;
    int len, LoginNum = 0, i, res;
    char LoginList[100][5];
    while (1)
    {
        len = read(fd, &packet, sizeof(Packet));
        //大于0 说明读取成功
        if (len > 0)
        {
            switch(packet.id)
            {
                //一号为登录包
                case 1:
                    strcpy(LoginList[LoginNum++], packet.src);
                    if (mkfifo(packet.src, 0777) > 0)
                    {
                        sys_err(packet.src, 3);
                    }
                    printf("%s Login\n", packet.src);
                    break;
                //二号为聊天包
                case 2:
                    if (find(packet.dest, LoginList, LoginNum) == -1)
                    {
                        Packet temp;
                        temp.id = 3;
                        strcpy(temp.src,"ser");
                        strcpy(temp.dest, packet.src);
                        sprintf(temp.data, "%s is not online", packet.dest);
                        send(&temp);
                    }
                    send(&packet);
                    break;
                //三号为退出包
                case 3:
                    res = find(packet.src, LoginList, LoginNum);
                    if (res >= 0)
                    {
                        LoginNum--;
                        for (i = res; i < LoginNum; ++i)
                        {
                            strcpy(LoginList[i], LoginList[i+1]);
                        }
                        printf("%s Logout\n", packet.src);
                    }
                    break;
            }
        }
    }
}

int main()
{
    int fd;
    if (mkfifo("serverfifo", 0666) > 0)
    {
        sys_err("mkfifo", 1);
    }
    fd = open("serverfifo", O_RDWR);
    if (fd < 0)
    {
        sys_err("open", 2);
    }
    recv(fd);
    close(fd);
     return 0;
}
