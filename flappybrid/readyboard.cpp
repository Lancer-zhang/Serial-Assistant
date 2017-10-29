#include "readyboard.h"

ReadyBoard::ReadyBoard(QObject *parent) :
    GameElement(parent)
{
    this->addFrame(QPixmap(":/image/resource/brid_image/text_ready.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/tutorial.png"));
    this->init();
}

void ReadyBoard::init()
{

}

void ReadyBoard::logic()
{
    if(!this->enabledLogic)
    {
        return;
    }
}

void ReadyBoard::draw(QPainter * painter)
{
    if(!this->enabledDraw)
    {
        return;
    }
    painter->drawPixmap((45.0/288)*this->windowRect.width(),
                        (145.0/512)*this->windowRect.height(),
                        (197.0/288)*this->windowRect.width(),
                        (63.0/512)*this->windowRect.height(),
                        this->pixmapList[0]);
    painter->drawPixmap((86.5/288)*this->windowRect.width(),
                        (220.0/512)*this->windowRect.height(),
                        (115.0/288)*this->windowRect.width(),
                        (99.0/512)*this->windowRect.height(),
                        this->pixmapList[1]);
}

void ReadyBoard::resize(int _width, int _height)
{
    this->windowRect.setRect(0,0,_width,_height);
}

