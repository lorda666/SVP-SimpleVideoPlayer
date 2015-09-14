#ifndef FULLSCREENCONTROLS_H
#define FULLSCREENCONTROLS_H

#include <QWidget>
#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPaintEvent>
#include <QMouseEvent>

#include "customslider.h"

class FullscreenControls : public QWidget
{
    Q_OBJECT

public:
    explicit FullscreenControls(QWidget *parent = 0);
    ~FullscreenControls();

    void paintEvent(QPaintEvent *pEvent);
    void mouseMoveEvent(QMouseEvent *pEvent);

    QPushButton* getPlayButton();
    QPushButton* getStopButton();
    QPushButton* getVolumeButton();
    CustomSlider* getPositionSlider();
    CustomSlider* getVolumeSlider();

    void setPlayButton();
    void setPauseButton();
    void setVolumeButton(QPushButton *pVolumeButton);
    void setVolumeSlider(int volume);

    void setElapsedTimeLabel(QLabel *pElapsedTimeLabel);
    void setDurationLabel(QLabel *pDurationLabel);

signals:
    void cursorMoved();

public slots:
    void setPosition(qint64 position);
    void setDuration(qint64 duration);

private:
    // ATTRIBUTES >>
    QPushButton     *m_pPlayButton;
    QPushButton     *m_pStopButton;
    QPushButton     *m_pVolumeButton;

    CustomSlider    *m_pPositionSlider;
    CustomSlider    *m_pVolumeSlider;

    QLabel      *m_pElapsedTimeLabel;
    QLabel      *m_pDurationLabel;

    QBoxLayout      *m_pTimeLineLayout;
    QBoxLayout      *m_pControlsLayout;
    // << ATTRIBUTES

    QString convertMs(qint64 ms);
};

#endif // FULLSCREENCONTROLS_H
