#ifndef CCLASSMANANGE_H
#define CCLASSMANANGE_H

#include <QDialog>
#include <QSqlTableModel>
#include <QTableView>
#include <QPushButton>

class CClassManange : public QDialog
{
    Q_OBJECT
public:
    explicit CClassManange(QWidget *parent = 0);
private:
    void InitClassManage();
    void SubmitData(QString &msg);
    bool IsExit();
protected:
    void closeEvent(QCloseEvent *);
private:
    QSqlTableModel *m_Model;
    QTableView     *m_View;
    QPushButton    *m_AddClass;
    QPushButton    *m_DelClass;
    QPushButton    *m_ChgClass;
    QPushButton    *m_ExitManage;
signals:

public slots:
    void slotAddClassClicked();
    void slotDelClassClicked();
    void slotChgClassClicked();
    void slotm_ExitManageClicked();
};

#endif // CCLASSMANANGE_H
