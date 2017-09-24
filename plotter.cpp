#include <QtGui>
#include <cmath>

#include "plotter.h"

Plotter::Plotter(QMainWindow *parent)
    : QMainWindow(parent)
{
    setBackgroundRole(QPalette::Shadow);
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);   //设置窗口不见得大小策略设置，这个窗口可以放大也可以缩小，
    setFocusPolicy(Qt::StrongFocus);
    m_RubberBandIsShown = false;

    m_ZoomInButton = new QToolButton(this);
    m_ZoomInButton->setIcon(QIcon(":/zoomin"));
    m_ZoomInButton->adjustSize();
    connect(m_ZoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));
    m_ZoomOutButton = new QToolButton(this);
    m_ZoomOutButton->setIcon(QIcon(":/zoomout"));
    m_ZoomOutButton->adjustSize();
    connect(m_ZoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));

    setPlotSettings(PlotSettings());
}

void Plotter::setPlotSettings(const PlotSettings &settings)
{
    m_ZoomStack.clear();
    m_ZoomStack.append(settings);
    m_CurZoom = 0;
    m_ZoomInButton->hide();
    m_ZoomOutButton->hide();
    refreshPixmap();
}

void Plotter::zoomOut()
{
    if (m_CurZoom > 0) {
        --m_CurZoom;
        m_ZoomOutButton->setEnabled(m_CurZoom > 0);
        m_ZoomInButton->setEnabled(true);
        m_ZoomInButton->show();
        refreshPixmap();
    }
}

void Plotter::zoomIn()
{
    if (m_CurZoom < m_ZoomStack.count() - 1) {
        ++m_CurZoom;
        m_ZoomInButton->setEnabled(m_CurZoom < m_ZoomStack.count() - 1);
        m_ZoomOutButton->setEnabled(true);
        m_ZoomOutButton->show();
        refreshPixmap();
    }
}
//设置用于给定曲线id中的数据
void Plotter::setCurveData(int id, const QVector<QPointF> &data)
{
    m_CurveMap[id] = data;
    refreshPixmap();
}
//从curvemap中移除一条给定的曲线
void Plotter::clearCurve(int id)
{
    m_CurveMap.remove(id);
    refreshPixmap();
}

QSize Plotter::minimumSizeHint() const
{
    return QSize(6 * Margin, 4 * Margin);
}

QSize Plotter::sizeHint() const
{
    return QSize(12 * Margin, 8 * Margin);
}

void Plotter::paintEvent(QPaintEvent * /* event */)
{
    QStylePainter painter(this);
    painter.drawPixmap(0, 0, m_Pixmap);

    if (m_RubberBandIsShown) {
        painter.setPen(palette().light().color());
        painter.drawRect(m_RubberBandRect.normalized().adjusted(0, 0, -1, -1));
    }

    if (hasFocus()) {
        QStyleOptionFocusRect option;
        option.initFrom(this);
        option.backgroundColor = palette().dark().color();
        painter.drawPrimitive(QStyle::PE_FrameFocusRect, option);
    }
}

void Plotter::resizeEvent(QResizeEvent * /* event */)
{
    int x = width() - (m_ZoomInButton->width() + m_ZoomOutButton->width() + 10);
    m_ZoomInButton->move(x/2, 5);
    m_ZoomOutButton->move(x/2 + m_ZoomInButton->width() + 5, 5);
    refreshPixmap();
}

void Plotter::mousePressEvent(QMouseEvent *event)
{
    QRect rect(40, Margin, width() - 40 - Margin, height() - 2 * Margin);

    if (event->button() == Qt::LeftButton) {
        if (rect.contains(event->pos())) {
            m_RubberBandIsShown = true;
            m_RubberBandRect.setTopLeft(event->pos());
            m_RubberBandRect.setBottomRight(event->pos());
            updateRubberBandRegion();
            setCursor(Qt::CrossCursor);
        }
    }
}

void Plotter::mouseMoveEvent(QMouseEvent *event)
{
    if (m_RubberBandIsShown) {
        updateRubberBandRegion();
        m_RubberBandRect.setBottomRight(event->pos());
        updateRubberBandRegion();
    }
}

void Plotter::mouseReleaseEvent(QMouseEvent *event)
{
    if ((event->button() == Qt::LeftButton) && m_RubberBandIsShown) {
        m_RubberBandIsShown = false;
        updateRubberBandRegion();
        unsetCursor();

        QRect rect = m_RubberBandRect.normalized();
        if (rect.width() < 4 || rect.height() < 4)
            return;
        rect.translate(-Margin, -Margin);

        PlotSettings prevSettings = m_ZoomStack[m_CurZoom];
        PlotSettings settings;
        double dx = prevSettings.spanX() / (width() - 2 * Margin);
        double dy = prevSettings.spanY() / (height() - 2 * Margin);
        settings.minX = prevSettings.minX + dx * rect.left();
        settings.maxX = prevSettings.minX + dx * rect.right();
        settings.minY = prevSettings.maxY - dy * rect.bottom();
        settings.maxY = prevSettings.maxY - dy * rect.top();
        settings.adjust();

        m_ZoomStack.resize(m_CurZoom + 1);
        m_ZoomStack.append(settings);
        zoomIn();
    }
}

