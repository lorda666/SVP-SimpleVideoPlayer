#include "fullscreencontrols.h"

#include <QStyle>
#include <QPalette>
#include <QPainter>
#include <QColor>

FullscreenControls::FullscreenControls(QWidget *parent) : QWidget(parent)
{
    this->m_pElapsedTimeLabel = new QLabel();
    this->m_pElapsedTimeLabel->setText("00:00:00");
    this->m_pElapsedTimeLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

    this->m_pPositionSlider = new CustomSlider(Qt::Horizontal);
    this->m_pPositionSlider->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    this->m_pDurationLabel = new QLabel();
    this->m_pDurationLabel->setText("00:00:00");
    this->m_pDurationLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

    this->m_pTimeLineLayout = new QHBoxLayout();
    this->m_pTimeLineLayout->addWidget(this->m_pElapsedTimeLabel);
    this->m_pTimeLineLayout->addWidget(this->m_pPositionSlider);
    this->m_pTimeLineLayout->addWidget(this->m_pDurationLabel);

    this->m_pPlayButton = new QPushButton();
    this->m_pPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    this->m_pStopButton = new QPushButton();
    this->m_pStopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));

    this->m_pVolumeButton = new QPushButton();
    this->m_pVolumeButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

    this->m_pVolumeSlider = new CustomSlider(Qt::Horizontal);
    this->m_pVolumeSlider->setRange(0, 100);
    this->m_pVolumeSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    this->m_pControlsLayout = new QHBoxLayout();
    this->m_pControlsLayout->addWidget(this->m_pPlayButton);
    this->m_pControlsLayout->addWidget(this->m_pStopButton);
    this->m_pControlsLayout->addWidget(this->m_pVolumeButton);
    this->m_pControlsLayout->addWidget(this->m_pVolumeSlider);
    this->m_pControlsLayout->setAlignment(Qt::AlignCenter);

    QBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addLayout(this->m_pTimeLineLayout);
    pLayout->addLayout(this->m_pControlsLayout);
    pLayout->setMargin(0);
    pLayout->setContentsMargins(5, 2, 5, 2);

    this->m_pControlsLayout->setAlignment(pLayout, Qt::AlignHCenter);

    this->setLayout(pLayout);

    this->setMouseTracking(true);
}

FullscreenControls::~FullscreenControls()
{

}

void FullscreenControls::paintEvent(QPaintEvent *pEvent)
{
    QColor backgroundColor = this->palette().color(QPalette::Background);
    backgroundColor.setRgba(qRgba(255, 255, 255, 255));

    QPainter painter(this);
    painter.fillRect(this->rect(), backgroundColor);

    QWidget::paintEvent(pEvent);
}

void FullscreenControls::mouseMoveEvent(QMouseEvent *pEvent)
{
    emit this->cursorMoved();
}

QPushButton *FullscreenControls::getPlayButton()
{
    return this->m_pPlayButton;
}

QPushButton *FullscreenControls::getStopButton()
{
    return this->m_pStopButton;
}

QPushButton *FullscreenControls::getVolumeButton()
{
    return this->m_pVolumeButton;
}

CustomSlider* FullscreenControls::getPositionSlider()
{
    return this->m_pPositionSlider;
}

CustomSlider* FullscreenControls::getVolumeSlider()
{
    return this->m_pVolumeSlider;
}

void FullscreenControls::setPlayButton()
{
    this->m_pPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    this->m_pPlayButton->setToolTip("Play");
}

void FullscreenControls::setPauseButton()
{
    this->m_pPlayButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    this->m_pPlayButton->setToolTip("Pause");
}

void FullscreenControls::setVolumeButton(QPushButton *pVolumeButton)
{
    this->m_pVolumeButton = pVolumeButton;
}

void FullscreenControls::setVolumeSlider(int volume)
{
    this->m_pVolumeSlider->setValue(volume);
}

void FullscreenControls::setElapsedTimeLabel(QLabel *pElapsedTimeLabel)
{
    this->m_pElapsedTimeLabel = pElapsedTimeLabel;
}

void FullscreenControls::setDurationLabel(QLabel *pDurationLabel)
{
    this->m_pDurationLabel = pDurationLabel;
}

void FullscreenControls::setPosition(qint64 position)
{
    this->m_pPositionSlider->setValue(position);
    this->m_pElapsedTimeLabel->setText(convertMs(position));
}

void FullscreenControls::setDuration(qint64 duration)
{
    this->m_pPositionSlider->setRange(0, duration);
    this->m_pDurationLabel->setText(convertMs(duration));
}

QString FullscreenControls::convertMs(qint64 ms)
{
    QString result = "";

    int hours = ((ms / 1000) / 3600) % 24;
    int minutes = ((ms / 1000) / 60) % 60;
    int seconds = (ms / 1000) % 60;

    result = QString("%L1:%L2:%L3").arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));

    return result;
}
