#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSqlError>
#include <QCloseEvent>
#include <QCompleter>
#include <QDebug>

#include "CStudentManage.h"

CStudentManage::CStudentManage(QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle("学生信息管理");
    InitStudentManage();
}

void CStudentManage::InitStudentManage()
{
    m_Model = new MySqlTableModel;
    m_View = new QTableView(this);

    connect(m_Model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
                            this, SLOT(slotModelDataChanged(QModelIndex, QModelIndex, QVector<int>)));
    //设置表
    m_Model->setTable("Student");

    //检索
    if (!m_Model->select())
    {
        QMessageBox::critical(this, "错误", m_Model->lastError().text());
        exit(0);
    }
    //设置为手动提交
    m_Model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //设置表头名
    m_Model->setHeaderData(0, Qt::Horizontal, "学号");
    m_Model->setHeaderData(1, Qt::Horizontal, "姓名");
    m_Model->setHeaderData(2, Qt::Horizontal, "性别");
    m_Model->setHeaderData(3, Qt::Horizontal, "出生年月");
    m_Model->setHeaderData(4, Qt::Horizontal, "民族");
    m_Model->setHeaderData(5, Qt::Horizontal, "班级号");
    m_Model->setHeaderData(6, Qt::Horizontal, "入学时间");
    m_Model->setHeaderData(7, Qt::Horizontal, "家庭地址");
    m_Model->setHeaderData(8, Qt::Horizontal, "政治面貌");
    m_Model->setHeaderData(9, Qt::Horizontal, "身份证号");
    m_Model->setHeaderData(10, Qt::Horizontal, "职位");
    m_Model->setHeaderData(11, Qt::Horizontal, "所学专业");
    //设置表格视图的表结构
    m_View->setModel(m_Model);

    //设置性别控件代理
    m_View->setItemDelegateForColumn(2, new SexDelegate);
    //设置班级号控件代理
    m_View->setItemDelegateForColumn(5, new ClassIDDelegate);

    QVBoxLayout *gridlay1;
    QHBoxLayout *HBoxlay2;
    QGroupBox *groupbox1;
    QGroupBox *groupbox2;
    QGridLayout *layout = new QGridLayout(this);

    //创建窗体及控件
    layout->addWidget(groupbox1 = new QGroupBox("现有学生信息"), 0, 0);
    layout->addWidget(groupbox2 = new QGroupBox("操作"), 1, 0);

    groupbox1->setLayout(gridlay1 = new QVBoxLayout);
    gridlay1->addWidget(m_View);

    groupbox2->setLayout(HBoxlay2 = new QHBoxLayout);
    HBoxlay2->addWidget(m_Edit = new QLineEdit);
    HBoxlay2->addWidget(m_AddStudent = new QPushButton("增加学生"));
    HBoxlay2->addWidget(m_DelStudent = new QPushButton("删除学生"));
    HBoxlay2->addWidget(m_ChgStudent = new QPushButton("保存修改"));
    HBoxlay2->addWidget(m_ExitManage = new QPushButton("退出"));

    //连接槽函数
    connect(m_AddStudent, SIGNAL(clicked()), this, SLOT(slotAddStudentClicked()));
    connect(m_DelStudent, SIGNAL(clicked()), this, SLOT(slotDelStudentClicked()));
    connect(m_ChgStudent, SIGNAL(clicked()), this, SLOT(slotChgStudentClicked()));
    connect(m_ExitManage, SIGNAL(clicked()), this, SLOT(slotExitManageClicked()));

    connect(m_Edit, SIGNAL(textChanged(QString)), this, SLOT(slotEditTextChanged(QString)));

    slotModelDataChanged(QModelIndex(), QModelIndex(), QVector<int>());

}

void CStudentManage::slotEditTextChanged(QString str)
{
    //如果为空，检索所有的字段
    if (str.isEmpty())
    {
        m_Model->setFilter("");
        m_Model->select();
    }

    QSqlRecord record = m_Model->record();
    QString ModelFilter;
    for (int i = 0; i < record.count(); ++i)
    {
        //获取字段名
        QString FilterName = record.fieldName(i);
        //如果是日期 直接跳过
        if (FilterName == "Birth" || FilterName == "Entrance_date")
            continue;
        if (i != 0)
        {
            ModelFilter += " or ";
        }
        //拼接
        QString subFilter = QObject::tr("%1 like '%%2%'").arg(FilterName.toUtf8().data(), str.toUtf8().data());
        //QString subFilter = QString().sprintf("%s like '%%%s%%'", FilterName.toUtf8().data(), str.toUtf8().data());

        ModelFilter += subFilter;
    }
    //qDebug() << ModelFilter;
    m_Model->setFilter(ModelFilter);
    m_Model->select();
}

void CStudentManage::slotAddStudentClicked()
{
    //开始事务
    m_Model->database().transaction();
    //在最后一行插入一个记录集
    QSqlRecord record = m_Model->record();
    if (m_Model->insertRecord(-1, record))
    {
        QMessageBox::information(NULL, "提示", "添加学生成功!");
    }
    else
    {
        QMessageBox::critical(NULL, "错误", "添加学生失败!");
    }
}

void CStudentManage::slotDelStudentClicked()
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

void CStudentManage::slotChgStudentClicked()
{
    QString msg = "修改保存成功!";
    SubmitData(msg);
}

void CStudentManage::slotExitManageClicked()
{
    if (IsExit())
    {
        this->accept();
    }
}

//提交事物
void CStudentManage::SubmitData(QString &msg)
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

bool CStudentManage::IsExit()
{
    if (QMessageBox::question(NULL, "退出", "您确认要退出？") == QMessageBox::Yes)
    {
        return true;
    }
    return false;
}

//重载closeEvent 对关闭做处理
void CStudentManage::closeEvent(QCloseEvent *ev)
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

void CStudentManage::slotModelDataChanged(QModelIndex, QModelIndex, QVector<int>)
{
    //遍历整个记录集
    QStringList StrList;
    for (int i = 0; i < m_Model->rowCount(); ++i)
    {
        QSqlRecord record = m_Model->record(i);
        for (int j = 0; j < record.count(); ++j)
        {
            QVariant var = record.value(j);
            if (var.isNull())
                continue;
            StrList << var.toString();
        }
    }
    //将记录集 设置到edit的匹配中
    QCompleter *completer = new QCompleter(StrList, this);
    completer->setFilterMode(Qt::MatchContains);
    m_Edit->setCompleter(completer);
}
