#ifndef CSTUDENTMANAGE_H
#define CSTUDENTMANAGE_H

#include <QDialog>
#include <QSqlTableModel>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QItemDelegate>
#include <QSqlRecord>

//性别的代理控件
class SexDelegate : public QItemDelegate
{
public:
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &,
                          const QModelIndex &) const Q_DECL_OVERRIDE
    {
        QComboBox *comboBox = new QComboBox(parent);
        comboBox->addItem("男");
        comboBox->addItem("女");
        return comboBox;
    }
};

//班级号的代理控件
class ClassIDDelegate : public QItemDelegate
{
public:
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &,
                          const QModelIndex &) const Q_DECL_OVERRIDE
    {
        QComboBox *comboBox = new QComboBox(parent);
        QSqlTableModel model;
        //设置表
        model.setTable("Class");
        //检索
        model.select();
        for (int i = 0; i < model.rowCount(); ++i)
        {
            QSqlRecord record = model.record(i);
            comboBox->addItem(record.value("Class_id").toString());
        }
        return comboBox;
    }
};



class MySqlTableModel : public QSqlTableModel
{
public:

    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE
    {
        if(idx.column() != 2)
            return QSqlTableModel::data(idx, role);
        QVariant var = QSqlTableModel::data(idx, role);
        if (var.type() == QVariant::Int)
        {
            if(var == 0)
            {
               return "女";
            }
            else
            {
                return "男";
            }
        }
        return var;
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE
    {
        if(index.column() == 2)
        {
            if(value == "男")
                return QSqlTableModel::setData(index, 1, role);
            return QSqlTableModel::setData(index, 0, role);
        }
        return QSqlTableModel::setData(index, value, role);
    }
};

class CStudentManage : public QDialog
{
    Q_OBJECT
public:
    explicit CStudentManage(QWidget *parent = 0);
private:
    void InitStudentManage();
    void SubmitData(QString &msg);
    bool IsExit();
protected:
    void closeEvent(QCloseEvent *);

private:
    MySqlTableModel  *m_Model;
    QTableView      *m_View;
    QLineEdit       *m_Edit;        //进行匹配的Edit
    QPushButton     *m_AddStudent;  //添加学生信息
    QPushButton     *m_DelStudent;  //删除学生信息
    QPushButton     *m_ChgStudent;  //更改学生信息
    QPushButton     *m_ExitManage;  //退出学生管理系统
signals:

public slots:
    void slotAddStudentClicked();
    void slotDelStudentClicked();
    void slotChgStudentClicked();
    void slotExitManageClicked();
    void slotModelDataChanged(QModelIndex, QModelIndex, QVector<int>);
    void slotEditTextChanged(QString);
};

#endif // CSTUDENTMANAGE_H
