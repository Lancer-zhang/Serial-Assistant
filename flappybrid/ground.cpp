#include "ground.h"

Ground::Ground(QObject *parent) :
    GameElement(parent)
{
    this->addFrame(QPixmap(":/image/resource/brid_image/land.png"));
    this->init();
}

void Ground::init()
{
    this->currentFrame = 0;
    this->speedX = (3.0/288)*this->windowRect.width();
}

void Ground::logic()
{
    if( !this->enabledLogic )
        return;

    this->positionX -= this->speedX;
    if(positionX < -this->bindRect.width() )
        this->positionX = 0;
}

void Ground::resize(int _width, int _height)
{
    this->bindRect.setRect(0,_height*(400.0/512),_width,_height*(112.0/512) );
    this->positionX = 0;
    this->speedX = (float)this->speedX/this->windowRect.width()*_width;
    this->windowRect.setRect(0,0,_width,_height);
}

void Ground::draw(QPainter *painter)
{
    painter->drawPixmap(this->positionX,
                        this->bindRect.topLeft().y(),
                        this->bindRect.width(),this->bindRect.height(),
                        this->pixmapList[this->currentFrame]);
    painter->drawPixmap(this->positionX + this->bindRect.width()-2,
                        this->bindRect.topLeft().y(),
                        this->bindRect.width(),this->bindRect.height(),
                        this->pixmapList[this->currentFrame]);
}

