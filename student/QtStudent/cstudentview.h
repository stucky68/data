#ifndef CSTUDENTVIEW_H
#define CSTUDENTVIEW_H

#include <QWidget>

class CStudentView : public QWidget
{
    Q_OBJECT
public:
    explicit CStudentView(QWidget *parent = 0);

signals:

public slots:
    void paintEvent(QPaintEvent *);
};

#endif // CSTUDENTVIEW_H
