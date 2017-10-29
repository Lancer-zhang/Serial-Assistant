#include "pipe.h"
#include <iostream>
using namespace std;

Pipe::Pipe(int pos = 0,QObject *parent) :
    GameElement(parent)
{
    this->startPosition = pos;
    this->loadFrame();
    this->init();
}

void Pipe::init()
{
    this->currentFrame = 0;
    this->pipePassEmited = false;
    this->pipeRect[above].setRect(2*this->windowRect.width() + this->startPosition*(175.6/288)*this->windowRect.width(),
                              this->windowRect.height()*((-271.0 + qrand()%200)/512), // 50 - 321 = 271.0
                              this->windowRect.width()*(52.0/288),
                              this->windowRect.height()*(321.0/512));
    this->pipeRect[following].setRect(2*this->windowRect.width() + this->startPosition*(175.6/288)*this->windowRect.width(),
                              this->pipeRect[above].bottom() + (100.0/512)*this->windowRect.height(), // Distance = 100
                              this->windowRect.width()*(52.0/288),
                              this->windowRect.height()*(321.0/512));
    this->speedX = (3.0/288)*this->windowRect.width();
}

void Pipe::logic()
{
    if(!this->enabledLogic)
        return;

    this->pipeRect[above].translate(-this->speedX,0);
    this->pipeRect[following].translate(-this->speedX,0);

    if(this->pipeRect[above].right() < 0)
    {
        this->pipeRect[above].moveTo((474.0/288)*this->windowRect.width(),
                                     this->windowRect.height()*((-271.0 + qrand()%200)/512));
        this->pipeRect[following].moveTo((474.0/288)*this->windowRect.width(),
                                         this->pipeRect[above].bottom() + (100.0/512)*this->windowRect.height());
        this->pipePassEmited = false;
    }

    if(this->pipeRect[above].left() < this->windowRect.width()*(1.0/4) && this->pipePassEmited == false)
    {
        emit pipePass();
        this->pipePassEmited = true;
    }
}

void Pipe::draw(QPainter *painter)
{
    painter->drawPixmap(this->pipeRect[above].x(),
                        this->pipeRect[above].y(),
                        this->pipeRect[above].width(),
                        this->pipeRect[above].height(),
                        this->pixmapList[0]);
    painter->drawPixmap(this->pipeRect[following].x(),
                        this->pipeRect[following].y(),
                        this->pipeRect[following].width(),
                        this->pipeRect[following].height(),
                        this->pixmapList[1]);
}

void Pipe::resize(int _width, int _height)
{
    this->pipeRect[above].setRect(this->pipeRect[above].x()/this->windowRect.width()*_width,
            this->pipeRect[above].y()/this->windowRect.height()*_height,
            _width*(52.0/288),
            _height*(321.0/512));
    this->pipeRect[following].setRect(this->pipeRect[following].x()/this->windowRect.width()*_width,
            this->pipeRect[following].y()/this->windowRect.height()*_height,
            _width*(52.0/288),
            _height*(321.0/512));
    this->speedX = (float)this->speedX/this->windowRect.width()*_width;
    this->windowRect.setRect(0,0,_width,_height);
}

void Pipe::loadFrame()
{
    this->clearAllFrame();
    this->addFrame(QPixmap(":/image/resource/brid_image/pipe_down.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/pipe_up.png"));
}

QRectF& Pipe::getRect(PipeType type)
{
        return this->pipeRect[type];
}

