#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>
#include <stdlib.h>

#define LENGTH 1024

int process_result_set(MYSQL *mysql, MYSQL_RES *result)
{
    unsigned int i = 0;
    unsigned int field_num;
    //获取字段名
    field_num = mysql_field_count(mysql);
    MYSQL_FIELD *fields = mysql_fetch_fields(result);
    for (i = 0; i < field_num; ++i)
    {
        printf("%-10s  ", fields[i].name);
    }
    printf("\n");
    MYSQL_ROW row = NULL;
    while ((row = mysql_fetch_row(result)))
    {
        field_num = mysql_num_fields(result);
        for (i = 0; i < field_num; ++i)
        {
            printf("%-10s  ",row[i]);
        }
        printf("\n");
    }
}

int main()
{
    int status = 0;
    MYSQL *mysql;
    MYSQL *connect;
    MYSQL_RES *result;
    //初始化
    mysql = mysql_init(NULL);
    if (mysql == NULL)
    {
        printf("init error\n");
        exit(-1);
    }

    //连接服务器
    connect = mysql_real_connect(mysql, "localhost", "root", "jiangxin", "mydb61", 0, NULL, CLIENT_MULTI_STATEMENTS);
    if (connect == NULL)
    {
        printf("connect error\n");
        exit(-1);
    }
    //执行多语句查询
    status = mysql_query(mysql, "DROP TABLE IF EXISTS test_table;\
            CREATE TABLE test_table(id int);\
            INSERT INTO test_table values(10);\
            UPDATE test_table SET id = 20 WHERE id = 10;\
            SELECT * FROM test_table;\
            DROP TABLE test_table");
    if (status)
    {
        printf("could not execute statement(s)\n");
        mysql_close(mysql);
        exit(1);
    }
    do
    {
        result = mysql_store_result(mysql);
        if (result) //select语句
        {
            process_result_set(mysql, result);
            mysql_free_result(result);
        }
        else
        {
            if (mysql_field_count(mysql) == 0)
            {
                printf("%lld rows affected\n", mysql_affected_rows(mysql));
            }
            else
            {
                printf("could not retrieve result set\n");
                break;
            }
        }
        if ((status = mysql_next_result(mysql)) > 0)
        {
            printf("could not execute statement\n");
        }
    }while (status == 0);
    mysql_close(mysql);
    return 0;
}
