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
    this->scoreBoard = new ScoreBoard();
    this->readyBoard = new ReadyBoard();
    this->overBoard = new OverBoard();

    this->pipe[0] = new Pipe(0);
    this->pipe[1] = new Pipe(1);
    this->pipe[2] = new Pipe(2);
    connect(&timer,SIGNAL(timeout()),this,SLOT(update()));
    timer.start(30);
    connect(this->pipe[0],SIGNAL(pipePass()),this,SLOT(getScore()));
    connect(this->pipe[1],SIGNAL(pipePass()),this,SLOT(getScore()));
    connect(this->pipe[2],SIGNAL(pipePass()),this,SLOT(getScore()));
    connect(this->startButton, SIGNAL(clicked()), this, SLOT(startGame()));
    connect(this->overBoard,SIGNAL(buttonVisible(bool,bool,bool)),this,SLOT(setButtonVisible(bool,bool,bool)));
    this->gameTitleStatus();
}

void FlappyBrid::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    this->background->logic();
    this->background->draw(&painter);

    this->pipe[0]->logic();
    this->pipe[0]->draw(&painter);

    this->pipe[1]->logic();
    this->pipe[1]->draw(&painter);

    this->pipe[2]->logic();
    this->pipe[2]->draw(&painter);

    this->ground->logic();
    this->ground->draw(&painter);

    this->brid->logic();
    this->brid->draw(&painter);

    this->title->logic();
    this->title->draw(&painter);

    this->scoreBoard->logic();
    this->scoreBoard->draw(&painter);

    this->readyBoard->logic();
    this->readyBoard->draw(&painter);

    this->overBoard->logic();
    this->overBoard->draw(&painter);



    this->impactBirdRect.moveCenter(this->brid->getBindRect().center());

    if(this->brid->getBindRect().intersects(this->ground->getBindRect())
            ||this->impactBirdRect.intersects(this->pipe[0]->getRect(above))
            ||this->impactBirdRect.intersects(this->pipe[0]->getRect(following))
            ||this->impactBirdRect.intersects(this->pipe[1]->getRect(above))
            ||this->impactBirdRect.intersects(this->pipe[1]->getRect(following))
            ||this->impactBirdRect.intersects(this->pipe[2]->getRect(above))
            ||this->impactBirdRect.intersects(this->pipe[2]->getRect(following)))
    {
        this->gameOverStatus();
    }
}

void FlappyBrid::resizeEvent(QResizeEvent *event)
{
    this->background->resize(this->width(),this->height());
    this->ground->resize(this->width(), this->height());
    this->brid->resize(this->width(), this->height());
    this->title->resize(this->width(),this->height());
    this->scoreBoard->resize(this->width(),this->height());
    this->readyBoard->resize(this->width(),this->height());
    this->overBoard->resize(this->width(), this->height());
    this->pipe[0]->resize(this->width(),this->height());
    this->pipe[1]->resize(this->width(),this->height());
    this->pipe[2]->resize(this->width(),this->height());

    this->impactBirdRect.setRect(0,0,(20.0/288)*this->width(),(20.0/512)*this->height());

    this->startButton->setGeometry(QRect((20.0/288)*this->width(),
                                   (341.0/512)*this->height(),
                                   (117.0/288)*this->width(),
                                   (71.0/512)*this->height()));
    this->scoreButton->setGeometry(QRect((151.0/288)*this->width(),
                                         (341.0/512)*this->height(),
                                         (117.0/288)*this->width(),
                                         (71.0/512)*this->height()));
    this->rateButton->setGeometry(QRect((106.5/288)*this->width(),
                                         (270.0/512)*this->height(),
                                         (75.0/288)*this->width(),
                                         (48.0/512)*this->height()));
}

void FlappyBrid::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(this->status == GAMEPLAY)
        {
            this->brid->bridUp();
        }
        if(this->status == GAMEREADY)
        {
            this->gamePlayStatus();
        }
    }
}

void FlappyBrid::startGame()
{
    this->background->init();
    this->ground->init();
    this->brid->init();
    this->scoreBoard->init();
    this->readyBoard->init();
    this->overBoard->init();
    this->pipe[0]->init();
    this->pipe[1]->init();
    this->pipe[2]->init();

    this->setButtonVisible(false,false,false);
    this->gameReadyStatus();
}

