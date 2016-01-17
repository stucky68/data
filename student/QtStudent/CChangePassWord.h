#ifndef CCHANGEPASSWORD_H
#define CCHANGEPASSWORD_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlTableModel>


class CChangePassWord : public QDialog
{
    Q_OBJECT
public:
    explicit CChangePassWord(QWidget *parent = 0);
    CChangePassWord(QString username, QWidget *parent = 0);

private:
    QLineEdit *m_Oldpassword;      //当前密码
    QLineEdit *m_Newpassword;      //新密码
    QPushButton *m_change;         //确认
    QPushButton *m_exit;           //取消
    QSqlTableModel *m_Model;       //数据库表结构
private:
    void InitChangePassWord();           //初始化登录界面
private:
    QString m_user;
signals:

public slots:
    void slotChange();
    void slotExit();
};

#endif // CCHANGEPASSWORD_H
