#ifndef SCOREBOARD_H
#define SCOREBOARD_H
#include "gameelement.h"


class ScoreBoard: public GameElement
{
    Q_OBJECT
private:
    int score;
    int tempUnit;
    int tempDecade;
    int tempHundred;
    void loadFrame();

public:
    explicit ScoreBoard(QObject *parent = 0);

    void init();
    void draw(QPainter *);
    void logic();
    void resize(int,int);

    void setScore(int score);


};

#endif // SCOREBOARD_H
