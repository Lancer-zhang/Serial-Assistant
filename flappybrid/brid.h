#ifndef BRID_H
#define BRID_H
#include "gameelement.h"
#include <qmath.h>
#include <QTimer>

class Brid : public GameElement
{
    Q_OBJECT
private:
    int lastFrame;
    int currentFrame;
    float speedY;
    float increaseY;
    float speedX;
    int angle;
    QTimer timer;

    void loadFrame();
public:
    explicit Brid(QObject *parent = 0);
    void init();
    void draw(QPainter *);
    void logic();
    void resize(int,int);
    void bridUp();



public slots:
    void updateFrame();
};

#endif // BRID_H
