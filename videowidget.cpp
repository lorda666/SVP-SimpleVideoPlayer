#include "videowidget.h"
#include <QApplication>
#include <QScreen>
#include <QPalette>
#include <QPainter>
#include <QColor>

VideoWidget::VideoWidget(QWidget *parent) : QVideoWidget(parent)
{
    this->setMouseTracking(true);
    this->m_pTimer = new QTimer;
    this->m_pTimer->setInterval(3000);

    connect(this->m_pTimer, SIGNAL(timeout()),
            this, SLOT(hideFullscreenControls()));

    this->m_CursorPos = QPoint(420, 850);
    this->m_CursorMoved = false;

    connect(this, SIGNAL(mouseStopped()), this->m_pTimer, SLOT(start()));

    this->initFullscreenControls();
    connect(this->m_pFullscreenControls, SIGNAL(cursorMoved()),
            this->m_pTimer, SLOT(stop()));

    this->setAutoFillBackground(true);
}

VideoWidget::~VideoWidget()
{

}

void VideoWidget::setFullScreenControls(FullscreenControls *pControls)
{
    this->m_pFullscreenControls = pControls;
}

FullscreenControls* VideoWidget::getFullscreenControls()
{
    return this->m_pFullscreenControls;
}

void VideoWidget::hideFullscreenControls()
{
    this->m_pFullscreenControls->hide();
    if(this->isFullScreen() == true)
    {
        this->setCursor(Qt::BlankCursor);
    }
}

void VideoWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
    if(this->isFullScreen() == true)
    {
        this->m_pFullscreenControls->show();
        this->setCursor(Qt::ArrowCursor);
        this->m_pTimer->stop();

        emit this->mouseStopped();
    }
}

void VideoWidget::paintEvent(QPaintEvent *pEvent)
{
    QColor backgroundColor = this->palette().color(QPalette::Background);
    backgroundColor.setRgba(qRgba(0, 0, 0, 255));

    QPainter painter(this);
    painter.fillRect(this->rect(), backgroundColor);

    //QVideoWidget::paintEvent(pEvent);
}

void VideoWidget::initFullscreenControls()
{
    this->m_pFullscreenControls = new FullscreenControls(this);

    this->m_pFullscreenControls->hide();

    QScreen *pScreenData = QApplication::screens().at(0);

    QRect screenRect = pScreenData->geometry();

    this->m_pFullscreenControls->setMinimumSize(screenRect.width() / 2,
                                                this->m_pFullscreenControls->height());
    this->m_pFullscreenControls->setMaximumSize(screenRect.width() / 2,
                                                this->m_pFullscreenControls->height());
    this->m_pFullscreenControls->resize(screenRect.width() / 2,
                                        this->m_pFullscreenControls->height());

    this->m_pFullscreenControls->layout()->setSizeConstraint(QLayout::SetMinAndMaxSize);

    int xPos = this->m_pFullscreenControls->width() / 2;
    int yPos = this->m_pFullscreenControls->height() * 2;

    this->m_pFullscreenControls->move(xPos, screenRect.height() - yPos);
    this->m_pFullscreenControls->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
}
