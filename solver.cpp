#include "solver.h"

Solver::Solver()
{

}


QStack<QPair<int, int> > Solver::solve(QVector<QVector<int> > field) {
    QPair<int, int> start, finish;

    for (int i = 0; i < field.size(); i++) { //поиск стартовой и финишной клетки на периметре
        if (i == 0 || i == field.size()-1) {
            for (int j = 0; j < field.at(i).size(); j++) {
                if (field.at(i).at(j) == 3 || field.at(i).at(j) == 7) start = qMakePair(i, j);
                else if (field.at(i).at(j) == 4) finish = qMakePair(i, j);
            }
        } else {
            if (field.at(i).at(0) == 3) start = qMakePair(i, 0);
            else if (field.at(i).at(field.at(i).size()-1) == 3 || field.at(i).at(field.at(i).size()-1) == 7)
                start = qMakePair(i, field.at(i).size() - 1);
            else if (field.at(i).at(0) == 4) finish = qMakePair(i, 0);
            else if (field.at(i).at(field.at(i).size()-1) == 4) finish = qMakePair(i, field.at(i).size() - 1);
        }
    }

    QVector<QVector<int> > res;
    for (int i = 0; i < field.size(); i++) {
        res.append(QVector<int>());
        for (int j = 0; j < field.at(i).size(); j++) {
            res[i].append(-1); //-1 длина еще не задана
        }
    }
    res[start.first][start.second] = 0;
    QStack<QPair<int, int> > stack;
    stack.push(start);
    while (stack.size() > 0) {
        Solver::markCell_mod(res, stack, field);
    }
    int curDist = res[finish.first][finish.second], i = finish.first, j = finish.second;
    QStack<QPair<int, int> > restack;
    while (curDist > 0) {
        curDist--;
        if (i > 0 && i < res.size() - 1) {
            if (res.at(i-1).at(j) == curDist) i--;
            else if (res.at(i+1).at(j) == curDist) i++;
            else if (j > 0 && j < field.at(i).size() - 1) {
                if (res.at(i).at(j+1) == curDist) j++;
                else if (res.at(i).at(j-1) == curDist) j--;
            }
            else if (j == 0) {
                if (res.at(i).at(j + 1) == curDist) j++;
            }
            else if (j == res.at(i).size()-1)
                    if (res.at(i).at(j-1) == curDist) j--;
        }
        else {
            if (i == 0) {
                if (res.at(1).at(j) == curDist) i++;
            } else {
                if (res.at(i-1).at(j) == curDist) i--;
            }
        }
        restack.push(qMakePair(i, j));
    }
    restack.push(finish);

    return restack;
}

void Solver::markCell_mod(QVector<QVector<int> > &lens, QStack<QPair<int, int> >& stack, QVector<QVector<int> > field) {
    QPair<int, int> pair = stack.pop();
    int i = pair.first, j = pair.second;
    if (i < lens.size() - 1) {
        if ((field.at(i + 1).at(j) == 2 || field.at(i+1).at(j) == 4) && lens.at(i + 1).at(j) == -1) {
            lens[i+1][j] = lens.at(i).at(j) + 1;
            stack.push(qMakePair(i+1, j));
        }
    }
    if (i > 0) {
        if ((field.at(i - 1).at(j) == 2 || field.at(i-1).at(j) == 4) && lens.at(i - 1).at(j) == -1) {
            lens[i-1][j] = lens.at(i).at(j) + 1;
            stack.push(qMakePair(i-1, j));
        }
    }
    if (j < lens.at(i).size() - 1) {
        if ((field.at(i).at(j + 1) == 2 || field.at(i).at(j+1) == 4) && lens.at(i).at(j + 1) == -1) {
            lens[i][j+1] = lens.at(i).at(j) + 1;
            stack.push(qMakePair(i, j+1));
        }
    }
    if (j > 0) {
        if ((field.at(i).at(j - 1) == 2 || field.at(i).at(j-1) == 4) && lens.at(i).at(j -1) == -1) {
            lens[i][j-1] = lens.at(i).at(j) + 1;
            stack.push(qMakePair(i, j-1));
        }
    }
}


