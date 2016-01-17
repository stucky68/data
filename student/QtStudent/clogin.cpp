#include <QSqlRecord>
#include <QDebug>
#include <QSqlError>
#include <QPushButton>
#include <QGridLayout>
#include <QDesktopWidget>
#include <QDebug>
#include <QMessageBox>
#include <QLabel>
#include <QGroupBox>

#include "clogin.h"
#include "cstudent.h"

CLogin::CLogin(QWidget *parent) : QDialog(parent)
{
    //设置标题名称
    this->setWindowTitle("用户登录");

    //初始化窗体
    InitLogin();
    m_loginOrnot = false;
}

void CLogin::InitLogin()
{
    QGridLayout *gridlay1;
    QGridLayout *gridlay2;
    QGroupBox *groupbox1;
    QGroupBox *groupbox2;
    QGridLayout *layout = new QGridLayout(this);

    //创建窗体及控件
    layout->addWidget(groupbox1 = new QGroupBox("登录"), 0, 0);
    layout->addWidget(groupbox2 = new QGroupBox("操作"), 1, 0);

    groupbox1->setLayout(gridlay1 = new QGridLayout);
    gridlay1->addWidget(new QLabel("用户名"), 0, 0);
    gridlay1->addWidget(new QLabel("密码"), 1, 0);
    gridlay1->addWidget(m_username = new QLineEdit, 0, 1);
    gridlay1->addWidget(m_password = new QLineEdit, 1, 1);

    m_password->setEchoMode(QLineEdit::Password);

    groupbox2->setLayout(gridlay2 = new QGridLayout);
    gridlay2->addWidget(m_login = new QPushButton("确认"), 0, 0);
    gridlay2->addWidget(m_logout = new QPushButton("取消"), 0, 1);

    //连接处理函数
    connect(m_login, SIGNAL(clicked()), this, SLOT(slotLogin()));
    connect(m_logout, SIGNAL(clicked()), this, SLOT(slotLogOut()));

    //创建数据库表
    m_Model = new QSqlTableModel(this);
}

void CLogin::slotLogin()
{
    if (m_username->text().isEmpty() || m_password->text().isEmpty())
    {
        QMessageBox::information(NULL, "提示", "用户帐号或密码不能为空！");
    }
    else
    {
        m_Model->setTable("Syuser");
        m_Model->setFilter("User_name='" + m_username->text() + "'");
        if (!m_Model->select())
        {
            QMessageBox::critical(NULL, "错误", m_Model->lastError().text());
            exit(0);
        }
        //查询结果行数为0
        if (m_Model->rowCount() == 0)
        {
            QMessageBox::information(NULL, "提示", "没有此用户");
        }
        else
        {
            if (m_Model->record(0).value(3) == m_password->text())
            {
                QMessageBox::information(NULL, "提示", "登录成功");
                m_loginOrnot = true;
                m_user = m_username->text();
                this->close();
            }
            else
            {
                QMessageBox::information(NULL, "提示", "用户密码错误");
                m_username->clear();
                m_password->clear();
                m_loginOrnot = false;
            }
        }
    }
}

void CLogin::slotLogOut()
{
    if (QMessageBox::question(NULL, "退出", "您确认要退出？") == QMessageBox::Yes)
    {
        this->close();
    }
}

bool CLogin::IsLogin()
{
    return m_loginOrnot == true;
}

