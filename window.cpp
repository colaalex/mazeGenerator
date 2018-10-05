#include "window.h"
#include "scenedraw.h"

Window::Window()
{
    addLayout();
    setLayout(layout);
    this->setGeometry(500, 500, 350, 150);
}

void Window::addLayout() {
    layout = new QVBoxLayout;

    QLabel *labelH = new QLabel("Введите высоту лабиринта");

    spinH = new QSpinBox;
    spinH->setRange(5, 37);
    spinH->setSingleStep(2);
    spinH->setValue(21);

    QLabel *labelW = new QLabel("Введите ширину лабиринта");

    spinW = new QSpinBox;
    spinW->setRange(5, 75);
    spinW->setSingleStep(2);
    spinW->setValue(21);

    QPushButton *button = new QPushButton;
    button->setText("Сгенерировать лабиринт");
    connect(button, SIGNAL (released()), this, SLOT (passParams()));

    layout->addWidget(labelH);
    layout->addWidget(spinH);
    layout->addWidget(labelW);
    layout->addWidget(spinW);
    layout->addWidget(button);
}

void Window::passParams() {
    new Scenedraw(spinW->value(), spinH->value());
    this->close();
}
