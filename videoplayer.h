#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QMediaPlaylist>

#include <QPushButton>
#include <QLabel>
#include <QBoxLayout>
#include <QSlider>
#include <QMenu>
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QScreen>
#include <QPoint>

#include "customslider.h"
#include "fullscreencontrols.h"
#include "videowidget.h"

class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit VideoPlayer(QWidget *parent = 0);
    ~VideoPlayer();

    void playSelectedFile(QString file);

public slots:
    void openFile();
    void play();

private slots:
    void mediaStateChanged(QMediaPlayer::State state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void volumeChanged(int volume);
    void setPosition(int position);
    void handleError();
    void setFullscreen();
    void exitFullscreen();
    void setElapsedTime(qint64 elapsedTime);
    void setMuted();
    void mediaStatus(QMediaPlayer::MediaStatus status);

protected:
    void contextMenuEvent(QContextMenuEvent *pEvent) Q_DECL_OVERRIDE;

private:

    // ATTRIBUTES >>
    VideoWidget         *m_pVideoWidget;
    QMediaPlayer        m_MediaPlayer;
    QPushButton         *m_pPlayButton;
    QPushButton         *m_pVolumeButton;
    QPushButton         *m_pStopButton;
    CustomSlider        *m_pPositionSlider;
    CustomSlider        *m_pVolumeSlider;
    QLabel              *m_pErrorLabel;
    QLabel              *m_pElapsedTimeLabel;
    QLabel              *m_pDurationLabel;
    QBoxLayout          *m_pMainLayout;

    QAction     *m_pFullscreenAction;
    QAction     *m_pExitFullscreenAction;
    QAction     *m_pPlayPauseAction;

    bool    m_IsMediaLoaded;

    FullscreenControls  *m_pFullscreenControls;
    // << ATTRIBUTES

    QString convertMs(qint64 ms);
    void initControls();
    void setFullscreenControls();
    void initMediaPlayer();
};

#endif // MAINWINDOW_H
