#ifndef GENERATOR_H
#define GENERATOR_H
#include <QVector>
#include <QPair>
#include <QStack>
#include <ctime>


QVector<QVector<int> > generate(int width, int height);
void markCell(QVector<QVector<int> > &lens, QStack<QPair<int, int> >& stack, QVector<QVector<int> > field);

#endif // GENERATOR_H
