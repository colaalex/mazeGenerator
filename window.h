#ifndef WINDOW_H
#define WINDOW_H

#include <QVBoxLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>


class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

private:
    void addLayout();

    QVBoxLayout *layout;
    QSpinBox *spinW, *spinH;

private slots:
    void passParams();
};

#endif // WINDOW_H
