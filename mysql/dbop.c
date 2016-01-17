#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>
#include <stdlib.h>

#define LENGTH 1024

int main()
{
    MYSQL mysql;
    MYSQL *connect;
    unsigned int i = 0, linelength = 0;
    unsigned int field_num;
    unsigned long *length;
    MYSQL_RES *result;
    MYSQL_FIELD *field;
    char buf[1024];
    //初始化
    connect = mysql_init(&mysql);
    if (connect == NULL)
    {
        printf("init error\n");
        exit(-1);
    }

    //连接服务器
    connect = mysql_real_connect(connect, "localhost", "root", "jiangxin", "mydb61", 0, NULL, 0);
    if (connect == NULL)
    {
        printf("connect error\n");
        exit(-1);
    }
    //执行查询
    printf("请输入SQL语句:");
    fgets(buf, LENGTH, stdin);
    if (mysql_query(&mysql, buf));
    {
        printf("%s\n", mysql_error(&mysql));
    }

    result = mysql_use_result(&mysql);

    //获取字段名
    while ((field = mysql_fetch_field(result)))
    {
    //    printf("%d\n", field->max_length);
        printf("%-10s  ",field->name);
    }
    printf("\n");
    MYSQL_ROW row = mysql_fetch_row(result);
    while ((row = mysql_fetch_row(result)))
    {
        field_num = mysql_num_fields(result);
        length = mysql_fetch_lengths(result);
        for (i = 0; i < field_num; ++i)
        {
            printf("%-10s  ",row[i]);
        }
        printf("\n");
        ++linelength;
    }
    printf("共%d行\n", linelength);
    //释放结果集的内存
    mysql_free_result(result);
    mysql_close(&mysql);
    return 0;
}
