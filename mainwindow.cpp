#include "mainwindow.h"
#include <QPainter>
#include "gdatafactory.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setFont(QFont("KaiTi",10,QFont::Bold));
    resize(1000,800);
    this->setWindowTitle("AOI设备运控");
    this->setCentralWidget(GDataFactory::get_central_wgt());
}

MainWindow::~MainWindow()
{

}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(this->rect(),QBrush(QColor(0,0,0)));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
}
