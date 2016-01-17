#include "Student.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <iomanip>

CStudent::CStudent()
{
    m_mysql = mysql_init(NULL);
    if (m_mysql == NULL)
    {
        error();
    }
    m_mysql = mysql_real_connect(m_mysql, "localhost", "root", "jiangxin", "Student", 0, NULL, 0);
    if (m_mysql == NULL)
    {
        error();
    }
    if (mysql_query(m_mysql, "set names utf8"))
    {
        error();
    }
}

CStudent::~CStudent()
{
    mysql_close(m_mysql);
}

bool CStudent::FindId(int id) const
{
    string query;
    std::stringstream stream;
    stream << "select * from student where id = " << id;
    query = stream.str();
    if (mysql_query(m_mysql, query.c_str()))
    {
        error();
    }
    MYSQL_RES *result = NULL;
    result = mysql_store_result(m_mysql);
    if (result == NULL )
    {
        error();
    }
    if (mysql_num_rows(result) == 0)
    {
        return false;
    }
    return true;
}

bool CStudent::FindName(string name) const
{
    string query;
    std::stringstream stream;
    stream << "select * from student where name = '" << name << "'";
    query = stream.str();
    if (mysql_query(m_mysql, query.c_str()))
    {
        error();
    }
    MYSQL_RES *result = NULL;
    result = mysql_store_result(m_mysql);
    if (result == NULL )
    {
        error();
    }
    if (mysql_num_rows(result) == 0)
    {
        return false;
    }
    return true;
}

void CStudent::error() const
{
    std::cout << "error";
    const char *buf = mysql_error(m_mysql);
    std::cout << buf << std::endl;
    exit(1);
}

void CStudent::Entering()
{
    Student temp;
    std::cout << "\t\t\t学号(0 to return):";
    std::cin >> temp.m_id;
    while (temp.m_id)
    {
        if (!FindId(temp.m_id))
        {
            std::cout << "\t\t\t姓   名:";
            std::cin >> temp.m_name;
            std::cout << "\t\t\t年   龄:";
            std::cin >> temp.m_age;
            std::cout << "\t\t\t性   别:";
            std::cin >> temp.m_sex;
            while (!(temp.m_sex == "男" || temp.m_sex == "女"))
            {
                std::cout << "\t\t\t你输入的性别有误，请重新输入:";
                std::cin >> temp.m_sex;
            }
            std::cout << "\t\t\t家庭住址:";
            std::cin >> temp.m_address;
            std::cout << "\t\t\t联系电话:";
            std::cin >> temp.m_telphone;
            std::cout << "\t\t\t寝室号:";
            std::cin >> temp.m_dormitory;

            string query;
            std::stringstream stream;
            stream << "insert into student values(" << temp.m_id << ",'" << temp.m_name << "',"
                << temp.m_age << ",'" << temp.m_sex << "','" << temp.m_address << "','"
                << temp.m_telphone << "','" << temp.m_dormitory << "'" << ")";
            query = stream.str();
            if (mysql_query(m_mysql, query.c_str()))
            {
                error();
            }
        }
        else
        {
            std::cout << "\t\t\t学号存在重复，请重新输入(0 to return):" << std::endl;
        }
        std::cout << "\t\t\t学号(0 to return):";
        std::cin >> temp.m_id;
    }
}

void CStudent::Show(string title, string query) const
{
    MYSQL_ROW row;
    MYSQL_RES *result = NULL;
    unsigned int num_fields;
    unsigned int i;
    std::cout << title << std::endl;
    if (mysql_query(m_mysql, query.c_str()))
    {
        error();
    }
    result = mysql_store_result(m_mysql);
    if (result == NULL)
    {
        error();
    }
    num_fields = mysql_num_fields(result);
    while ((row = mysql_fetch_row(result)))
    {
        for(i = 0; i < num_fields; i++)
        {
            std::cout << row[i] << "\t";
        }
        std::cout << std::endl;
    }
}

void CStudent::Show() const
{
    Show("学号\t姓名\t年龄\t性别\t家庭住址\t联系电话\t寝室号\t", "select * from student");
}

void CStudent::Query() const
{
    int nChoose = 0;
    std::cout << "\t\t\t1、按学号查询 2、按姓名查询 0、返回:";
    std::cin >> nChoose;
    switch (nChoose)
    {
        case 1:
            {
                int id;
                std::cout << "\t\t\t请输入学号:";
                std::cin >> id;
                if (FindId(id))
                {
                    string query;
                    std::stringstream stream;
                    stream << "select * from student where id = "<< id;
                    query = stream.str();
                    Show("学号\t姓名\t年龄\t性别\t家庭住址\t联系电话\t寝室号\t", query);
                }
                else
                {
                    std::cout << "\t\t\t不存在此学号的学生" << std::endl;
                }
                break;
            }
        case 2:
            {
                string name;
                std::cout << "\t\t\t请输入姓名:";
                std::cin >> name;
                if (FindName(name))
                {
                    string query;
                    std::stringstream stream;
                    stream << "select * from student where name = '"<< name << "'";
                    query = stream.str();
                    Show("学号\t姓名\t年龄\t性别\t家庭住址\t联系电话\t寝室号\t", query);
                }
                else
                {
                    std::cout << "\t\t\t不存在此姓名的学生" << std::endl;
                }
                break;
            }
        case 0:
            return;
        default:
            std::cout << "\t\t\t选择错误" << std::endl;
            break;
    }
}

void CStudent::Count() const
{
    Show("学生总数\t男生总数\t女生总数\t", "select count(*) 总人数,\
            sum(case when sex='男' then 1 else 0 end) 男生总数,\
            sum(case when sex='女' then 1 else 0 end) 女生总数 from student");
}

void CStudent::Erase() const
{
    int nID = 0;
    Student *ptemp,*pErase;
    std::cout << "\t\t\t请输入要删除的学号:";
    std::cin >> nID;
    if (FindId(nID))
    {
        string query;
        std::stringstream stream;
        stream << "delete from student where id = "<< nID;
        query = stream.str();
        if (mysql_query(m_mysql, query.c_str()))
        {
            error();
        }
        else
        {
            std::cout << "\t\t\t成功删除" << nID << "的信息" << std::endl;
        }
    }
    else
    {
        std::cout << "\t\t\t不存在此学号的学生" << std::endl;
    }
}

void CStudent::Modify() const
{
    int nID = 0;
    Student temp;
    std::cout << "\t\t\t请输入要修改的学生的学号:";
    std::cin >> nID;
    if (FindId(nID))
    {
        std::cout << "\t\t\t姓   名:";
        std::cin >> temp.m_name;
        std::cout << "\t\t\t年   龄:";
        std::cin >> temp.m_age;
        std::cout << "\t\t\t性   别:";
        std::cin >> temp.m_sex;
        std::cout << "\t\t\t家庭住址:";
        std::cin >> temp.m_address;
        std::cout << "\t\t\t联系电话:";
        std::cin >> temp.m_telphone;
        std::cout << "\t\t\t寝室号:";
        std::cin >> temp.m_dormitory;
        temp.m_id = nID;
        string query;
        std::stringstream stream;
        stream << "update student set id=" << temp.m_id << ",name='" << temp.m_name <<
            "',age=" << temp.m_age << ",sex='" << temp.m_sex << "',address='" << temp.m_address <<
            "',telphone='"<< temp.m_telphone << "',dormitory='" << temp.m_dormitory << "'";
        query = stream.str();
        if (mysql_query(m_mysql, query.c_str()))
        {
            error();
        }
    }
    else
    {
        std::cout << "\t\t\t不存在此学号的学生" << std::endl;
    }
}
