#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>
#include <stdlib.h>

#define LENGTH 1024
#define STRING_SIZE 50
#define DROP_SAMPLE_TABLE "DROP TABLE IF EXISTS test_table"
#define CREATE_SAMPLE_TABLE "CREATE TABLE test_table(col1 INT, col2 VARCHAR(40), col3 SMALLINT, col4 TIMESTAMP)"
#define INSERT_SAMPLE "INSERT INTO test_table(col1, col2, col3) values(?,?,?)"

int main()
{
    MYSQL *mysql;
    MYSQL *connect;
    //初始化
    mysql = mysql_init(NULL);
    if (mysql == NULL)
    {
        printf("init error\n");
        exit(-1);
    }

    //连接服务器
    connect = mysql_real_connect(mysql, "localhost", "root", "jiangxin", "mydb61", 0, NULL, 0);
    if (connect == NULL)
    {
        printf("connect error\n");
        exit(-1);
    }
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[3];
    my_ulonglong affected_rows;
    int param_count;
    short small_data;
    int int_data;
    char str_data[STRING_SIZE];
    unsigned long str_length;
    my_bool is_null;

    if (mysql_query(mysql, DROP_SAMPLE_TABLE))
    {
        fprintf(stderr, "DROP TABLE failed\n");
        fprintf(stderr, "%s\n", mysql_error(mysql));
        exit(0);
    }

    if (mysql_query(mysql, CREATE_SAMPLE_TABLE))
    {
        fprintf(stderr, "CREATE TABLE failed\n");
        fprintf(stderr, "%s\n", mysql_error(mysql));
        exit(0);
    }

    //初始化预处理环境句柄
    stmt = mysql_stmt_init(mysql);
    if (!stmt)
    {
        fprintf(stderr, "mysql_stmt_init(), out of memory\n");
        exit(0);
    }
    //准备预处理语句
    if (mysql_stmt_prepare(stmt, INSERT_SAMPLE, strlen(INSERT_SAMPLE)))
    {
        fprintf(stderr, "mysql_stmt_prepare(), INSERT failed\n");
        fprintf(stderr, "%s\n", mysql_error(mysql));
        exit(0);
    }
    fprintf(stdout, "prepare, INSERT successful\n");

    //获取sql语句中 占位符的个数
    param_count = mysql_stmt_param_count(stmt);
    fprintf(stdout, "totol parameters in INSERT: %d\n", param_count);
    if (param_count != 3)
    {
        fprintf(stderr, "invaild parameters count returned by MSSQL\n");
        exit(0);
    }

    memset(bind, 0, sizeof(bind));

    //设置第一个占位符的属性
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = (char *)&int_data;
    bind[0].is_null = 0;
    bind[0].length = 0;
    //设置第二个占位符的属性
    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (char *)str_data;
    bind[1].is_null = 0;
    bind[1].length = &str_length;
    //设置第三个占位符的属性
    bind[2].buffer_type = MYSQL_TYPE_SHORT;
    bind[2].buffer = (char *)&small_data;
    bind[2].is_null = &is_null;
    bind[2].length = 0;

    //绑定属性
    if (mysql_stmt_bind_param(stmt, bind))
    {
        fprintf(stderr, "mysql_stmt_bind_param() failed\n");
        fprintf(stderr, "%s\n", mysql_error(mysql));
        exit(0);
    }

    //准备插入数据
    int_data = 10;
    strncpy(str_data, "MySQL", STRING_SIZE);
    str_length = strlen(str_data);
    is_null = 1;
    //执行
    if (mysql_stmt_execute(stmt))   //执行预处理环境  插入第一条记录
    {
        fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        exit(0);
    }
    //获取受影响的行信息
    affected_rows= mysql_stmt_affected_rows(stmt);
    fprintf(stdout, " total affected rows(insert 1): %lu\n",
            (unsigned long) affected_rows);

    if (affected_rows != 1)
    {
        fprintf(stderr, " invalid affected rows by MySQL\n");
        exit(0);
    }

    int_data= 1000;
    strncpy(str_data, "The most popular Open Source database", STRING_SIZE);
    str_length= strlen(str_data);
    small_data= 1000;
    is_null= 0;
    //插入第二条记录
    if (mysql_stmt_execute(stmt))
    {
        fprintf(stderr, " mysql_stmt_execute, 2 failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        exit(0);
    }

    affected_rows= mysql_stmt_affected_rows(stmt);
    fprintf(stdout, " total affected rows(insert 2): %lu\n",
            (unsigned long) affected_rows);

    if (affected_rows != 1)
    {
        fprintf(stderr, " invalid affected rows by MySQL\n");
        exit(0);
    }

    //关闭预处理环境
    if (mysql_stmt_close(stmt))
    {
        fprintf(stderr, " failed while closing the statement\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        exit(0);
    }
    //关闭连接
    mysql_close(mysql);
    return 0;
}
