#ifndef TITLE_H
#define TITLE_H
#include "gameelement.h"
#include <QTimer>

class Title : public GameElement
{
    Q_OBJECT
private:
    QTimer timer;
    int lastFrame;
    int currentFrame;
    void loadFrame();

public:
    explicit Title(QObject *parent = 0);
    void init();
    void draw(QPainter *);
    void logic();
    void resize(int,int);

signals:

public slots:
    void updateFrame();
};

#endif // TITLE_H
