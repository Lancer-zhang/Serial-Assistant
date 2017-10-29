#include "overboard.h"

OverBoard::OverBoard(QObject *parent) :
    GameElement(parent)
{
    loadFrame();
    init();
}
void OverBoard::init()
{
    this->score = 0;
    this->medal = NONE;
    this->scoreLabelArrived = false;
    this->overTextRect.setRect((41.0/288)*this->windowRect.width(),
                               -1*this->windowRect.height(),
                               (205.0/288)*this->windowRect.width(),
                               (55.0/512)*this->windowRect.height());
    this->scoreLabelRect.setRect((25.0/288)*this->windowRect.width(),
                                 2*this->windowRect.height(),
                                 (238.0/288)*this->windowRect.width(),
                                 (127.0/512)*this->windowRect.height());
}

void OverBoard::logic()
{
    if(!this->enabledLogic)
        return;


    if(this->overTextRect.y() < (145.0/512)*this->windowRect.height())
        this->overTextRect.translate(0,(20.0/512)*this->windowRect.height());
    else
        this->overTextRect.setY((145.0/512)*this->windowRect.height());

    if(this->scoreLabelRect.y() > (204.0/512)*this->windowRect.height())
    {
        this->scoreLabelRect.translate(0,-(25.0/512)*this->windowRect.height());
    }
    else
    {
        this->scoreLabelRect.setY((204.0/512)*this->windowRect.height());
        this->scoreLabelArrived = true;
    }
}

void OverBoard::draw(QPainter *painter)
{
    if(!this->enabledDraw)
        return;

    painter->drawPixmap(this->overTextRect.x(),
                        this->overTextRect.y(),
                        this->overTextRect.width(),
                        this->overTextRect.height(),
                        this->pixmapList[0]);
    painter->drawPixmap(this->scoreLabelRect.x(),
                        this->scoreLabelRect.y(),
                        this->scoreLabelRect.width(),
                        this->scoreLabelRect.height(),
                        this->pixmapList[1]);

    if( !this->scoreLabelArrived )
        return;

    //score
    //当有三位数的时候，需要把十位再draw一次，防止103这样的数，只判断"(this->score%100)/10"是不行的
    if((this->score%1000)/100 != 0)
    {
        painter->drawPixmap((184.0/288)*this->windowRect.width(),
                            (237.0/512)*this->windowRect.height(),
                            (17.0/288)*this->windowRect.width(),
                            (21.0/512)*this->windowRect.height(),
                            this->pixmapList[(this->score%1000)/100 + 2]);
        painter->drawPixmap((201.0/288)*this->windowRect.width(),
                            (237.0/512)*this->windowRect.height(),
                            (17.0/288)*this->windowRect.width(),
                            (21.0/512)*this->windowRect.height(),
                            this->pixmapList[(this->score%100)/10 + 2]);
    }
    else if((this->score%100)/10 != 0)
    {
        painter->drawPixmap((201.0/288)*this->windowRect.width(),
                            (237.0/512)*this->windowRect.height(),
                            (17.0/288)*this->windowRect.width(),
                            (21.0/512)*this->windowRect.height(),
                            this->pixmapList[(this->score%100)/10 + 2]);
    }
    painter->drawPixmap((218.0/288)*this->windowRect.width(),
                        (237.0/512)*this->windowRect.height(),
                        (17.0/288)*this->windowRect.width(),
                        (21.0/512)*this->windowRect.height(),
                        this->pixmapList[this->score%10 + 2]);

    //best score
    if((this->score%1000)/100 != 0)
    {
        painter->drawPixmap((184.0/288)*this->windowRect.width(),
                            (278.0/512)*this->windowRect.height(),
                            (17.0/288)*this->windowRect.width(),
                            (21.0/512)*this->windowRect.height(),
                            this->pixmapList[(this->score%1000)/100 + 2]);
        painter->drawPixmap((201.0/288)*this->windowRect.width(),
                            (278.0/512)*this->windowRect.height(),
                            (17.0/288)*this->windowRect.width(),
                            (21.0/512)*this->windowRect.height(),
                            this->pixmapList[(this->score%100)/10 + 2]);
    }
    else if((this->score%100)/10 != 0)
    {
        painter->drawPixmap((201.0/288)*this->windowRect.width(),
                            (278.0/512)*this->windowRect.height(),
                            (17.0/288)*this->windowRect.width(),
                            (21.0/512)*this->windowRect.height(),
                            this->pixmapList[(this->score%100)/10 + 2]);
    }
    painter->drawPixmap((218.0/288)*this->windowRect.width(),
                        (278.0/512)*this->windowRect.height(),
                        (17.0/288)*this->windowRect.width(),
                        (21.0/512)*this->windowRect.height(),
                        this->pixmapList[this->score%10 + 2]);

    //Medal
    if(this->medal != NONE)
    {
        painter->drawPixmap((55.0/288)*this->windowRect.width(),
                         (245.0/512)*this->windowRect.height(),
                         (45.0/288)*this->windowRect.width(),
                         (45.0/512)*this->windowRect.height(),
                         this->pixmapList[this->medal]);
    }
    //Button
    emit this->buttonVisible(true,true,false);
}

void OverBoard::resize(int _width, int _height)
{
    this->overTextRect.setRect((this->overTextRect.x()/this->windowRect.width())*_width,
                               (this->overTextRect.y()/this->windowRect.height())*_height,
                               (this->overTextRect.width()/this->windowRect.width())*_width,
                               (this->overTextRect.height()/this->windowRect.height())*_height);
    this->scoreLabelRect.setRect((this->scoreLabelRect.x()/this->windowRect.width())*_width,
                               (this->scoreLabelRect.y()/this->windowRect.height())*_height,
                               (this->scoreLabelRect.width()/this->windowRect.width())*_width,
                               (this->scoreLabelRect.height()/this->windowRect.height())*_height);
    this->windowRect.setRect(0,0,_width,_height);
}

void OverBoard::loadFrame()
{
    this->clearAllFrame();
    this->addFrame(QPixmap(":/image/resource/brid_image/text_game_over.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/score_panel.png"));

    this->addFrame(QPixmap(":/image/resource/brid_image/number_score_00.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/number_score_01.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/number_score_02.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/number_score_03.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/number_score_04.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/number_score_05.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/number_score_06.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/number_score_07.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/number_score_08.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/number_score_09.png"));

    this->addFrame(QPixmap(":/image/resource/brid_image/medals_0.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/medals_1.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/medals_2.png"));
    this->addFrame(QPixmap(":/image/resource/brid_image/medals_3.png"));
}

void OverBoard::setScore(int score)
{
    this->score = score;
    switch(this->score/10)
    {
    case    0:
        this->medal = NONE; break;
    case    1:
        this->medal = COPPERMEDAL;  break;
    case    2:
        this->medal = SILVERMEDAL;  break;
    case    3:
        this->medal = GOLDMEDAL;  break;
    default:
        this->medal = PLATINICMEDAL; break;
    }
}
