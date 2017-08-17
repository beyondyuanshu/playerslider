#include "zplayerslider.h"

#include <QtWidgets>
#include <QVBoxLayout>
#include <QDebug>

ZPlayerSlider::ZPlayerSlider(QWidget *parent) : QWidget(parent)
    , scrollArea(nullptr)
    , m_accuracyType(Hour)
    , m_originWidth(0)
    , m_grooveSpacing(0)
    , m_margin(0)
    , m_passedPlayedArea()
    , m_handleArea()
    , m_handleRadius(10.0)
    , m_handleInitRadius(m_handleRadius)
    , m_backwardsSeconds(0)
    , m_isMousePressedInHandelArea(false)
    , previewImagePosX(-99)
    , m_currentPosX(0)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);

    setMouseTracking(true);

    resize(1000, 100);  // if not set size, the widget will be invisible
}

void ZPlayerSlider::setScrollArea(QScrollArea *scrollArea)
{
    this->scrollArea = scrollArea;
}

void ZPlayerSlider::setAccuracyType(const ZPlayerSlider::AccuracyType &type)
{
    m_accuracyType = type;
    update();
}

void ZPlayerSlider::setOriginWidth(int w)
{
    m_originWidth = w;
    update();
}

void ZPlayerSlider::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    int baselineOffset = 4;
    int scaleHeight = 10;
    int scaleBigHeight = 15;

    int steps = getSteps(m_accuracyType);
    int hourSteps = getSteps(Hour);

    m_grooveSpacing = m_originWidth / (24 * hourSteps);
    m_groovePerSecondSpacing = (m_grooveSpacing * steps * 24 * 1.0) / (3600 * 24);
    m_margin = (m_originWidth % (24 * hourSteps)) / 2;

    setFixedSize(m_grooveSpacing * steps * 24 + m_margin * 2, 100); // setMinimumSize is important

    qint64 currentSeconds = -QDateTime::currentDateTime().secsTo(QDateTime(QDate::currentDate(),
                                                                           QTime()));

    m_currentPosX = qint64((currentSeconds - m_backwardsSeconds) * m_groovePerSecondSpacing) + m_margin;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // central line
    painter.save();
    painter.setPen(QPen("darkgray"));
    painter.drawLine(m_margin, height() / 2, width() - m_margin, height() / 2);
    painter.restore();

    painter.save();
    painter.setPen(QPen("silver"));
    painter.drawLine(0, height() / 2, m_margin, height() / 2);
    painter.drawLine(width() - m_margin, height() / 2, width(), height() / 2);
    painter.restore();

    m_passedPlayedArea = QRectF(m_margin * 1.0, qreal(height() / 2 - baselineOffset),
                                currentSeconds * m_groovePerSecondSpacing, baselineOffset * 2);

    painter.save();
    const QRectF playedArea = QRectF(m_margin * 1.0, qreal(height() / 2 - baselineOffset),
                                     (currentSeconds - m_backwardsSeconds) * m_groovePerSecondSpacing, baselineOffset * 2);
    painter.setPen(QPen("white"));
    painter.setBrush(QBrush("dodgerblue"));
    painter.drawRoundedRect(playedArea, 5.0, 5.0);
    painter.restore();

    painter.save();
    qreal backwardsPlayedAreaLeft = m_margin * 1.0 + (currentSeconds - m_backwardsSeconds) *
                                    m_groovePerSecondSpacing;
    qreal backwardsPlayedAreaWidth = m_backwardsSeconds * m_groovePerSecondSpacing;
    const QRectF backwardsPlayedArea = QRectF(backwardsPlayedAreaLeft,
                                              qreal(height() / 2 - baselineOffset),
                                              backwardsPlayedAreaWidth, baselineOffset * 2);
    painter.setPen(QPen("white"));
    painter.setBrush(QBrush("darkorange"));
    painter.drawRoundedRect(backwardsPlayedArea, 5.0, 5.0);
    painter.restore();

    // top scale
    painter.save();
    painter.setPen(QPen("gray"));
    for (int i = 0; i <= (24 * steps); ++i) {
        int x1, y1, x2, y2;
        x1 = x2 = m_margin + m_grooveSpacing * i + 1;
        if ((i % steps) == 0) {
            y1 = height() / 2 - scaleBigHeight;
        } else {
            y1 = height() / 2 - scaleHeight;
        }
        y2 = height() / 2 - baselineOffset;
        painter.drawLine(x1, y1, x2, y2);
    }
    painter.restore();

    // bottom scale
    painter.save();
    for (int i = 0; i <= (24 * steps); ++i) {
        int x1, y1, x2, y2;
        x1 = x2 = m_margin + m_grooveSpacing * i + 1;
        y1 = height() / 2 + baselineOffset;
        if ((i % steps) == 0) {
            y2 = height() / 2 + scaleBigHeight;
        } else {
            y2 = height() / 2 + scaleHeight;
        }
        painter.setPen(QPen("gray"));
        painter.drawLine(x1, y1, x2, y2);

        if (/*((i % steps) == 0)
                && ((i / steps) > 0)
                && ((i / steps) < 24)*/
            i % 10 == 0) {
            int x, y;
            x = m_margin + m_grooveSpacing * i + 1 - 25;
            y = height() / 2 + 20 + 10;
            const QRect rect = QRect(x, y, 50, 10);

            QTime time(i / steps, 0);

            QFont font = painter.font();
            font.setPixelSize(10);
            painter.setFont(font);
            painter.setPen(QPen("dimgray"));
            painter.drawText(rect,
                             Qt::AlignCenter, /*time.toString("hh:00")*/QDateTime::currentDateTime().toString("hh:mm:ss"));
        }
    }
    painter.restore();

    // handle
    painter.save();
    qreal handleLeft = m_margin * 1.0 + (currentSeconds - m_backwardsSeconds) * m_groovePerSecondSpacing
                       - m_handleRadius;
    qreal handleTop = height() / 2 - m_handleRadius;
    m_handleArea = QRectF(handleLeft, handleTop, m_handleRadius * 2, m_handleRadius * 2);
    int startAngle = 0;
    int spanAngle = 360 * 16;
    painter.setPen(QPen("lightgray"));
    painter.setBrush(QBrush("white"));
    painter.drawChord(m_handleArea, startAngle, spanAngle);
    painter.restore();

    painter.save();
    qreal imageWidth = 40.0;
    qreal imageHeight = 40.0;
    QRectF imageTarget((previewImagePosX - imageWidth / 2) * 1.0,
                       height() * 1.0 / 2 - 4.0 - imageHeight,
                       imageWidth,
                       imageHeight);
    QImage image(":/images/qt_logo.png");
    QRectF imageSource(0.0, 0.0, image.width(), image.height());
    painter.drawImage(imageTarget, image, imageSource);
    painter.restore();

    painter.save();
    qreal handleTextLeft = handleLeft - 25.0 + m_handleRadius;
    qreal handleTextTop = handleTop + m_handleRadius * 2 + 4.0;
    const QRectF handleTextRectf = QRectF(handleTextLeft, handleTextTop, 60.0, 14.0);
    painter.setPen(QPen("dodgerblue"));
    painter.setBrush(QBrush("dodgerblue"));
    painter.drawRoundedRect(handleTextRectf, 2.0, 2.0);
    painter.restore();

    painter.save();
    QFont font = painter.font();
    font.setPixelSize(10);
    painter.setFont(font);
    painter.setPen(QPen("white"));
    QTime playingTime = QTime::fromMSecsSinceStartOfDay(int(currentSeconds - m_backwardsSeconds) *
                                                        1000);
    painter.drawText(handleTextRectf, Qt::AlignCenter, playingTime.toString("hh:mm:ss"));
    painter.restore();
}

