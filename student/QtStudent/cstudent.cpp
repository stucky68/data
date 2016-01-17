#include <QLabel>
#include <QStatusBar>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>

#include "cstudent.h"
#include "clogin.h"
#include "CCourseManage.h"
#include "CClassManange.h"
#include "CStudentManage.h"
#include "CStudentCourseManage.h"
#include "CChangePassWord.h"
#include <CSystemManage.h>

CStudent::CStudent(QWidget *parent) : QMainWindow(parent)
{
    this->setWindowTitle("学生成绩管理系统");
    InitStudent();
}
void CStudent::InitStudent()
{
    CLogin login;
    login.exec();

    if (login.IsLogin())
    {
        menuBar()->addAction("&学生信息管理", this, SLOT(StudentManage_Click()));

        menuBar()->addAction("&学生成绩管理", this, SLOT(StudentCourseManage_Click()));

        m_System = menuBar()->addMenu("&系统维护");
        m_System->addAction("&权限管理", this, SLOT(SystemManage_Click()));
        m_System->addAction("&密码修改", this, SLOT(ChangePassword_Click()));
        m_System->addAction("&班级管理", this, SLOT(ClassManage_Click()));
        m_System->addAction("&课程设置", this, SLOT(CourseManage_Click()));

        menuBar()->addAction("&退出系统", this, SLOT(ExitSys_Click()));

        //创建客户区域
        m_StudentView = new CStudentView;
        this->setCentralWidget(m_StudentView);

        //创建状态栏
        QStatusBar *pStatusbar = this->statusBar();
        m_username = login.m_user;
        pStatusbar->addWidget(new QLabel("欢迎使用学生信息管理系统：" + m_username));
    }
    else
    {
        exit(0);
    }
}

//退出系统
void CStudent::ExitSys_Click()
{
    if (QMessageBox::question(NULL, "提示", "您确认要退出本系统？") == QMessageBox::Yes)
    {
        this->close();
    }
}

//课程管理
void CStudent::CourseManage_Click()
{
    CCourseManage course;
    course.exec();
}

//班级管理
void CStudent::ClassManage_Click()
{
    CClassManange Class;
    Class.resize(550, 340);
    Class.exec();
}

//学生信息管理
void CStudent::StudentManage_Click()
{
    CStudentManage student;
    student.resize(1250, 340);
    student.exec();
}

//学生成绩管理
void CStudent::StudentCourseManage_Click()
{
    CStudentCourseManage StudentCoures;
    StudentCoures.exec();
}

//修改密码
void CStudent::ChangePassword_Click()
{
    CChangePassWord changepsd(m_username);
    changepsd.exec();
}

void CStudent::SystemManage_Click()
{
    CSystemManage system;
    system.exec();
}

