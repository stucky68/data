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

#include "CChangePassWord.h"

CChangePassWord::CChangePassWord(QWidget *parent) : QDialog(parent)
{

}

CChangePassWord::CChangePassWord(QString username, QWidget *parent) : QDialog(parent), m_user(username)
{
    this->setWindowTitle("密码修改");
    InitChangePassWord();
}

void CChangePassWord::InitChangePassWord()
{
    QGridLayout *gridlay1;
    QGridLayout *gridlay2;
    QGroupBox *groupbox1;
    QGroupBox *groupbox2;
    QGridLayout *layout = new QGridLayout(this);

    //创建窗体及控件
    layout->addWidget(groupbox1 = new QGroupBox("修改密码"), 0, 0);
    layout->addWidget(groupbox2 = new QGroupBox("操作"), 1, 0);

    groupbox1->setLayout(gridlay1 = new QGridLayout);
    gridlay1->addWidget(new QLabel("新密码"), 0, 0);
    gridlay1->addWidget(new QLabel("确认密码"), 1, 0);
    gridlay1->addWidget(m_Oldpassword = new QLineEdit, 0, 1);
    gridlay1->addWidget(m_Newpassword = new QLineEdit, 1, 1);

    m_Oldpassword->setEchoMode(QLineEdit::Password);
    m_Newpassword->setEchoMode(QLineEdit::Password);

    groupbox2->setLayout(gridlay2 = new QGridLayout);
    gridlay2->addWidget(m_change = new QPushButton("确认"), 0, 0);
    gridlay2->addWidget(m_exit = new QPushButton("取消"), 0, 1);

    //连接处理函数
    connect(m_change, SIGNAL(clicked()), this, SLOT(slotChange()));
    connect(m_exit, SIGNAL(clicked()), this, SLOT(slotExit()));

    //创建数据库表
    m_Model = new QSqlTableModel(this);
}

void CChangePassWord::slotChange()
{
    if (m_Oldpassword->text().isEmpty() || m_Newpassword->text().isEmpty())
    {
        QMessageBox::information(NULL, "提示", "当前密码或新密码不能为空！");
    }
    else
    {
        qDebug() << m_user;
        m_Model->setTable("Syuser");
        m_Model->setFilter("User_name='" + m_user + "'");
        if (!m_Model->select())
        {
            QMessageBox::critical(NULL, "错误", m_Model->lastError().text());
            exit(0);
        }
        QSqlRecord record = m_Model->record(0);


        if (record.value(3) ==  m_Oldpassword->text())
        {
            m_Model->setData(m_Model->index(0, 3), m_Newpassword->text());
            if (m_Model->submitAll())
            {
                QMessageBox::information(NULL, "提示", "修改密码成功!");
            }
            else
            {
                QMessageBox::critical(NULL, "错误", m_Model->lastError().text());
            }
        }
        else
        {
            QMessageBox::information(NULL, "提示", "当前密码错误!");
        }
        //
        //m_Model->setData()
    }
}

void CChangePassWord::slotExit()
{
    if (QMessageBox::question(NULL, "退出", "您确认要退出？") == QMessageBox::Yes)
    {
        this->close();
    }
}

