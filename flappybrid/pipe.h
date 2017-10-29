#ifndef PIPE_H
#define PIPE_H
#include "gameelement.h"

class Pipe : public GameElement
{
    Q_OBJECT
private:
    int currentFrame;
    int speedX;
    int startPosition;
    QRectF pipeRect[2];
    bool pipePassEmited;

    void loadFrame();
public:
    explicit Pipe(int pos,QObject *parent = 0);
    void init();
    void draw(QPainter *);
    void logic();
    void resize(int, int);

    QRectF &getRect(PipeType);
signals:
    void pipePass();
};

#endif // PIPE_H
