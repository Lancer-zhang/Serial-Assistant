#include "background.h"

Background::Background(QObject *parent) :
    GameElement(parent)
{
    init();
}

void Background::init()
{
    this->loadFrame();
    this->currentFrame = 0;
    this->enabledDraw = true;
}

void Background::loadFrame()
{
    this->clearAllFrame();
    if(qrand()%2)
    {
        this->addFrame(QPixmap(":/image/resource/brid_image/bg_day.png"));
    }
    else
    {
        this->addFrame(QPixmap(":/image/resource/brid_image/bg_night.png"));
    }
}

void Background::draw(QPainter * painter)
{
    if(!this->enabledDraw)
        return;
    painter->drawPixmap(this->bindRect.x(),this->bindRect.y(),
                        this->bindRect.width(),this->bindRect.height(),
                        this->pixmapList[this->currentFrame]);
}

void Background::logic()
{

}

void Background::resize(int _width, int _height)
{
    this->bindRect.setRect(0,0,_width,_height);
    this->windowRect.setRect(0,0,_width,_height);
}
