#ifndef FORMPLAYERSLIDER_H
#define FORMPLAYERSLIDER_H

#include <QWidget>

namespace Ui
{
class FormPlayerSlider;
}

class FormPlayerSlider : public QWidget
{
    Q_OBJECT

public:
    explicit FormPlayerSlider(QWidget *parent = 0);
    ~FormPlayerSlider();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void slot_accuracyChanged(int state);
    void slot_btnClicked();

    void slot_adjustVerticalScrollBarValue();
    void slot_adjustVerticalScrollBarValue(int changedValue);

private:
    Ui::FormPlayerSlider *ui;
};

#endif // FORMPLAYERSLIDER_H
