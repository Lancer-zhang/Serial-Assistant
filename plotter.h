#ifndef PLOTTER_H
#define PLOTTER_H

#include <QMap>
#include <QPixmap>
#include <QVector>
#include <QWidget>
#include <QToolButton>
#include <QStylePainter>
#include <QStyleOptionFocusRect>
#include <QStyle>
#include <QMainWindow>
class QToolButton;
class PlotSettings;
namespace Ui {
    class MainWindow;
}
class Plotter : public QMainWindow
{
    Q_OBJECT

public:
    Plotter(QMainWindow *parent = 0);

    void setPlotSettings(const PlotSettings &settings);
    void setCurveData(int id, const QVector<QPointF> &data);
    void clearCurve(int id);//创建绘图区
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public slots:
    void zoomIn();
    void zoomOut();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);//重新实现qwidget事件处理器

private:
    void updateRubberBandRegion();//更新橡皮筋区域
    void refreshPixmap();
    void drawGrid(QPainter *painter);
    void drawCurves(QPainter *painter);

    enum { Margin = 25 };//为图形区域周围提供一些空间

    QToolButton *m_ZoomInButton;
    QToolButton *m_ZoomOutButton;
    QMap<int, QVector<QPointF> > m_CurveMap;
    QVector<PlotSettings> m_ZoomStack;
    int m_CurZoom;
    bool m_RubberBandIsShown;
    QRect m_RubberBandRect;
    QPixmap m_Pixmap;//对整个窗口部件的绘制数据的进行了复制保存
};

class PlotSettings
{
public:
    PlotSettings();

    void scroll(int dx, int dy);
    void adjust();
    double spanX() const { return maxX - minX; }
    double spanY() const { return maxY - minY; }

    double minX;
    double maxX;
    int numXTicks;
    double minY;
    double maxY;
    int numYTicks;

private:
    static void adjustAxis(double &min, double &max, int &numTicks);
};

#endif
