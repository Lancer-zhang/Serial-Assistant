#ifndef BRID_H
#define BRID_H
#include "gameelement.h"

class Brid : public GameElement
{
public:
    explicit Brid(QObject *parent = 0);
    void init();
    void draw(QPainter *);
    void logic();
    void resize(int,int);
};

#endif // BRID_H
