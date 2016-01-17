#ifndef CCCourseManage_H
#define CCCourseManage_H

#include <QDialog>
#include <QSqlTableModel>
#include <QTableView>
#include <QPushButton>

class CCourseManage : public QDialog
{
    Q_OBJECT
public:
    explicit CCourseManage(QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *);

private:
    QSqlTableModel *m_Model;
    QTableView     *m_View;
    QPushButton    *m_AddCourse;
    QPushButton    *m_DelCourse;
    QPushButton    *m_ChgCourse;
    QPushButton    *m_ExitManage;
private:
    void InitCourseManage();
    void SubmitData(QString &msg);
    bool IsExit();
signals:

public slots:
    void slotAddCourseClicked();
    void slotDelCourseClicked();
    void slotChgCourseClicked();
    void slotm_ExitManageClicked();
};

#endif // CCourseManage_H
