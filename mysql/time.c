#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

int main()
{
    int ret = 0, status = 0;
    MYSQL *mysql;
    MYSQL_RES *result;

    mysql = mysql_init(NULL);
    mysql = mysql_real_connect(mysql, "localhost", "root", "jiangxin", "mydb61", 0, NULL, 0);
    if (mysql == NULL)
    {
        ret = mysql_errno(mysql);
        printf("%s", mysql_error(mysql));
        printf("func mysql_real_connect() err:%d\n", ret);
        return ret;
    }
    else
    {
        printf("conncet ok \n");
    }

    MYSQL_TIME ts;
    MYSQL_BIND bind[3];
    MYSQL_STMT *stmt;
    char query[1024] = "INSERT INTO test_table(date_field, time_field, timestamp_field) values(?,?,?)";

    stmt = mysql_stmt_init(mysql);
    if (!stmt)
    {
        fprintf(stderr, "mysql_stmt_init(), out of memory\n");
        exit(0);
    }
    if (mysql_stmt_prepare(stmt, query, strlen(query)))
    {
        fprintf(stderr, "\n mysql_stmt_prepare(), INSERT failed");
        fprintf(stderr, "\n %s", mysql_stmt_error(stmt));
        exit(0);
    }

    bind[0].buffer_type = MYSQL_TYPE_DATE;
    bind[0].buffer = (char *)&ts;
    bind[0].is_null = 0;
    bind[0].length = 0;

    bind[1] = bind[2] = bind[0];

    mysql_stmt_bind_param(stmt, bind);

    ts.year = 2002;
    ts.month = 02;
    ts.day = 03;

    ts.hour = 10;
    ts.minute = 45;
    ts.second = 20;

    mysql_stmt_execute(stmt);
    if (mysql_stmt_close(stmt))
    {
        fprintf(stderr, " failed while closing the statement\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        exit(0);
    }
    mysql_close(mysql);
}
