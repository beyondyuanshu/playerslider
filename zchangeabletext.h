#ifndef ZCHANGEABLETEXT_H
#define ZCHANGEABLETEXT_H

#include <QWidget>

class ZChangeableText : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(qreal radius READ radius WRITE setRadius)

public:
    explicit ZChangeableText(QWidget *parent = nullptr);

    void setRadius(const qreal radius) { m_radius = radius; }
    qreal radius() { return m_radius; }

public slots:
    void setRadius(const int radius);

protected:
    void paintEvent(QPaintEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

signals:

public slots:

private:
    void paintText(const QString &text, bool isLeft);

    qreal m_radius;
    QPointF m_centerPoint;
};

#endif // ZCHANGEABLETEXT_H
