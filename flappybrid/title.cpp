#include "title.h"

Title::Title(QObject *parent) :
    GameElement(parent)
{
    this->loadFrame();
    this->init();
    enabledDraw = true;
    connect(&timer,SIGNAL(timeout()),this,SLOT(updateFrame()));
    timer.start(150);
}

void Title::logic()
{
    if(!this->enabledLogic)
        return;
}

void Title::init()
{

    this->lastFrame = 0;
    this->currentFrame = 1;
}

void Title::draw(QPainter * painter)
{
    if(!this->enabledDraw)
        return;

    painter->drawPixmap((54.5/288)*this->windowRect.width(),
                        (150.0/512)*this->windowRect.height(),
                        (179.0/288)*this->windowRect.width(),
                        (49.0/512)*this->windowRect.height(),
                        this->pixmapList[3]);

    painter->drawPixmap((119.5/288)*this->windowRect.width(),
                        (200.0/512)*this->windowRect.height(),
                        (49.0/288)*this->windowRect.width(),
                        (49.0/512)*this->windowRect.height(),
                        this->pixmapList[this->currentFrame]);
}

void Title::resize(int _width, int _height)
{
        this->windowRect.setRect(0,0,_width,_height);
}

void Title::updateFrame()
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

void Title::loadFrame()
{
    this->clearAllFrame();
    this->addFrame(QPixmap(":/image/resource/brid_image/bird0_0.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/bird0_1.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/bird0_2.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/title.png"));
}
