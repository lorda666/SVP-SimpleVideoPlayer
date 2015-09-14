#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QVideoWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QTimer>

#include "fullscreencontrols.h"

class VideoWidget : public QVideoWidget
{
    Q_OBJECT

public:
    VideoWidget(QWidget *parent = 0);
    ~VideoWidget();

    void setFullScreenControls(FullscreenControls *pControls);
    FullscreenControls* getFullscreenControls();

signals:
    void mouseStopped();

public slots:
    void hideFullscreenControls();

protected:
    void mouseMoveEvent(QMouseEvent *pEvent);
    void paintEvent(QPaintEvent *pEvent);

private:
    FullscreenControls  *m_pFullscreenControls;
    QPoint  m_CursorPos;
    QTimer  *m_pTimer;
    bool    m_CursorMoved;

    void initFullscreenControls();
};

#endif // VIDEOWIDGET_H
