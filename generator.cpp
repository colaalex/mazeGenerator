#include "generator.h"

void markCell(QVector<QVector<int> >& lens, QStack<QPair<int, int> >& stack, QVector<QVector<int> > field);

QVector<QVector<int> > generate(int width, int height) {
    srand(time(0));
    QVector<QVector<int> > field;

    //убедимся, что длина и ширина нечетны для корректной генерации
    width = (width % 2 == 1) ? width : width + 1;
    height = (height % 2 == 1) ? height : height + 1;
    //for (int i = 0; i < 2 * height + 1; i++) { //инициализация матрицы
    for (int i = 0; i < height; i++) {
        field.append(QVector<int>());
        //for (int j = 0; j < 2 * width + 1; j++) {
        for (int j = 0; j < width; j++) {
            if (i % 2 != 0 && j % 2 != 0 && (i < height - 1 && j < width - 1)) {
                field[i].append(1); //1 - клетка непосещенная
            }
            else field[i].append(0); //0 -стена, есть еще 2 - дорожка
        }
    }

    //алгоритм генерации лабиринта
    for (int i = 1; i < field.at(0).size() - 1; i++) {
        field[1][i] = 2;
    }

    for (int i = 3; i < field.size() - 1; i+=2) {
        int j = 1;
        QVector<int> set;
        while (j < field.at(i).size() - 1) {
            set.append(j);

            int r = rand() % 2; //0 - не идти вправо, 1 - идти
            if (r == 1 && j < field.at(i).size() - 2) {
                field[i][j] = 2;
                field[i][j+1] = 2;
                field[i][j+2] = 2;
                j += 2;
            }
            else {
                int randCel = set.at(rand() % set.size());
                field[i][randCel] = 2;
                field[i-1][randCel] = 2;
                if (j < field.at(i).size() - 2) j += 2;
                else break;
                set.clear();
            }

        }
    }

    //выбор клетки входа
    bool selected = false;
    int startI, startJ;
    while (!selected) {
        int randij = rand() % 2; //0 - бегаем по строкам, 1 - бегаем по столбцам
        if (randij == 0) {
            int randj = rand() % 2; //0 - 0, 1 - field[0].size() - 1
            randj = (randj == 0) ? 0 : field.at(0).size() - 1;
            int randi = 1 + rand() % field.size() - 1;
            int nearj = (randj == 0) ? 1 : field.at(0).size() - 2;
            if (field.at(randi).at(nearj) == 2) {
                field[randi][randj] = 3; //3 - вход
                startI = randi, startJ = nearj;
                selected = true;
            }
        } else {
            int randi = rand() % 2; //0 - 0, 1 - field.size() - 1
            randi = (randi == 0) ? 0 : field.size() - 1;
            int randj = 1 + rand() % field.at(0).size() - 1;
            int neari = (randi == 0) ? 1 : field.size() - 2;
            if (field.at(neari).at(randj) == 2) {
                field[randi][randj] = 3; //3 - вход
                startI = neari, startJ = randj;
                selected = true;
            }
        }
    }

    //создание матрицы расстояний (для поиска наиболее отдаленного выхода)
    QVector<QVector<int> > lens =  QVector<QVector<int> >(field);
    for (int i = 0; i < field.size(); i++) {
        for (int j = 0; j < field.at(i).size(); j++) {
            lens[i][j] = -1; //-1 длина еще не задана
        }
    }
    lens[startI][startJ] = 0;
    QStack<QPair<int, int> > stack;
    stack.push(qMakePair(startI, startJ));
    while (stack.size() > 0) {
        markCell(lens, stack, field);
    }

    QPair<int, int> exitCell;
    int maxDist = 0;
    for (int i = 0; i < field.size(); i++) {
        if (i == 0) {
            for (int j = 1; j < field.at(i).size() - 1; j++) {
                if (lens.at(i+1).at(j) > maxDist) {
                    maxDist = lens.at(i+1).at(j);
                    exitCell = qMakePair(i, j);
                }
            }
        } else if (i == field.size() - 1) {
            for (int j = 1; j < field.at(i).size() - 1; j++) {
                if (lens.at(i-1).at(j) > maxDist) {
                    maxDist = lens.at(i-1).at(j);
                    exitCell = qMakePair(i, j);
                }
            }
        } else {
            if (lens.at(i).at(1) > maxDist) {
                maxDist = lens.at(i).at(1);
                exitCell = qMakePair(i, 0);
            }
            if (lens.at(i).at(lens.at(i).size() - 2) > maxDist) {
                maxDist = lens.at(i).at(lens.at(i).size() - 2);
                exitCell = qMakePair(i, lens.at(i).size() - 1);
            }
        }
    }
    field[exitCell.first][exitCell.second] = 4; //4 - выход

    return field;
}

void markCell(QVector<QVector<int> > &lens, QStack<QPair<int, int> >& stack, QVector<QVector<int> > field) {
    QPair<int, int> pair = stack.pop();
    int i = pair.first, j = pair.second;
    if (i < lens.size() - 1) {
        if (field.at(i + 1).at(j) == 2 && lens.at(i + 1).at(j) == -1) {
            lens[i+1][j] = lens.at(i).at(j) + 1;
            stack.push(qMakePair(i+1, j));
        }
    }
    if (i > 0) {
        if (field.at(i - 1).at(j) == 2 && lens.at(i - 1).at(j) == -1) {
            lens[i-1][j] = lens.at(i).at(j) + 1;
            stack.push(qMakePair(i-1, j));
        }
    }
    if (j < lens.at(i).size() - 1) {
        if (field.at(i).at(j + 1) == 2 && lens.at(i).at(j + 1) == -1) {
            lens[i][j+1] = lens.at(i).at(j) + 1;
            stack.push(qMakePair(i, j+1));
        }
    }
    if (j > 0) {
        if (field.at(i).at(j - 1) == 2 && lens.at(i).at(j -1) == -1) {
            lens[i][j-1] = lens.at(i).at(j) + 1;
            stack.push(qMakePair(i, j-1));
        }
    }
}
