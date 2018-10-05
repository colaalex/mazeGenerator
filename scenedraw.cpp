#include "Scenedraw.h"



Scenedraw::Scenedraw(int width, int height, QWidget *parent) : QWidget(parent) {
    //this->setFocusPolicy(Qt::TabFocus);
    this->width = width;
    this->height = height;
    scene = new QGraphicsScene();
    field = generate(width, height);
    drawView();
    view = new QGraphicsView(scene);
    view->show();
}

void Scenedraw::drawView() {

    //QGraphicsScene * scene = new QGraphicsScene();

    int tileSize = 25; //длина клетки (клетка квадратная)
    int rows = field.size(), cols = field.at(0).size();
    int centRow = std::ceil(rows / 2); //номер центральной клетки
    int centCol = std::ceil(cols / 2);

    for (int i = 0; i < field.size(); i++) {
        int row = i + 1;
        int y = (row - centRow) * tileSize; //координата текущей клетки, считается относительно центральной
        for (int j = 0; j < field.at(i).size(); j++) {
            int col = j + 1;
            int x = (col - centCol) * tileSize;

            QGraphicsRectItem * rect = new QGraphicsRectItem();

            rect->setRect(x, y, tileSize, tileSize);
            int cur = field.at(i).at(j);
            if (cur == 0) rect->setBrush(QBrush(Qt::cyan));
            else if (cur == 5) rect->setBrush(QBrush(Qt::red)); //5 - дорожка прохождения
            else if (cur == 4) {finish = qMakePair(i, j); rect->setBrush(QBrush(Qt::black));}
            else if (cur == 3) {start = qMakePair(i, j); rect->setBrush(QBrush(Qt::black));}
            else if (cur == 6 || cur == 7) rect->setBrush(QBrush(Qt::yellow)); //6 - положение игрока, 7 - игрок на старте
            else rect->setBrush(QBrush(Qt::black));
            scene->addItem(rect);
        }
    }

    //добавить кнопки под полем
    btnReload = new QPushButton();
    btnReload->setGeometry((1 - centCol) * tileSize, (2 + centRow) * tileSize, cols*tileSize/2, 25);
    btnReload->setText("Обновить поле");
    scene->addWidget(btnReload);
    connect(btnReload, SIGNAL (released()),this, SLOT (handle()));

    btnSolve = new QPushButton();
    btnSolve->setGeometry(centCol + 1.15 * tileSize, (2 + centRow) * tileSize, cols*tileSize/2, 50);
    btnSolve->setText("Решить лабиринт");
    scene->addWidget(btnSolve);
    connect(btnSolve, SIGNAL (released()), this, SLOT (solve()));

    btnGame = new QPushButton();
    btnGame->setGeometry((1-centCol)*tileSize, 25+(2+centRow)*tileSize, cols*tileSize/2, 25);
    btnGame->setText("Пройти лабиринт");
    scene->addWidget(btnGame);
    connect(btnGame, SIGNAL (released()), this, SLOT (startGame()));

    btnLeft = new QPushButton();
    btnLeft->setGeometry((1-centCol)*tileSize, (2+centRow)*tileSize, cols*tileSize/3, 50);
    btnLeft->setText("<-");
    btnLeft->setVisible(false);
    scene->addWidget(btnLeft);
    connect(btnLeft, SIGNAL(released()), this, SLOT(goLeft()));

    btnUp = new QPushButton();
    btnUp->setGeometry((1-centCol+cols/3)*tileSize, (2+centRow)*tileSize, cols*tileSize/3, 25);
    btnUp->setText("/\\");
    btnUp->setVisible(false);
    scene->addWidget(btnUp);
    connect(btnUp, SIGNAL(released()), this, SLOT(goUp()));

    btnDown = new QPushButton();
    btnDown->setGeometry((1-centCol+cols/3)*tileSize, 25+(2+centRow)*tileSize, cols*tileSize/3, 25);
    btnDown->setText("\\/");
    btnDown->setVisible(false);
    scene->addWidget(btnDown);
    connect(btnDown, SIGNAL(released()), this, SLOT(goDown()));

    btnRight = new QPushButton();
    btnRight->setGeometry((1-centCol+2*cols/3)*tileSize, (2+centRow)*tileSize, cols*tileSize/3, 50);
    btnRight->setText("->");
    btnRight->setVisible(false);
    scene->addWidget(btnRight);
    connect(btnRight, SIGNAL(released()), this, SLOT(goRight()));
}


