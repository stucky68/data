#include <QPainter>
#include <QPixmap>

#include "cstudentview.h"

CStudentView::CStudentView(QWidget *parent) : QWidget(parent)
{

}

void CStudentView::paintEvent(QPaintEvent *)
{
    QPixmap pixmap("../student.png");
    QPainter p(this);
    p.drawPixmap(0, 0, 639, 343, pixmap);
}

