#include "game.h"

Game::Game(QVector<QVector<int> > field, QPair<int, int> start, QPair<int, int> finish, Scenedraw *draw)
{
    this->field = field;
    this->start = start;
    this->finish = finish;
    curX = start.second;
    curY = start.first;
    this->field[curY][curX] = 6;
    this->draw = draw;
    draw->redraw(this->field);
}

void Game::keyPressEvent(QKeyEvent *event) {
    int key = event->key();
    if (key == Qt::Key_Down) {
        field[curY++][curX] = 2;
        field[curY][curX] = 6;
    }
    draw->redraw(field);
}
