#ifndef CSTUDENTCOURSEMANAGE_H
#define CSTUDENTCOURSEMANAGE_H

#include <QDialog>
#include <QSqlTableModel>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QItemDelegate>
#include <QSqlRecord>
#include <QComboBox>
#include <QCompleter>
#include <QDebug>

//设置班级名称的代理控件
class StudentNameDelegate : public QItemDelegate
{
public:
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &,
                          const QModelIndex &) const Q_DECL_OVERRIDE
    {
        QComboBox *comboBox = new QComboBox(parent);
        QSqlTableModel model;
        QStringList StrList;
        comboBox->setEditable(true);
        //设置表
        model.setTable("Student");
        //检索
        model.select();
        for (int i = 0; i < model.rowCount(); ++i)
        {
            QSqlRecord record = model.record(i);
            QVariant StudentName = record.value("Student_name");
            //将Student表中的所有学生姓名加入到匹配中
            StrList << StudentName.toString();
            comboBox->addItem(StudentName.toString());
        }
        //设置匹配
        QCompleter *completre = new QCompleter(StrList, parent);
        completre->setFilterMode(Qt::MatchContains);
        comboBox->setCompleter(completre);
        return comboBox;
    }
};

//设置班级名称的代理控件
class ClaasIdDelegate : public QItemDelegate
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
            //qDebug() << record.value("Class_name").toString();
            comboBox->addItem(record.value("Class_name").toString());
        }
        return comboBox;
    }
};

//设置课程名称的代理控件
class CourseIdDelegate : public QItemDelegate
{
public:
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &,
                          const QModelIndex &) const Q_DECL_OVERRIDE
    {
        QComboBox *comboBox = new QComboBox(parent);
        QSqlTableModel model;
        //设置表
        model.setTable("Course");
        //检索
        model.select();
        for (int i = 0; i < model.rowCount(); ++i)
        {
            QSqlRecord record = model.record(i);
            //qDebug() << record.value("Course_name").toString();
            comboBox->addItem(record.value("Course_name").toString());
        }
        return comboBox;
    }
};

class CStudentCourseManage : public QDialog
{
    Q_OBJECT
public:
    explicit CStudentCourseManage(QWidget *parent = 0);
private:
    void InitStudentCourseManage();
    void SubmitData(QString &msg);
    bool IsExit();
protected:
    void closeEvent(QCloseEvent *);
private:
    QSqlTableModel *m_Model;
    QTableView     *m_View;
    QLineEdit      *m_Edit;
    QPushButton    *m_AddStudentCourse;
    QPushButton    *m_DelStudentCourse;
    QPushButton    *m_ChgStudentCourse;
    QPushButton    *m_ExitManage;
signals:

public slots:
    void slotAddStudentCourseClicked();
    void slotDelStudentCourseClicked();
    void slotChgStudentCourseClicked();
    void slotExitManageClicked();

    void slotModelDataChanged(QModelIndex, QModelIndex, QVector<int>);
    void slotEditTextChanged(QString);
};

#endif // CSTUDENTCOURSEMANAGE_H
