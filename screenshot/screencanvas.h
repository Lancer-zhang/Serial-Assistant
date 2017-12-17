#ifndef SCREENCANVAS_H
#define SCREENCANVAS_H

#include <QWidget>
#include <QScreen>
#include <QGuiApplication>
#include <QWindow>
#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPaintEvent>
#include <QPointF>
#include <QRectF>
#include "constant.h"

class ScreenCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit ScreenCanvas(QWidget *parent = 0);
    void setBackgroundPixmap(QPixmap pixmap);

private:
    int m_ScreenWidth;
    int m_ScreenHeight;


    QPointF m_StartPoint;     //鼠标绘制起点
    QPointF m_EndPoint;     //鼠标绘制终点
    //RectPaint shotArea;    //截图区域
    int m_Flag;
    QPixmap m_FullPixmap;
    QPixmap m_FinishPixmap;
    QRectF m_ShotArea;


    void initCanvas();
    QRectF getShotArea(QPointF p1, QPointF p2);
    QPixmap shotScreen(QRectF &rect);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *e);


signals:
    void shotScreenFinish(QPixmap pixmap);
public slots:
};

#endif // SCREENCANVAS_H
