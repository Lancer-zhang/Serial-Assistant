#ifndef READYBOARD_H
#define READYBOARD_H
#include "gameelement.h"


class ReadyBoard : public GameElement
{
public:
    explicit ReadyBoard(QObject *parent = 0);

    void init();
    void draw(QPainter *);
    void logic();
    void resize(int,int);
};

#endif // READYBOARD_H
