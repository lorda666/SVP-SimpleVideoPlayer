#ifndef CUSTOMSLIDER_H
#define CUSTOMSLIDER_H

#include <QWidget>
#include <QSlider>
#include <QMouseEvent>
#include <QKeyEvent>

class CustomSlider : public QSlider
{
    //Q_OBJECT

public:
    CustomSlider(QWidget *parent = 0);
    CustomSlider(Qt::Orientation orientation, QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *pEvent);
    void keyPressEvent(QKeyEvent *pEvent);
};

#endif // CUSTOMSLIDER_H
