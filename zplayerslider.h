#ifndef ZPLAYERPROGRESSBAR_H
#define ZPLAYERPROGRESSBAR_H

#include <QWidget>
#include <QScrollArea>

class ZPlayerSlider : public QWidget
{
    Q_OBJECT
public:
    explicit ZPlayerSlider(QWidget *parent = nullptr);

    enum AccuracyType {
        Hour = 0,
        Minute = 1,
        Second = 2
    };

signals:
    void adjustScrollBar();
    void adjustScrollBar(int changedValue);

public:
    void setScrollArea(QScrollArea *scrollArea);

    void setAccuracyType(const AccuracyType &type);
    void setOriginWidth(int w);

    inline qint64 getCurrentPosX() {
        return m_currentPosX;
    }

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QScrollArea *scrollArea;

    AccuracyType m_accuracyType;

    int m_originWidth;
    int m_grooveSpacing;
    qreal m_groovePerSecondSpacing;
    int m_margin;

    QRectF m_passedPlayedArea;

    QRectF m_handleArea;
    qreal m_handleRadius;
    qreal m_handleInitRadius;

    qint64 m_backwardsSeconds;
    bool m_isMousePressedInHandelArea;

    int previewImagePosX;
    qint64 m_currentPosX;

    int getSteps(const AccuracyType &accuracyType);
};

#endif // ZPLAYERPROGRESSBAR_H
