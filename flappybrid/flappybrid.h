#ifndef FLAPPYBRID_H
#define FLAPPYBRID_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>
#include <QTime>
#include "background.h"
#include "ground.h"
#include "brid.h"
#include "title.h"
#include <QResizeEvent>
#include <QPainter>

class FlappyBrid : public QWidget
{
    Q_OBJECT
public:
    explicit FlappyBrid(QWidget *parent = 0);

    void paintEvent(QPaintEvent *);
    //void mousePressEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);

private:
    QPushButton *startButton;
    QPushButton *scoreButton;
    QPushButton *rateButton;

    Background *background;
    Ground *ground;
    Brid *brid;
    Title *title;


    QTimer timer;

signals:

public slots:
};

#endif // FLAPPYBRID_H
