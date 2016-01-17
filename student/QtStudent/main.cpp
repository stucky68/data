#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>

#include "cstudent.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("jiangxin");
    db.setDatabaseName("StudentManage");
    bool bRet = db.open();
    if (!bRet)
    {
        QMessageBox::critical(NULL, "错误", db.lastError().text());
        exit(0);
    }

    CStudent istudent;
    istudent.resize(639, 393);
    istudent.show();
    return app.exec();
}


