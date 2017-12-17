#include "screencanvas.h"
#include <QDebug>

ScreenCanvas::ScreenCanvas(QWidget *parent) : QWidget(parent)
{
        QDesktopWidget *desktop = QApplication::desktop();
        const QRect screenGeometry =  desktop->screenGeometry(this);
        m_ScreenWidth = screenGeometry.width();
        m_ScreenHeight = screenGeometry.height();
        this->raise();                      //软置顶
        this->showFullScreen();
        setMouseTracking(true);                 //鼠标移动捕捉
//        this->setCursor(Qt::CrossCursor);

        initCanvas();
}

void ScreenCanvas::setBackgroundPixmap(QPixmap pixmap)
{
    m_FullPixmap = pixmap;
}

void ScreenCanvas::initCanvas()
{
    m_StartPoint.setX(0);
    m_StartPoint.setY(0);
    m_EndPoint.setX(0);
    m_EndPoint.setY(0);
    m_ShotArea = getShotArea(m_StartPoint, m_EndPoint);
    m_Flag = DrawStatus::waitDraw;
}

QRectF ScreenCanvas::getShotArea(QPointF start, QPointF end)
{
    //通过任意两点构造一个矩形
    float x1,y1,x2,y2;
    if(start.x()<end.x())
    {
        x1=start.x();
        x2=end.x();
    }
    else
    {
        x1=end.x();
        x2=start.x();
    }

    if(start.y()<end.y())
    {
        y1=start.y();
        y2=end.y();
    }
    else
    {
        y1= end.y();
        y2=start.y();
    }

    QPointF ps(x1, y1);
    QPointF pe(x2, y2);
    QRectF rect(ps,pe);
    return rect;
}

QPixmap ScreenCanvas::shotScreen(QRectF &rect)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap pixmap;
//    if (const QWindow *window = windowHandle())
//        screen = window->screen();
    pixmap = screen->grabWindow(0,rect.x(),rect.y(),rect.width(),rect.height());
    return pixmap;
}

void ScreenCanvas::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(m_Flag == DrawStatus::waitDraw)
        {
            m_StartPoint.setX(event->x());
            m_StartPoint.setY(event->y());
            m_EndPoint.setX(event->x());
            m_EndPoint.setY(event->y());
            m_ShotArea = getShotArea(m_StartPoint, m_EndPoint);
            qDebug()<<"press   "<<m_ShotArea;
            m_Flag = DrawStatus::drawing;
        }
        else if(m_Flag == DrawStatus::drawed)
        {
            m_FinishPixmap = shotScreen(m_ShotArea);
            this->hide();
            emit shotScreenFinish(m_FinishPixmap);
            this->close();
        }
    }
    else if( event->button() == Qt::RightButton)
    {
        if(m_Flag == DrawStatus::waitDraw)
        {
            qDebug()<<"bujietule";
            this->close();
        }
//        else if(m_Flag == DrawStatus::drawed)
//        {
//            setCursor(Qt::CrossCursor);
//            initCanvas();
//            update();
//        }
    }

}

void ScreenCanvas::mouseMoveEvent(QMouseEvent *event)
{

    if(event->buttons() & Qt::LeftButton)
    {
        if(m_Flag == DrawStatus::drawing)
        {
            m_EndPoint.setX(event->x());
            m_EndPoint.setY(event->y());
            m_ShotArea = getShotArea(m_StartPoint, m_EndPoint);
            qDebug()<<"move  222 "<<m_ShotArea;
            update();
        }
        else if(m_Flag == DrawStatus::drawed)
        {
            //to do
        }
    }
}

void ScreenCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(m_Flag == DrawStatus::drawing)
        {
            m_EndPoint.setX(event->x());
            m_EndPoint.setY(event->y());
            m_ShotArea = getShotArea(m_StartPoint, m_EndPoint);
            m_Flag = DrawStatus::drawed;
            update();
        }
    }
}

void ScreenCanvas::paintEvent(QPaintEvent *e)
{
    qDebug()<<"painter";
    QPainter painter(this);
    QPixmap tempmask(m_ScreenWidth, m_ScreenHeight);
    tempmask.fill((QColor(0, 0, 0, 160)));

    painter.drawPixmap(0, 0, m_FullPixmap);    //先绘制全屏原图背景
    painter.drawPixmap(0, 0, tempmask);            //然后绘制半透明背景，用来降低亮度
    switch(m_Flag)
    {
    case DrawStatus::waitDraw:
    {
        break;
    }
    case DrawStatus::drawing:
    {
        painter.setPen(QPen(Qt::red,5,Qt::DashLine));//设置画笔形式
        painter.drawRect(m_ShotArea);            //然后绘制矩形框
       qDebug()<<m_ShotArea.bottomLeft()<<m_ShotArea.topRight();
        painter.drawPixmap(m_ShotArea, m_FullPixmap, m_ShotArea);     //然后将矩形框中的半透明图像替换成原图
        break;
    }
    case DrawStatus::drawed:
    {
        painter.setPen(QPen(Qt::green,2,Qt::DashLine));//设置画笔形式
        //painter.setBrush(Qt::white);
        painter.drawRect(m_ShotArea);            //然后绘制矩形框
        painter.drawPixmap(m_ShotArea, m_FullPixmap, m_ShotArea);     //然后将矩形框中的半透明图像替换成原图
        break;
    }
    default:
        break;
    }
       QWidget::paintEvent(e);
}
