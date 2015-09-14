#include "customslider.h"

CustomSlider::CustomSlider(QWidget *parent) : QSlider(parent)
{

}

CustomSlider::CustomSlider(Qt::Orientation orientation, QWidget *parent)
    : QSlider(orientation, parent)
{

}

void CustomSlider::mousePressEvent(QMouseEvent *pEvent)
{
    if(pEvent->button() == Qt::LeftButton)
    {
        int position = this->minimum() + (this->maximum() - this->minimum()) *
                pEvent->x() / this->width();
        this->setSliderPosition(position);

        emit this->sliderMoved(position);

        pEvent->accept();
    }

    QSlider::mousePressEvent(pEvent);
}

void CustomSlider::keyPressEvent(QKeyEvent *pEvent)
{
    int currentSliderPosition = this->sliderPosition();
    int newSliderPosition;
    if(pEvent->key() == Qt::Key_Left)
    {
        newSliderPosition = currentSliderPosition - 5000;
        if(newSliderPosition < 0)
        {
            newSliderPosition = 0;
        }

        this->setSliderPosition(newSliderPosition);

        emit this->sliderMoved(newSliderPosition);

        pEvent->accept();
    }
    else if(pEvent->key() == Qt::Key_Right)
    {
        newSliderPosition = currentSliderPosition + 5000;
        if(newSliderPosition > this->maximum())
        {
            newSliderPosition = this->maximum();
        }

        this->setSliderPosition(newSliderPosition);

        emit this->sliderMoved(newSliderPosition);

        pEvent->accept();
    }

    QSlider::keyPressEvent(pEvent);
}
