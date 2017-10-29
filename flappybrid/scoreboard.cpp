#include "scoreboard.h"

ScoreBoard::ScoreBoard(QObject *parent) :
    GameElement(parent)
{
    loadFrame();
    init();
}

void ScoreBoard::init()
{
    this->score = 0;
    this->bindRect.setRect((131.5/288)*this->windowRect.width(),
                           (80.0/512)*this->windowRect.height(),
                           (25.0/288)*this->windowRect.width(),
                           (45.0/512)*this->windowRect.height());
    this->tempUnit = 0;
    this->tempDecade = 0;
    this->tempHundred = 0;
}

void ScoreBoard::logic()
{
    if(!this->enabledLogic)
    {
        return;
    }
    else
    {
        this->tempUnit = this->score%10;
        this->tempDecade = (this->score%100)/10;
        this->tempHundred = (this->score%1000)/100;
    }
}

void ScoreBoard::draw(QPainter * painter)
{
    if(!this->enabledDraw)
    {
        return;
    }
    else
    {
        if(this->tempDecade == 0 && this->tempHundred == 0)
        {
            painter->drawPixmap(this->bindRect.x(),
                                this->bindRect.y(),
                                this->bindRect.width(),
                                this->bindRect.height(),
                                this->pixmapList[this->tempUnit]);
        }
        else if(this->tempHundred == 0)
        {
            painter->drawPixmap(this->bindRect.x()-14.0,
                                this->bindRect.y(),
                                this->bindRect.width(),
                                this->bindRect.height(),
                                this->pixmapList[this->tempDecade]);
            painter->drawPixmap(this->bindRect.x()+14.0,
                                this->bindRect.y(),
                                this->bindRect.width(),
                                this->bindRect.height(),
                                this->pixmapList[this->tempUnit]);
        }
        else
        {
            painter->drawPixmap(this->bindRect.x()-26.0,
                                this->bindRect.y(),
                                this->bindRect.width(),
                                this->bindRect.height(),
                                this->pixmapList[this->tempHundred]);
            painter->drawPixmap(this->bindRect.x(),
                                this->bindRect.y(),
                                this->bindRect.width(),
                                this->bindRect.height(),
                                this->pixmapList[this->tempDecade]);
            painter->drawPixmap(this->bindRect.x()+27.0,
                                this->bindRect.y(),
                                this->bindRect.width(),
                                this->bindRect.height(),
                                this->pixmapList[this->tempUnit]);
        }
    }
}

void ScoreBoard::resize(int _width, int _height)
{
    this->bindRect.setRect((131.5/288)*_width,
                           (80.0/512)*_height,
                           (25.0/288)*_width,
                           (45.0/512)*_height);
    this->windowRect.setRect(0,0,_width,_height);
}

void ScoreBoard::loadFrame()
{
    this->clearAllFrame();
    this->addFrame(QPixmap(":/image/resource/brid_image/font_048.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/font_049.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/font_050.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/font_051.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/font_052.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/font_053.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/font_054.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/font_055.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/font_056.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/font_057.png"));
}
void ScoreBoard::setScore(int score)
{
    this->score = score;
}
