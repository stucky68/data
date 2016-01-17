#ifndef CLOGIN_H
#define CLOGIN_H

#include <QDialog>
#include <QLineEdit>
#include <QSqlTableModel>

class CLogin : public QDialog
{
    Q_OBJECT
public:
    explicit CLogin(QWidget *parent = 0);
    bool IsLogin();             //是否登录成功
public:
    QString m_user;             //登录用户名
private:
    QLineEdit *m_username;      //用户名lineedit
    QLineEdit *m_password;      //密码linedit
    QPushButton *m_login;       //确认
    QPushButton *m_logout;      //取消
    QSqlTableModel *m_Model;     //数据库表结构
    bool m_loginOrnot;          //是否登录成功
private:
    void InitLogin();           //初始化登录界面
signals:

public slots:
    void slotLogin();
    void slotLogOut();
};


#endif // CLOGIN_H
