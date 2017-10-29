#include "brid.h"

Brid::Brid(QObject *parent) :
    GameElement(parent)
{
    this->init();
    connect(&timer,SIGNAL(timeout()),this,SLOT(updateFrame()));
}

void Brid::init()
{
    this->loadFrame();
    this->lastFrame = 0;
    this->currentFrame = 1;
    this->angle = 0;
    this->speedX = (10.0/288)*this->windowRect.width();
    this->speedY = -this->windowRect.height() * (1.0/50);
    this->increaseY = this->windowRect.height() * (1.0/500);
    this->bindRect.setRect(this->windowRect.width()*(1.0/5), this->windowRect.height()*(220.0/512),
                      this->windowRect.width()*(49.0/288), this->windowRect.height()*(49.0/512));
    this->timer.start(150);
}

void Brid::logic()
{
    if( !this->enabledLogic )
    {
        return;
    }
            //计算小鸟飞行时的角度
            this->angle = qAtan2(this->speedY,this->speedX)*180.0/3.14159;
            if(this->angle < -10)
                this->angle = -25;
            else if(this->angle > 60)
                this->angle = 90;
            //不点击界面时，小鸟自由下落的速度
            this->speedY += this->increaseY;
            //限制小鸟下降时的最快速度
            if(this->speedY > this->windowRect.height() * (14.0/512))
            {
                this->speedY = this->windowRect.height() * (14.0/512);
                this->angle = 90;
            }
            //每次绘制时，小鸟Y方向移动的距离
            this->bindRect.translate(0,this->speedY);
            //限制小鸟向上飞出屏幕的最大高度
            if(this->bindRect.top() < -this->bindRect.height())
                this->bindRect.moveTop(-this->bindRect.height());
            //小鸟落地
            if(this->bindRect.bottom() > this->windowRect.height()*(415.0/512))
            {
                this->bindRect.moveBottom(this->windowRect.height()*(415.0/512));
                this->timer.stop();
            }
            return;
}

void Brid::resize(int _width, int _height)
{
    this->bindRect.setRect(_width*(1.0/5), _height*(this->bindRect.y()/this->windowRect.height()), _width*(48.0/288), _height*(49.0/512));
    this->speedX = this->speedX/this->windowRect.width()*_width;
    this->speedY = this->speedY/this->windowRect.height()*_height;
    this->increaseY = this->increaseY/this->windowRect.height()*_height;
    this->windowRect.setRect(0,0,_width,_height);
}

void Brid::draw(QPainter * painter)
{
    if( !this->enabledDraw )
    {
        return;
    }
        painter->save();
        painter->translate(this->bindRect.center());
        painter->rotate(this->angle);
        painter->translate(-this->bindRect.center());
        painter->drawPixmap(this->bindRect.x(), this->bindRect.y(), this->bindRect.width(),this->bindRect.height(),
                            this->pixmapList[this->currentFrame]);
        painter->restore();
}

void Brid::loadFrame()
{
    this->clearAllFrame();
    switch(qrand()%3)
    {
    case 0:
        this->addFrame(QPixmap(":/image/resource/brid_image/bird0_0.png"));
        this->addFrame(QPixmap(":/image/resource/brid_image/bird0_1.png"));
        this->addFrame(QPixmap(":/image/resource/brid_image/bird0_2.png"));
        break;
    case 1:
        this->addFrame(QPixmap(":/image/resource/brid_image/bird1_0.png"));
        this->addFrame(QPixmap(":/image/resource/brid_image/bird1_1.png"));
        this->addFrame(QPixmap(":/image/resource/brid_image/bird1_2.png"));
        break;
    case 2:
        this->addFrame(QPixmap(":/image/resource/brid_image/bird2_0.png"));
        this->addFrame(QPixmap(":/image/resource/brid_image/bird2_1.png"));
        this->addFrame(QPixmap(":/image/resource/brid_image/bird2_2.png"));
        break;
    }
}

void Brid::updateFrame()
{
    if( !this->enabledDraw )
        return;

    if(this->currentFrame > this->lastFrame)
    {
        this->lastFrame = this->currentFrame;
        this->currentFrame++;
    }
    else if(this->currentFrame < this->lastFrame)
    {
        this->lastFrame = this->currentFrame;
        this->currentFrame--;
    }

    if(this->currentFrame<0 || this->currentFrame>2)
        this->currentFrame = 1;
}

void Brid::bridUp()
{
    this->speedY = -this->windowRect.height() * (1.0/48);
}
