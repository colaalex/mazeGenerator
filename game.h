#ifndef GAME_H
#define GAME_H

#include <QVector>
#include <QPair>
#include <QWidget>
#include <QKeyEvent>
#include "scenedraw.h"


class Game : public QWidget
{
    Q_OBJECT

public:
    Game(QVector<QVector<int> > field, QPair<int, int> start, QPair<int, int> finish, Scenedraw *draw);

private:
    int curX, curY;
    QVector<QVector<int> > field;
    QPair<int, int> start, finish;
    Scenedraw *draw;

protected:
    virtual void keyPressEvent(QKeyEvent *event);
};

#endif // GAME_H
