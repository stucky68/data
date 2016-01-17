#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>
#include <stdlib.h>

int main()
{
    MYSQL mysql;
    MYSQL *connect;

    connect = mysql_init(&mysql);
    if (connect == NULL)
    {
        printf("init error\n");
        exit(-1);
    }

    connect = mysql_real_connect(connect, "localhost", "root", "jiangxin", "mydb61", 0, NULL, 0);
    if (connect == NULL)
    {
        printf("connect error\n");
        exit(-1);
    }

    mysql_close(&mysql);
    return 0;
}
