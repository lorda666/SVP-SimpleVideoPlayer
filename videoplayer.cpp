#include "videoplayer.h"

#include <QWidget>
#include <QVideoWidget>
#include <QVideoSurfaceFormat>
#include <QBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QFileInfo>
#include <QStyle>
#include <QMessageBox>

VideoPlayer::VideoPlayer(QWidget *parent) :
    QWidget(parent),
    m_MediaPlayer(0, QMediaPlayer::VideoSurface),
    m_pPlayButton(0),
    m_pPositionSlider(0)
{
    QAbstractButton *pOpenButton = new QPushButton(tr("Load file"));
    connect(pOpenButton, SIGNAL(clicked()), this, SLOT(openFile()));

    this->initControls();

    QBoxLayout *pControlsLayout = new QHBoxLayout();
    pControlsLayout->setMargin(0);
    pControlsLayout->addWidget(pOpenButton);
    pControlsLayout->addWidget(this->m_pPlayButton);
    pControlsLayout->addWidget(this->m_pStopButton);
    pControlsLayout->addWidget(this->m_pElapsedTimeLabel);
    pControlsLayout->addWidget(this->m_pPositionSlider);
    pControlsLayout->addWidget(this->m_pDurationLabel);
    pControlsLayout->addWidget(this->m_pVolumeButton);
    pControlsLayout->addWidget(this->m_pVolumeSlider);

    pControlsLayout->setMargin(10);

    this->m_pVideoWidget = new VideoWidget();
    this->m_pVideoWidget->setMinimumHeight(480);
    this->m_pVideoWidget->setMinimumWidth(640);
    this->m_pVideoWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

    this->setFullscreenControls();

    this->m_pMainLayout = new QVBoxLayout();
    this->m_pMainLayout->addWidget(this->m_pVideoWidget);
    this->m_pMainLayout->addLayout(pControlsLayout);
    this->m_pMainLayout->setMargin(0);

    this->m_pMainLayout->setSizeConstraint(QLayout::SetMinimumSize);

    this->setLayout(this->m_pMainLayout);

    this->initMediaPlayer();

    this->m_pFullscreenAction = new QAction(tr("Fullscreen"), this);
    this->m_pFullscreenAction->setShortcut(Qt::Key_F);
    this->m_pFullscreenAction->setEnabled(true);
    this->m_pVideoWidget->addAction(this->m_pFullscreenAction);
    connect(this->m_pFullscreenAction, SIGNAL(triggered(bool)),
            this, SLOT(setFullscreen()));

    this->m_pExitFullscreenAction = new QAction(this);
    this->m_pExitFullscreenAction->setEnabled(false);

    QList<QKeySequence> exitFullscreenShortcuts;
    exitFullscreenShortcuts.append(Qt::Key_Escape);
    exitFullscreenShortcuts.append(Qt::Key_F);
    this->m_pExitFullscreenAction->setShortcuts(exitFullscreenShortcuts);

    connect(this->m_pExitFullscreenAction, SIGNAL(triggered(bool)),
            this, SLOT(setFullscreen()));

    this->m_pPlayPauseAction = new QAction(this);
    this->m_pPlayPauseAction->setShortcut(Qt::Key_Space);
    connect(this->m_pPlayPauseAction, SIGNAL(triggered(bool)),
            this, SLOT(play()));

    this->m_pVideoWidget->addAction(this->m_pExitFullscreenAction);
    this->m_pVideoWidget->addAction(this->m_pPlayPauseAction);

    this->m_IsMediaLoaded = false;
}

VideoPlayer::~VideoPlayer()
{

}

void VideoPlayer::playSelectedFile(QString file)
{
    QFileInfo fileInfo(file);

    QString fileExtension = fileInfo.suffix();
    if((fileExtension == "avi") || (fileExtension == "mp4") ||
       (fileExtension == "flv") || (fileExtension == "mov"))
    {
        if(file.isEmpty() == false)
        {
            this->m_MediaPlayer.setMedia(QUrl::fromLocalFile(file));
            this->setWindowTitle("SVP - " + fileInfo.fileName());
        }
    }
    else
    {
        QMessageBox::information(0, "Invalid file", "Cannot play this file! It must be *.avi, *.mp4, "
                                                    "*.flv or *.mov");
    }
}

