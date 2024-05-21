#include "headinfowgt.h"
#include <QPainter>
#include <QPainterPath>
#include <QPen>
HeadInfoWgt* HeadInfoWgt::m_pInstance = nullptr;
HeadInfoWgt::HeadInfoWgt(QWidget *parent) : QWidget(parent)
{
    this->setMaximumHeight(100);
}

void HeadInfoWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    QPainterPath path;
    path.addRoundedRect(this->rect(),10,10);
    painter.setClipPath(path);
    painter.fillPath(path,QBrush(QColor(0,0,0)));
    painter.setPen(QPen(QColor(0,255,0,150),1.5));
    painter.drawPath(path);

    painter.setPen(QPen(QColor(0,255,0)));
    painter.setFont(QFont(u8"LiSu",40,QFont::Bold,false));
    painter.drawText(this->rect(),Qt::AlignHCenter|Qt::AlignVCenter,u8"AOI设备运控");
}