void Plotter::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Left:
        m_ZoomStack[m_CurZoom].scroll(-1, 0);
        refreshPixmap();
        break;
    case Qt::Key_Right:
        m_ZoomStack[m_CurZoom].scroll(+1, 0);
        refreshPixmap();
        break;
    case Qt::Key_Down:
        m_ZoomStack[m_CurZoom].scroll(0, -1);
        refreshPixmap();
        break;
    case Qt::Key_Up:
        m_ZoomStack[m_CurZoom].scroll(0, +1);
        refreshPixmap();
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void Plotter::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numTicks = numDegrees / 15;

    if (event->orientation() == Qt::Horizontal) {
        m_ZoomStack[m_CurZoom].scroll(numTicks, 0);
    } else {
        m_ZoomStack[m_CurZoom].scroll(0, numTicks);
    }
    refreshPixmap();
}

void Plotter::updateRubberBandRegion()
{
    QRect rect = m_RubberBandRect.normalized();
    update(rect.left(), rect.top(), rect.width(), 1);
    update(rect.left(), rect.top(), 1, rect.height());
    update(rect.left(), rect.bottom(), rect.width(), 1);
    update(rect.right(), rect.top(), 1, rect.height());
}

void Plotter::refreshPixmap()
{
    m_Pixmap = QPixmap(size());
    m_Pixmap.fill(this, 0, 0);
    QPainter painter(&m_Pixmap);
    QRect rect(0, 0, width() , height());
    painter.eraseRect(rect);

    painter.initFrom(this);
    drawGrid(&painter);

    drawCurves(&painter);
    update();
}

void Plotter::drawGrid(QPainter *painter)
{
    QRect rect(40, Margin, width() - 40 - Margin, height() - 2 * Margin);
    if (!rect.isValid())
        return;

    PlotSettings settings = m_ZoomStack[m_CurZoom];
    QPen quiteDark(Qt::black);
    QPen light = palette().light().color();

    for (int i = 0; i <= settings.numXTicks; ++i) {
        int x = rect.left() + (i * (rect.width() - 1) / settings.numXTicks);
        double label = settings.minX + (i * settings.spanX() / settings.numXTicks);
        painter->setPen(quiteDark);
        painter->drawLine(x, rect.top(), x, rect.bottom());
        painter->drawLine(x, rect.bottom(), x, rect.bottom() + 5);
        painter->drawText(x - 50, rect.bottom() + 5, 100, 20, Qt::AlignHCenter | Qt::AlignTop, QString::number(label));
    }
    for (int j = 0; j <= settings.numYTicks; ++j) {
        int y = rect.bottom() - (j * (rect.height() - 1) / settings.numYTicks);
        double label = settings.minY + (j * settings.spanY() / settings.numYTicks);
        painter->setPen(quiteDark);
        painter->drawLine(rect.left(), y, rect.right(), y);
        painter->drawLine(rect.left() - 5, y, rect.left(), y);
        painter->drawText(rect.left() - 40, y - 10, 40, 20, Qt::AlignRight | Qt::AlignVCenter, QString::number(label));
    }
    painter->drawRect(rect.adjusted(0, 0, -1, -1));
}

void Plotter::drawCurves(QPainter *painter)
{
    static const QColor colorForIds[6] = {
        Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow
    };
    PlotSettings settings = m_ZoomStack[m_CurZoom];
    QRect rect(40, Margin, width() - 40 - Margin, height() - 2 * Margin);
    if (!rect.isValid())
        return;

    painter->setClipRect(rect.adjusted(+1, +1, -1, -1));

    QMapIterator<int, QVector<QPointF> > i(m_CurveMap);
    while (i.hasNext()) {
        i.next();

        int id = i.key();
        QVector<QPointF> data = i.value();
        QPolygonF polyline(data.count());

        for (int j = 0; j < data.count(); ++j) {
            double dx = data[j].x() - settings.minX;
            double dy = data[j].y() - settings.minY;
            double x = rect.left() + (dx * (rect.width() - 1)  / settings.spanX());
            double y = rect.bottom() - (dy * (rect.height() - 1) / settings.spanY());
            polyline[j] = QPointF(x, y);
        }
        painter->setPen(colorForIds[uint(id) % 6]);
        painter->drawPolyline(polyline);
    }
}

PlotSettings::PlotSettings()
{
    minX = 0.0;
    maxX = 10.0;
    numXTicks = 10;

    minY = 0.0;
    maxY = 10.0;
    numYTicks = 6;
}

void PlotSettings::scroll(int dx, int dy)
{
    double stepX = spanX() / numXTicks;
    minX += dx * stepX;
    maxX += dx * stepX;

    double stepY = spanY() / numYTicks;
    minY += dy * stepY;
    maxY += dy * stepY;
}

void PlotSettings::adjust()
{
    adjustAxis(minX, maxX, numXTicks);
    adjustAxis(minY, maxY, numYTicks);
}

void PlotSettings::adjustAxis(double &min, double &max, int &numTicks)
{
    const int MinTicks = 4;
    double grossStep = (max - min) / MinTicks;
    double step = std::pow(10.0, std::floor(std::log10(grossStep)));

    if (5 * step < grossStep) {
        step *= 5;
    } else if (2 * step < grossStep) {
        step *= 2;
    }

    numTicks = int(std::ceil(max / step) - std::floor(min / step));
    if (numTicks < MinTicks)
        numTicks = MinTicks;
    min = std::floor(min / step) * step;
    max = std::ceil(max / step) * step;
}
