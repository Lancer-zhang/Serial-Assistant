#ifndef GROUND_H
#define GROUND_H
#include "gameelement.h"

class Ground : public GameElement
{
    Q_OBJECT
private:
    int currentFrame;
    int positionX;
    float speedX;
public:
    explicit Ground(QObject *parent = 0);
    void init();
    void draw(QPainter *);
    void logic();
    void resize(int,int);
};

#endif // GROUND_H