void ZPlayerSlider::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        QPointF localPos = event->localPos();
        if (m_handleArea.contains(localPos)) {
            m_isMousePressedInHandelArea = true;
            m_handleRadius *= 1.2;
            update();
        }
    }
}

void ZPlayerSlider::mouseMoveEvent(QMouseEvent *event)
{
    QPointF localPos = event->localPos();
    if (event->buttons() & Qt::LeftButton) {
        if (localPos.x() >= m_margin) {
            QDateTime currentDateTime = QDateTime::currentDateTime();
            qint64 currentSecs = -currentDateTime.secsTo(QDateTime(QDate::currentDate(),
                                                                   QTime()));
            m_backwardsSeconds = currentSecs - qint64((localPos.x() - m_margin) / m_groovePerSecondSpacing);
            if (m_backwardsSeconds < 0)
                m_backwardsSeconds = 0;
            update();
        }

        if (scrollArea) {
            QPoint pos = scrollArea->mapFromGlobal(cursor().pos());
            if (pos.x() >= m_originWidth) {
                emit adjustScrollBar(pos.x() - m_originWidth);
            } else if (pos.x() < 0) {
                emit adjustScrollBar(pos.x());
            }
        }
    }

    if (m_handleArea.contains(localPos)
            || m_passedPlayedArea.contains(localPos)) {
        setCursor(QCursor(Qt::PointingHandCursor));
        previewImagePosX = int(localPos.x());
        update();
    } else {
        setCursor(QCursor(Qt::ArrowCursor));
        previewImagePosX = -99;
        update();
    }
}

void ZPlayerSlider::mouseReleaseEvent(QMouseEvent *event)
{
    QPointF localPos = event->localPos();
    if (m_passedPlayedArea.contains(localPos)
            && !m_handleArea.contains(localPos)
            && !m_isMousePressedInHandelArea) {
        QDateTime currentDateTime = QDateTime::currentDateTime();
        qint64 currentSecs = -currentDateTime.secsTo(QDateTime(QDate::currentDate(),
                                                               QTime()));
        m_backwardsSeconds = currentSecs - qint64(localPos.x() / m_groovePerSecondSpacing);
    }

    m_handleRadius = m_handleInitRadius;
    update();

    m_isMousePressedInHandelArea = false;
}

void ZPlayerSlider::enterEvent(QEvent *event)
{
    QPoint pos = mapFromGlobal(cursor().pos());
//    qDebug() << pos;
//    QPointF localPos = this->pos();
//    if (m_passedPlayedArea.contains(localPos))
}

void ZPlayerSlider::leaveEvent(QEvent *event)
{

}

int ZPlayerSlider::getSteps(const AccuracyType &accuracyType)
{
    int steps;
    switch (accuracyType) {
    case Minute:
        steps = 10;
        break;
    case Second:
        steps = 3600;
        break;
    case Hour:
    default:
        steps = 4;
        break;
    }
    return steps;
}
