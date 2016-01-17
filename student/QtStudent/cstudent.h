#ifndef CSTUDENT_H
#define CSTUDENT_H

#include "cstudentview.h"
#include <QMainWindow>
#include <QMenu>


class CStudent : public QMainWindow
{
    Q_OBJECT
public:
    explicit CStudent(QWidget *parent = 0);
private:
    void InitStudent();
private:
    QMenu *m_System;
    CStudentView *m_StudentView;
    QString m_username;

signals:

public slots:
    void ClassManage_Click();
    void CourseManage_Click();
    void StudentManage_Click();
    void ChangePassword_Click();
    void SystemManage_Click();

    void StudentCourseManage_Click();

    void ExitSys_Click();       //退出系统
};

#endif // CSTUDENT_H
