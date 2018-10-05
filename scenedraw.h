#ifndef SCENEDRAW_H
#define SCENEDRAW_H
#include "generator.h"
#include "solver.h"
#include <QVector>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QPair>
#include <Qt>
#include <QWidget>
#include <QGraphicsRectItem>
#include <QStack>
#include <cmath>

class Scenedraw: public QWidget {
    Q_OBJECT
public:
     explicit Scenedraw(int width, int height, QWidget *parent = 0);
     virtual ~Scenedraw();
     void redraw(QVector<QVector<int> > field);


private:
    void drawView();
    void gameVisible();
    void mainVisible();
    QVector<QVector<int> > solved();
    void goToCell(QPair<int, int> pos);

    QGraphicsView *view;
    int width, height;
    QPair<int, int> start, finish, current;
    QPushButton *btnReload, *btnSolve, *btnGame, *btnLeft, *btnRight, *btnUp, *btnDown;
    QGraphicsScene *scene;
    QVector<QVector<int> > field;


private slots:
    void handle();
    void solve();
    void startGame();
    void goLeft();
    void goRight();
    void goUp();
    void goDown();
};


#endif // SCENEDRAW_H
