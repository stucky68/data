#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlError>
#include <QCloseEvent>
#include <QDebug>

#include "CClassManange.h"

CClassManange::CClassManange(QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle("班级管理");
    InitClassManage();
}

void CClassManange::InitClassManage()
{
    m_Model = new QSqlTableModel(this);
    m_View = new QTableView;

    //设置表
    m_Model->setTable("Class");
    //检索
    if (!m_Model->select())
    {
        QMessageBox::critical(this, "错误", m_Model->lastError().text());
        exit(0);
    }
    //设置为手动提交
    m_Model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //设置表头名
    m_Model->setHeaderData(0, Qt::Horizontal, "班级编号");
    m_Model->setHeaderData(1, Qt::Horizontal, "年级");
    m_Model->setHeaderData(2, Qt::Horizontal, "班级名称");
    m_Model->setHeaderData(3, Qt::Horizontal, "班级人数");
    m_Model->setHeaderData(4, Qt::Horizontal, "最大人数");
    //设置表格视图的表结构
    m_View->setModel(m_Model);


    QVBoxLayout *gridlay1;
    QHBoxLayout *HBoxlay2;
    QGroupBox *groupbox1;
    QGroupBox *groupbox2;
    QGridLayout *layout = new QGridLayout(this);

    //创建窗体及控件
    layout->addWidget(groupbox1 = new QGroupBox("现有班级信息"), 0, 0);
    layout->addWidget(groupbox2 = new QGroupBox("操作"), 1, 0);

    groupbox1->setLayout(gridlay1 = new QVBoxLayout);
    gridlay1->addWidget(m_View);

    groupbox2->setLayout(HBoxlay2 = new QHBoxLayout);
    HBoxlay2->addWidget(m_AddClass = new QPushButton("增加班级"));
    HBoxlay2->addWidget(m_DelClass = new QPushButton("删除班级"));
    HBoxlay2->addWidget(m_ChgClass = new QPushButton("保存修改"));
    HBoxlay2->addWidget(m_ExitManage = new QPushButton("退出"));

    //连接槽函数
    connect(m_AddClass, SIGNAL(clicked()), this, SLOT(slotAddClassClicked()));
    connect(m_DelClass, SIGNAL(clicked()), this, SLOT(slotDelClassClicked()));
    connect(m_ChgClass, SIGNAL(clicked()), this, SLOT(slotChgClassClicked()));
    connect(m_ExitManage, SIGNAL(clicked()), this, SLOT(slotm_ExitManageClicked()));
}

//增加班级
void CClassManange::slotAddClassClicked()
{
    //开始事务
    m_Model->database().transaction();
    //在最后一行插入一个记录集
    QSqlRecord record = m_Model->record();
    if (m_Model->insertRecord(-1, record))
    {
        QMessageBox::information(NULL, "提示", "添加班级成功!");
    }
    else
    {
        QMessageBox::critical(NULL, "错误", "添加班级失败!");
    }
}

//删除班级
void CClassManange::slotDelClassClicked()
{
    //开始事务
    m_Model->database().transaction();
    //获取被选中的model
    QItemSelectionModel *selectModel = m_View->selectionModel();

    //通过被选中的model, 获取被选中的格子
    QModelIndexList selectList = selectModel->selectedIndexes();

    QList<int> delRow;

    //遍历格子，获得行
    for (int i = 0; i < selectList.size(); ++i)
    {
        delRow << selectList.at(i).row();
    }

    //去除重复的行
    while (delRow.size() > 0)
    {
        int row = delRow.at(0);
        //去重
        delRow.removeAll(row);

        //删除行
        m_Model->removeRow(row);
    }
    //提交
    QString msg = "删除成功!";
    SubmitData(msg);
}

void CClassManange::slotChgClassClicked()
{
    QString msg = "修改保存成功!";
    SubmitData(msg);
}

void CClassManange::slotm_ExitManageClicked()
{
    if (IsExit())
    {
        this->accept();
    }
}

bool CClassManange::IsExit()
{
    if (QMessageBox::question(NULL, "退出", "您确认要退出？") == QMessageBox::Yes)
    {
        return true;
    }
    return false;
}

//提交事物
void CClassManange::SubmitData(QString &msg)
{
    //如果提交失败，回滚事务
    if (!m_Model->submitAll())
    {
        QMessageBox::critical(NULL, "错误", m_Model->lastError().text());
        if (!m_Model->database().rollback())
        {
            QMessageBox::critical(NULL, "错误", m_Model->lastError().text());
        }
        exit(0);
    }
    else
    {
        //提交事务
        if (!m_Model->database().commit())
        {
            QMessageBox::critical(NULL, "错误", m_Model->lastError().text());
            exit(0);
        }
        else
        {
            QMessageBox::information(NULL, "提示", msg);
        }
    }
}

//重载closeEvent 对关闭做处理
void CClassManange::closeEvent(QCloseEvent *ev)
{
    if (IsExit())
    {
        //退出
        ev->accept();
    }
    else
    {
        //忽略
        ev->ignore();
    }
}

