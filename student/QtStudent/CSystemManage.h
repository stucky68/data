#ifndef CSYSTEMMANAGE_H
#define CSYSTEMMANAGE_H

#include <QDialog>
#include <QLineEdit>
#include <QSqlTableModel>
#include <QComboBox>


class CSystemManage : public QDialog
{
    Q_OBJECT
public:
    explicit CSystemManage(QWidget *parent = 0);

private:
    QLineEdit *m_username;      //用户名lineedit
    QLineEdit *m_password;      //密码linedit
    QPushButton *m_ok;       //确认
    QPushButton *m_cancel;      //取消
    QComboBox   *m_role;        //职务
    QSqlTableModel *m_Model;     //数据库表结构
private:
    void InitSystemManage();           //初始化登录界面
signals:

public slots:
    void slotAddSystem();
    void slotExitManage();
};

#endif // CSYSTEMMANAGE_H
