#include "zchangeabletext.h"

#include <QtWidgets>

#include <QDebug>

static const qreal pai = 3.14159265;

ZChangeableText::ZChangeableText(QWidget *parent) : QWidget(parent)
{
    resize(1000, 400);
    startTimer(1000);

    m_radius = qMin(width(), height()) * 1.0 - 30;

    // test animation
    //    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    //    animation->setDuration(1000);
    //    animation->setStartValue(QRect(0, 0, 100, 30));
    //    animation->setEndValue(QRect(250, 250, 100, 30));
    //    animation->start();

    QTimeLine *timeLine = new QTimeLine(10000, this);
    connect(timeLine, SIGNAL(frameChanged(int)), SLOT(setRadius(int)));
    timeLine->setFrameRange(100, 150);
    timeLine->setLoopCount(0);
    timeLine->start();
}

void ZChangeableText::setRadius(const int radius)
{
    m_radius = radius * 1.0;
}

void ZChangeableText::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.save();
    //    m_radius = qMin(width(), height()) * 1.0 - 30;
    m_centerPoint = QPointF(width() * 1.0 / 2, height() * 1.0);
    QRectF rectangle(width() * 1.0 / 2 - m_radius, height() - m_radius, m_radius * 2, m_radius * 2);
    painter.setPen("red");
    painter.drawArc(rectangle, 0, 180 * 16);
    painter.restore();

    painter.save();
    QFont font = painter.font();
    font.setPixelSize(100);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen("Green");
    QRectF qtRect = QRectF(m_centerPoint.x() - m_radius, m_centerPoint.y() - m_radius, m_radius * 2,
                           m_radius);
    painter.drawText(qtRect, Qt::AlignCenter, "Qt");
    painter.restore();

    paintText("Hammer", true);
    paintText("Yang chenglin", false);
}

void ZChangeableText::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    update();
}

void ZChangeableText::paintText(const QString &text, bool isLeft)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int len = text.length();
    for (int i = 0; i < len; ++i) {
        qreal angleOffsetXAxis = (((2 * pai * 0.25) * 1.0) / (len + 1)) * (i + 1)
                                 + (isLeft ? pai * 0.5 : 0);
        QPointF targetPoint = QPointF(m_centerPoint.x() + qCos(angleOffsetXAxis) * m_radius,
                                      m_centerPoint.y() - qSin(angleOffsetXAxis) * m_radius);

        qreal width = ((2 * pai * m_radius) * 0.25) / (len + 1);
        qreal height = width;
        QRectF rectangle = QRectF(-width / 2, -height, width, height);

        painter.save();
        painter.translate(targetPoint.x(), targetPoint.y());
        painter.rotate(90 - (angleOffsetXAxis * 90) / (pai / 2));
        QFont font = painter.font();
        font.setPixelSize(int(width));
        font.setBold(true);
        painter.setFont(font);
        qsrand(uint(QTime::currentTime().second()));
        painter.setPen(QColor(qrand() % 255, qrand() % 255, qrand() % 255));
        painter.drawText(rectangle, Qt::AlignCenter, text.at(len - i - 1));
        painter.restore();
    }
}