void VideoPlayer::openFile()
{
    QFileDialog openFileDialog;
    openFileDialog.setNameFilter("Video files (*.avi *.mp4 *.flv *.mov)");

    if(openFileDialog.exec() == true)
    {
        QString file = openFileDialog.getOpenFileName(this, tr("Select video file"));
        QFileInfo fileInfo(file);
        if(file.isEmpty() == false)
        {
            this->m_MediaPlayer.setMedia(QUrl::fromLocalFile(file));
            this->setWindowTitle("SVP - \"" + fileInfo.fileName() + "\"");
        }
    }
}

void VideoPlayer::play()
{
    switch(this->m_MediaPlayer.state())
    {
    case QMediaPlayer::PlayingState:
        this->m_MediaPlayer.pause();
        break;
    default:
        this->m_MediaPlayer.play();
        break;
    }
}

void VideoPlayer::contextMenuEvent(QContextMenuEvent *pEvent)
{
    QMenu menu(this);
    menu.addAction(this->m_pFullscreenAction);
    menu.exec(pEvent->globalPos());
}

void VideoPlayer::mediaStateChanged(QMediaPlayer::State state)
{
    switch(state)
    {
    case QMediaPlayer::PlayingState:
        this->m_pPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        this->m_pPlayButton->setToolTip("Pause");
        this->m_pFullscreenControls->setPauseButton();
        break;
    case QMediaPlayer::StoppedState:
        this->m_pPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        this->m_pPlayButton->setToolTip("Play");
        this->m_pFullscreenControls->setPlayButton();
        break;
    default:
        this->m_pPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        this->m_pPlayButton->setToolTip("Play");
        this->m_pFullscreenControls->setPlayButton();
        break;
    }
}

void VideoPlayer::positionChanged(qint64 position)
{
    this->m_pPositionSlider->setValue(position);
}

void VideoPlayer::durationChanged(qint64 duration)
{
    this->m_pPositionSlider->setRange(0, duration);
    this->m_pDurationLabel->setText(convertMs(duration));
}

void VideoPlayer::volumeChanged(int volume)
{
    this->m_pVolumeSlider->setValue(volume);
    this->m_pFullscreenControls->setVolumeSlider(volume);
}

void VideoPlayer::setPosition(int position)
{
    this->m_MediaPlayer.setPosition(position);
}

void VideoPlayer::handleError()
{
    this->m_pPlayButton->setEnabled(false);
    QMessageBox::information(0, "Error", this->m_MediaPlayer.errorString());
}

void VideoPlayer::setFullscreen()
{
    if(this->m_pVideoWidget->isFullScreen() == false)
    {
        this->m_pVideoWidget->setFullScreen(true);
        this->m_pExitFullscreenAction->setEnabled(true);
        this->m_pFullscreenAction->setEnabled(false);
        this->m_pVideoWidget->setCursor(Qt::BlankCursor);
    }
    else
    {
        this->m_pVideoWidget->setFullScreen(false);
        this->m_pExitFullscreenAction->setEnabled(false);
        this->m_pFullscreenAction->setEnabled(true);

        this->m_pVideoWidget->setCursor(Qt::ArrowCursor);

        this->m_pFullscreenControls->hide();
    }
}

void VideoPlayer::exitFullscreen()
{
    this->m_pVideoWidget->setFullScreen(false);
}

void VideoPlayer::setElapsedTime(qint64 elapsedTime)
{
    this->m_pElapsedTimeLabel->setText(convertMs(elapsedTime));
}

QString VideoPlayer::convertMs(qint64 ms)
{
    QString result = "";

    int hours = ((ms / 1000) / 3600) % 24;
    int minutes = ((ms / 1000) / 60) % 60;
    int seconds = (ms / 1000) % 60;

    result = QString("%L1:%L2:%L3").arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));

    return result;
}

