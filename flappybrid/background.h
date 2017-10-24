#ifndef BACKGROUND_H
#define BACKGROUND_H
#include "gameelement.h"

class Background : public GameElement
{
    Q_OBJECT
private:
    int currentFrame;
    void loadFrame();
public:
    explicit Background(QObject *parent = 0);
    void init();
    void draw(QPainter *);
    void logic();
    void resize(int,int);
};

#endif // BACKGROUND_H
