#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlError>
#include <QCloseEvent>

#include "CCourseManage.h"

CCourseManage::CCourseManage(QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle("课程管理");
    InitCourseManage();
}

void CCourseManage::InitCourseManage()
{
    m_Model = new QSqlTableModel(this);
    m_View = new QTableView;

    //设置表
    m_Model->setTable("Course");
    //检索
    if (!m_Model->select())
    {
        QMessageBox::critical(this, "错误", m_Model->lastError().text());
        exit(0);
    }
    //设置为手动提交
    m_Model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //设置表头名
    m_Model->setHeaderData(0, Qt::Horizontal, "课程号");
    m_Model->setHeaderData(1, Qt::Horizontal, "课程名");
    m_Model->setHeaderData(2, Qt::Horizontal, "课程学分");
    //设置表格视图的表结构
    m_View->setModel(m_Model);


    QVBoxLayout *gridlay1;
    QHBoxLayout *HBoxlay2;
    QGroupBox *groupbox1;
    QGroupBox *groupbox2;
    QGridLayout *layout = new QGridLayout(this);

    //创建窗体及控件
    layout->addWidget(groupbox1 = new QGroupBox("现有课程信息"), 0, 0);
    layout->addWidget(groupbox2 = new QGroupBox("操作"), 1, 0);

    groupbox1->setLayout(gridlay1 = new QVBoxLayout);
    gridlay1->addWidget(m_View);

    groupbox2->setLayout(HBoxlay2 = new QHBoxLayout);
    HBoxlay2->addWidget(m_AddCourse = new QPushButton("增加课程"));
    HBoxlay2->addWidget(m_DelCourse = new QPushButton("删除课程"));
    HBoxlay2->addWidget(m_ChgCourse = new QPushButton("保存修改"));
    HBoxlay2->addWidget(m_ExitManage = new QPushButton("退出"));

    //连接槽函数
    connect(m_AddCourse, SIGNAL(clicked()), this, SLOT(slotAddCourseClicked()));
    connect(m_DelCourse, SIGNAL(clicked()), this, SLOT(slotDelCourseClicked()));
    connect(m_ChgCourse, SIGNAL(clicked()), this, SLOT(slotChgCourseClicked()));
    connect(m_ExitManage, SIGNAL(clicked()), this, SLOT(slotm_ExitManageClicked()));

}

//增加课程
void CCourseManage::slotAddCourseClicked()
{
    //开始事务
    m_Model->database().transaction();
    //在最后一行插入一个记录集
    QSqlRecord record = m_Model->record();
    if (m_Model->insertRecord(-1, record))
    {
        QMessageBox::information(NULL, "提示", "添加课程成功!");
    }
    else
    {
        QMessageBox::critical(NULL, "错误", "添加课程失败!");
    }
}

//删除课程
void CCourseManage::slotDelCourseClicked()
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

//提交事物
void CCourseManage::SubmitData(QString &msg)
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

//保存修改
void CCourseManage::slotChgCourseClicked()
{
    QString msg = "修改保存成功!";
    SubmitData(msg);
}

//退出
void CCourseManage::slotm_ExitManageClicked()
{
    if (IsExit())
    {
        this->accept();
    }
}

//重载closeEvent 对关闭做处理
void CCourseManage::closeEvent(QCloseEvent *ev)
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

bool CCourseManage::IsExit()
{
    if (QMessageBox::question(NULL, "退出", "您确认要退出？") == QMessageBox::Yes)
    {
        return true;
    }
    return false;
}