void FlappyBrid::gameTitleStatus()
{
    this->background->enabledLogic = true;
    this->background->enabledDraw = true;

    this->ground->enabledLogic = true;
    this->ground->enabledDraw = true;

    this->title->enabledLogic = true;
    this->title->enabledDraw = true;

    this->brid->enabledLogic = false;
    this->brid->enabledDraw = false;

    this->scoreBoard->enabledLogic = false;
    this->scoreBoard->enabledDraw = false;

    this->readyBoard->enabledLogic = false;
    this->readyBoard->enabledDraw = false;

    this->overBoard->enabledLogic = false;
    this->overBoard->enabledDraw = false;

    this->pipe[0]->enabledLogic = false;
    this->pipe[0]->enabledDraw = false;
    this->pipe[1]->enabledLogic = false;
    this->pipe[1]->enabledDraw = false;
    this->pipe[2]->enabledLogic = false;
    this->pipe[2]->enabledDraw = false;
     this->setButtonVisible(true,true,true);
    this->status = GAMETITLE;
}
void FlappyBrid::gameReadyStatus()
{
    this->score = 0;

    this->background->enabledLogic = true;
    this->background->enabledDraw = true;

    this->ground->enabledLogic = true;
    this->ground->enabledDraw = true;

    this->title->enabledLogic = false;
    this->title->enabledDraw = false;

    this->brid->enabledLogic = false;
    this->brid->enabledDraw = true;

    this->scoreBoard->enabledLogic = true;
    this->scoreBoard->enabledDraw = true;

    this->readyBoard->enabledLogic = true;
    this->readyBoard->enabledDraw = true;

    this->overBoard->enabledLogic = false;
    this->overBoard->enabledDraw = false;

    this->pipe[0]->enabledLogic = false;
    this->pipe[0]->enabledDraw = false;
    this->pipe[1]->enabledLogic = false;
    this->pipe[1]->enabledDraw = false;
    this->pipe[2]->enabledLogic = false;
    this->pipe[2]->enabledDraw = false;
    this->status = GAMEREADY;
}
void FlappyBrid::gamePlayStatus()
{
    this->background->enabledLogic = true;
    this->background->enabledDraw = true;

    this->ground->enabledLogic = true;
    this->ground->enabledDraw = true;

    this->title->enabledLogic = false;
    this->title->enabledDraw = false;

    this->brid->enabledLogic = true;
    this->brid->enabledDraw = true;

    this->scoreBoard->enabledLogic = true;
    this->scoreBoard->enabledDraw = true;

    this->readyBoard->enabledLogic = false;
    this->readyBoard->enabledDraw = false;

    this->overBoard->enabledLogic = false;
    this->overBoard->enabledDraw = false;

    this->pipe[0]->enabledLogic = true;
    this->pipe[0]->enabledDraw = true;
    this->pipe[1]->enabledLogic = true;
    this->pipe[1]->enabledDraw = true;
    this->pipe[2]->enabledLogic = true;
    this->pipe[2]->enabledDraw = true;
    this->status = GAMEPLAY;
}
void FlappyBrid::gameOverStatus()
{
    this->background->enabledLogic = true;
    this->background->enabledDraw = true;

    this->ground->enabledLogic = false;
    this->ground->enabledDraw = true;

    this->title->enabledLogic = false;
    this->title->enabledDraw = false;


    this->overBoard->setScore(this->score);
    this->overBoard->enabledLogic = true;
    this->overBoard->enabledDraw = true;

    this->brid->enabledLogic = true;
    this->brid->enabledDraw = true;

    this->readyBoard->enabledLogic = false;
    this->readyBoard->enabledDraw = false;

    this->scoreBoard->enabledLogic = false;
    this->scoreBoard->enabledDraw = false;

    this->pipe[0]->enabledLogic = false;
    this->pipe[0]->enabledDraw = true;
    this->pipe[1]->enabledLogic = false;
    this->pipe[1]->enabledDraw = true;
    this->pipe[2]->enabledLogic = false;
    this->pipe[2]->enabledDraw = true;
    this->status = GAMEOVER;
}

void FlappyBrid::getScore()
{
    this->score += 1;
    this->scoreBoard->setScore(this->score);
}

void FlappyBrid::setButtonVisible(bool _startBtn, bool _scoreBtn, bool _rateBtn)
{
    this->startButton->setVisible(_startBtn);
    this->scoreButton->setVisible(_scoreBtn);
    this->rateButton->setVisible(_rateBtn);
}
