#ifndef FLAPPYBRID_H
#define FLAPPYBRID_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>
#include <QTime>
#include "background.h"
#include "ground.h"
#include "brid.h"
#include "title.h"
#include "readyboard.h"
#include "scoreboard.h"
#include "overboard.h"
#include "pipe.h"
#include <QResizeEvent>
#include <QPainter>
#include "constant.h"

class FlappyBrid : public QWidget
{
    Q_OBJECT
public:
    explicit FlappyBrid(QWidget *parent = 0);
    ~FlappyBrid();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);

private:
    QPushButton *startButton;
    QPushButton *scoreButton;
    QPushButton *rateButton;

    Background *background;
    Ground *ground;
    Brid *brid;
    Title *title;
    ReadyBoard *readyBoard;
    ScoreBoard *scoreBoard;
    OverBoard *overBoard;
    Pipe *pipe[3];

    int score;
    QRectF impactBirdRect;
    QTimer timer;
    GameStatus status;

    void gameTitleStatus();
    void gameReadyStatus();
    void gamePlayStatus();
    void gameOverStatus();



signals:

public slots:
    void setButtonVisible(bool _startBtn, bool _scoreBtn, bool _rateBtn);
    void getScore();
    void startGame();
};

#endif // FLAPPYBRID_H
