#ifndef SOLVER_H
#define SOLVER_H

#include <QVector>
#include <QPair>
#include <QStack>

class Solver
{
public:
    Solver();
    QStack<QPair<int, int> > solve(QVector<QVector<int> > field);

private:
    void markCell_mod(QVector<QVector<int> > &lens, QStack<QPair<int, int> >& stack, QVector<QVector<int> > field);
};

#endif // SOLVER_H
