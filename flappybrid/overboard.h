#ifndef OVERBOARD_H
#define OVERBOARD_H
#include "gameelement.h"
#include "constant.h"

class OverBoard : public GameElement
{
    Q_OBJECT
private:
    int score;
    Medal medal;
    bool scoreLabelArrived;
    QRectF scoreLabelRect;
    QRectF overTextRect;
    void loadFrame();
public:
    explicit OverBoard(QObject *parent = 0);
    void init();
    void draw(QPainter *);
    void logic();
    void resize(int, int);
    void setScore(int);

signals:
    void buttonVisible(bool,bool,bool);
public slots:
};

#endif // OVERBOARD_H