void Scenedraw::handle() {
    field = generate(width, height);
    drawView();
    view->update();
}


void Scenedraw::solve() {
    field = solved();
    drawView();
    view->update();
    btnGame->setEnabled(false);
}


QVector<QVector<int> > Scenedraw::solved() {
    Solver *solver = new Solver();
    QVector<QVector<int> > field1 = QVector<QVector<int> >(field);
    QStack<QPair<int, int> > stack = solver->Solver::solve(field);
    while (stack.size() > 0) {
        QPair<int, int> pair = stack.pop();
        field1[pair.first][pair.second] = 5; //5 - путь решенный
    }
    return field1;
}

void Scenedraw::redraw(QVector<QVector<int> > field) {
    this->field = field;
    drawView();
    view->update();
}

void Scenedraw::startGame() {
    int startI = start.first;
    int startJ = start.second;
    current = start;
    field[startI][startJ] = 7;
    drawView();
    view->update();
    gameVisible();
}

void Scenedraw::gameVisible() {
    btnReload->setVisible(false);
    btnSolve->setVisible(false);
    btnGame->setVisible(false);
    btnUp->setVisible(true);
    btnLeft->setVisible(true);
    btnDown->setVisible(true);
    btnRight->setVisible(true);
}

void Scenedraw::mainVisible() {
    btnReload->setVisible(true);
    btnSolve->setVisible(true);
    btnGame->setVisible(true);
    btnUp->setVisible(false);
    btnLeft->setVisible(false);
    btnDown->setVisible(false);
    btnRight->setVisible(false);
}

void Scenedraw::goToCell(QPair<int, int> pos){
    int i = pos.first, j = pos.second;
    int curI = current.first, curJ = current.second;
    if ((std::abs(curI - i) == 1 && curJ == j) || (curI == i && std::abs(curJ - j) == 1)) {
        if (current == start) field[start.first][start.second] = 3;
        else if (pos == finish) {
            field[curI][curJ] = 2;
            mainVisible();
            solve();
            return;
        }
        else field[curI][curJ] = 2;
        field[i][j] = 6;
    }
    drawView();
    view->update();
    gameVisible();
    current = pos;
}

void Scenedraw::goLeft() {
    int j = current.second, i = current.first;
    if (j > 0 && (field.at(i).at(j-1) == 2 || field.at(i).at(j-1) == 3 || field.at(i).at(j-1) == 4))
        goToCell(qMakePair(i, j-1));
}

void Scenedraw::goUp() {
    int j = current.second, i = current.first;
    if (i > 0 && (field.at(i-1).at(j) == 2 || field.at(i-1).at(j) == 3 || field.at(i-1).at(j) == 4))
        goToCell(qMakePair(i-1, j));
}

void Scenedraw::goDown() {
    int j = current.second, i = current.first;
    if (i < field.size()-1 && (field.at(i+1).at(j) == 2 || field.at(i+1).at(j) == 3 || field.at(i+1).at(j) == 4))
        goToCell(qMakePair(i+1, j));
}

void Scenedraw::goRight() {
    int j = current.second, i = current.first;
    if (j < field.at(i).size()-1 && (field.at(i).at(j+1) == 2 || field.at(i).at(j+1) == 3 || field.at(i).at(j+1) == 4))
        goToCell(qMakePair(i, j+1));
}


Scenedraw::~Scenedraw()
{

}
