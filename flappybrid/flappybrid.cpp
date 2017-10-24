#include "flappybrid.h"

FlappyBrid::FlappyBrid(QWidget *parent) : QWidget(parent)
{
    qsrand(QTime::currentTime().second());
this->resize(400,700);
    this->startButton = new QPushButton(this);
    this->startButton->setGeometry(QRect((20.0/288)*this->width(),
                                   (341.0/512)*this->height(),
                                   (117.0/288)*this->width(),
                                   (71.0/512)*this->height()));
    this->startButton->setStyleSheet("QPushButton{border-image:url(:/image/resource/brid_image/button_play.png);}"
                                 "QPushButton:pressed{margin: 2px 2px 2px 2px;}");

    this->scoreButton = new QPushButton(this);
    this->scoreButton->setGeometry(QRect((151.0/288)*this->width(),
                                         (341.0/512)*this->height(),
                                         (117.0/288)*this->width(),
                                         (71.0/512)*this->height()));
    this->scoreButton->setStyleSheet("QPushButton{border-image:url(:/image/resource/brid_image/button_score.png);}"
                                 "QPushButton:pressed{margin: 2px 2px 2px 2px;}");

    this->rateButton = new QPushButton(this);
    this->rateButton->setGeometry(QRect((106.5/288)*this->width(),
                                         (265.0/512)*this->height(),
                                         (75.0/288)*this->width(),
                                         (48.0/512)*this->height()));
    this->rateButton->setStyleSheet("QPushButton{border-image:url(:/image/resource/brid_image/button_rate.png);}"
                                 "QPushButton:pressed{margin: 2px 2px 2px 2px;}");

    this->background = new Background();
    this->ground = new Ground();
    this->brid = new Brid();
    this->title = new Title();
    connect(&timer,SIGNAL(timeout()),this,SLOT(update()));
    timer.start(30);
}

void FlappyBrid::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    this->background->logic();
    this->background->draw(&painter);

    this->ground->logic();
    this->ground->draw(&painter);

    this->brid->logic();
    this->brid->draw(&painter);

    this->title->logic();
    this->title->draw(&painter);
}

void FlappyBrid::resizeEvent(QResizeEvent *event)
{
    this->background->resize(this->width(),this->height());
    this->ground->resize(this->width(), this->height());
    this->brid->resize(this->width(), this->height());
    this->title->resize(this->width(),this->height());
}