void VideoPlayer::initControls()
{
    this->m_pPlayButton = new QPushButton();
    this->m_pPlayButton->setToolTip(tr("Play"));
    this->m_pPlayButton->setEnabled(false);
    this->m_pPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(this->m_pPlayButton, SIGNAL(clicked()), this, SLOT(play()));

    this->m_pStopButton = new QPushButton();
    this->m_pStopButton->setToolTip(tr("Stop"));
    this->m_pStopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    connect(this->m_pStopButton, SIGNAL(clicked(bool)),
            &this->m_MediaPlayer, SLOT(stop()));

    this->m_pElapsedTimeLabel = new QLabel();
    this->m_pElapsedTimeLabel->setText("00:00:00");
    this->m_pElapsedTimeLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    this->m_pPositionSlider = new CustomSlider(Qt::Horizontal);
    this->m_pPositionSlider->setRange(0, 0);
    this->m_pPositionSlider->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
    connect(this->m_pPositionSlider, SIGNAL(sliderMoved(int)),
            this, SLOT(setPosition(int)));

    this->m_pDurationLabel = new QLabel();
    this->m_pDurationLabel->setText("00:00:00");
    this->m_pDurationLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    this->m_pVolumeButton = new QPushButton();
    this->m_pVolumeButton->setEnabled(true);
    this->m_pVolumeButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    this->m_pVolumeButton->setToolTip(tr("Mute"));
    connect(this->m_pVolumeButton, SIGNAL(clicked(bool)),
            this, SLOT(setMuted()));

    this->m_pVolumeSlider = new CustomSlider(Qt::Horizontal);
    this->m_pVolumeSlider->setRange(0, 100);
    this->m_pVolumeSlider->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    this->m_pVolumeSlider->setValue(100);
    connect(this->m_pVolumeSlider, SIGNAL(sliderMoved(int)),
            &this->m_MediaPlayer, SLOT(setVolume(int)));
}

void VideoPlayer::setFullscreenControls()
{
    this->m_pFullscreenControls = this->m_pVideoWidget->getFullscreenControls();

    connect(this->m_pFullscreenControls->getPositionSlider(), SIGNAL(sliderMoved(int)),
            this, SLOT(setPosition(int)));

    connect(this->m_pFullscreenControls->getVolumeSlider(), SIGNAL(sliderMoved(int)),
            &this->m_MediaPlayer, SLOT(setVolume(int)));

    connect(this->m_pFullscreenControls->getStopButton(), SIGNAL(clicked(bool)),
            &this->m_MediaPlayer, SLOT(stop()));

    connect(this->m_pFullscreenControls->getPlayButton(), SIGNAL(clicked(bool)),
            this, SLOT(play()));

    connect(this->m_pFullscreenControls->getVolumeButton(), SIGNAL(clicked(bool)),
            this, SLOT(setMuted()));
}

void VideoPlayer::initMediaPlayer()
{
    this->m_MediaPlayer.setMuted(false);
    this->m_MediaPlayer.setVideoOutput(this->m_pVideoWidget);

    connect(&this->m_MediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(mediaStateChanged(QMediaPlayer::State)));

    connect(&this->m_MediaPlayer, SIGNAL(positionChanged(qint64)),
            this, SLOT(positionChanged(qint64)));

    connect(&this->m_MediaPlayer, SIGNAL(positionChanged(qint64)),
            this->m_pFullscreenControls, SLOT(setPosition(qint64)));

    connect(&this->m_MediaPlayer, SIGNAL(positionChanged(qint64)),
            this, SLOT(setElapsedTime(qint64)));

    connect(&this->m_MediaPlayer, SIGNAL(durationChanged(qint64)),
            this, SLOT(durationChanged(qint64)));

    connect(&this->m_MediaPlayer, SIGNAL(durationChanged(qint64)),
            this->m_pFullscreenControls, SLOT(setDuration(qint64)));

    connect(&this->m_MediaPlayer, SIGNAL(volumeChanged(int)),
            this, SLOT(volumeChanged(int)));

    connect(&this->m_MediaPlayer, SIGNAL(error(QMediaPlayer::Error)),
            this, SLOT(handleError()));

    connect(&this->m_MediaPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(mediaStatus(QMediaPlayer::MediaStatus)));
}

void VideoPlayer::setMuted()
{
    if(this->m_MediaPlayer.isMuted() == false)
    {
        this->m_MediaPlayer.setMuted(true);
        this->m_pVolumeButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
        this->m_pVolumeButton->setToolTip(tr("Unmute"));
        this->m_pFullscreenControls->setVolumeButton(true);
    }
    else
    {
        this->m_MediaPlayer.setMuted(false);
        this->m_pVolumeButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
        this->m_pVolumeButton->setToolTip(tr("Mute"));
        this->m_pFullscreenControls->setVolumeButton(false);
    }
}

void VideoPlayer::mediaStatus(QMediaPlayer::MediaStatus status)
{
    if(status == QMediaPlayer::LoadedMedia)
    {
        this->m_pPlayButton->setEnabled(true);
        if(this->m_IsMediaLoaded == false)
        {
            this->m_MediaPlayer.play();
            this->m_IsMediaLoaded = true;
        }
    }
}
