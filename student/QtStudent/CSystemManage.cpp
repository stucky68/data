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

#include "CSystemManage.h"

CSystemManage::CSystemManage(QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle("权限管理");
    InitSystemManage();
}

void CSystemManage::InitSystemManage()
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
    gridlay1->addWidget(new QLabel("权限"), 2, 0);
    gridlay1->addWidget(m_username = new QLineEdit, 0, 1);
    gridlay1->addWidget(m_password = new QLineEdit, 1, 1);
    gridlay1->addWidget(m_role = new QComboBox, 2, 1);

    m_role->addItem("校长");
    m_role->addItem("教导处主任");
    m_role->addItem("教师");
    m_password->setEchoMode(QLineEdit::Password);

    groupbox2->setLayout(gridlay2 = new QGridLayout);
    gridlay2->addWidget(m_ok = new QPushButton("确认"), 0, 0);
    gridlay2->addWidget(m_cancel = new QPushButton("取消"), 0, 1);

    connect(m_ok, SIGNAL(clicked()), this, SLOT(slotAddSystem()));
    connect(m_cancel, SIGNAL(clicked()), this, SLOT(slotExitManage()));

    m_Model = new QSqlTableModel(this);
}

void CSystemManage::slotAddSystem()
{
    if (m_username->text().isEmpty() || m_password->text().isEmpty())
    {
        QMessageBox::information(NULL, "提示", "用户帐号或密码不能为空！");
    }
    else
    {
        //先检查用户名是否存在
        m_Model->setTable("Syuser");
        m_Model->setFilter("User_name='" + m_username->text() + "'");
        if (!m_Model->select())
        {
            QMessageBox::critical(NULL, "错误", m_Model->lastError().text());
            exit(0);
        }
        //查询结果行数为0
        if (m_Model->rowCount() > 0)
        {
            QMessageBox::information(NULL, "提示", "此用户已存在");
        }
        else
        {
            //如果不存在 插入一条记录集
            QSqlRecord record = m_Model->record();
            record.setValue("User_name", m_username->text());
            record.setValue("User_role", m_role->currentText());
            record.setValue("Password", m_password->text());
            if (m_Model->insertRecord(-1, record))
            {
                QMessageBox::information(NULL, "提示", "添加系统用户成功");
            }
            else
            {
                QMessageBox::critical(NULL, "错误", m_Model->lastError().text());
                exit(0);
            }
        }
    }
}

void CSystemManage::slotExitManage()
{
    if (QMessageBox::question(NULL, "退出", "您确认要退出？") == QMessageBox::Yes)
    {
        this->accept();
    }
}
