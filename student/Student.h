#ifndef STUDENT_H_H
#define STUDENT_H_H
#include <string>
#include <mysql/mysql.h>

using std::string;

//学生结构体
struct Student
{
    int m_id;
    string m_name;
    int m_age;
    string m_sex;
    string m_address;
    string m_telphone;
    string m_dormitory;
};

class CStudent
{
public:
    CStudent();
    ~CStudent();

    //按ID查找
    bool FindId(int id) const;
    //按姓名查找
    bool FindName(string name) const;
    //录入学生信息
    void Entering();
    //显示学生信息
    void Show(string title, string query) const;
    void Show() const;
   //查询学生信息
    void Query() const;
    //统计学生信息
    void Count() const;
    //删除学生信息
    void Erase() const;
    //修改学生信息
    void Modify() const;
private:
    void error() const;
private:
    MYSQL *m_mysql;
};

#endif
