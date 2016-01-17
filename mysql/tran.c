#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <mysql/mysql.h>

#define BEGIN_TRAN  "START TRANSACTION"
#define SET_TRAN    "SET AUTOCOMMIT=0"
#define UNSET_TRAN  "SET AUTOCOMMIT=1"
#define COMMIT_TRAN "COMMIT"
#define ROLLBACK_TRAN "ROLLBACK"

int mysql_BeginTran(MYSQL *mysql)
{
    int ret = 0;

    //执行事务开始SQL
    ret = mysql_query(mysql, BEGIN_TRAN);
    if (ret != 0)
    {
        printf("func mysql_query() err %d\n", ret);
        return ret;
    }
    //设置事务手动提交
    ret = mysql_query(mysql, SET_TRAN);
    if (ret != 0)
    {
        printf("func mysql_query() err %d\n", ret);
        return ret;
    }
    return ret;
}

int mysql_Rollback(MYSQL *mysql)
{
    int ret = 0;

    //事务回滚操作
    ret = mysql_query(mysql, ROLLBACK_TRAN);
    if (ret != 0)
    {
        printf("func mysql_query() err %d\n", ret);
        return ret;
    }

    //恢复事务自动提交标志
    ret = mysql_query(mysql, UNSET_TRAN);
    if (ret != 0)
    {
        printf("func mysql_query() err %d\n", ret);
        return ret;
    }
    return ret;
}

int mysql_Commit(MYSQL *mysql)
{
    int ret = 0;

    //执行事务提交sql
    ret = mysql_query(mysql, COMMIT_TRAN);
    if (ret != 0)
    {
        printf("func mysql_query() err %d\n", ret);
        return ret;
    }

    //恢复事务自动提交标志
    ret = mysql_query(mysql, UNSET_TRAN);
    if (ret != 0)
    {
        printf("func mysql_query() err %d\n", ret);
        return ret;
    }
    return ret;
}

// drop table test_table
// // 建表 create table test_table(col1 int, col2 varchar(10), col3 varchar(10));
#define sql01 "INSERT INTO test_table(col1, col2, col3) values(10, '10', '1')"
#define sql02 "INSERT INTO test_table(col1, col2, col3) values(20, '20', '2')"
#define sql03 "INSERT INTO test_table(col1, col2, col3) values(30, '30', '3')"
#define sql04 "INSERT INTO test_table(col1, col2, col3) values(40, '40', '4')"

int main()
{
    int ret = 0;
    MYSQL *mysql;

    mysql = mysql_init(NULL);
    mysql = mysql_real_connect(mysql, "localhost", "root", "jiangxin", "mydb61", 0, NULL, 0);
    if (mysql == NULL)
    {
        ret = mysql_errno(mysql);
        printf("func mysql_real_connect() err\n");
        return ret;
    }
    else
    {
        printf("connect ok\n");
    }
    ret = mysql_BeginTran(mysql);   //修改事务的属性，变成不自动提交
    if (ret != 0)
    {
        printf("mysql_BeginTran() err:%d\n", ret);
        return ret;
    }
    ret = mysql_query(mysql, sql01);
    if (ret != 0)
    {
        printf("mysql_query() err:%d\n", ret);
        return ret;
    }
    ret = mysql_query(mysql, sql02);
    if (ret != 0)
    {
        printf("mysql_query() err:%d\n", ret);
        return ret;
    }
    ret = mysql_Commit(mysql);
    if (ret != 0)
    {
        printf("mysql_Commit() err:%d\n", ret);
        return ret;
    }

    ret = mysql_BeginTran(mysql);
    if (ret != 0)
    {
        printf("mysql_BeginTran() err:%d\n", ret);
        return ret;
    }
    ret = mysql_query(mysql, sql03);
    if (ret != 0)
    {
        printf("mysql_query() err:%d\n", ret);
        return ret;
    }
    ret = mysql_query(mysql, sql04);
    if (ret != 0)
    {
        printf("mysql_query() err:%d\n", ret);
        return ret;
    }
    //回滚事务
    ret = mysql_Rollback(mysql);
    if (ret != 0)
    {
        printf("mysql_Rollback() err:%d\n", ret);
        return ret;
    }
    mysql_close(mysql);
}
