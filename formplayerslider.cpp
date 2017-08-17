#include "formplayerslider.h"
#include "ui_formplayerslider.h"

#include <QScrollBar>
#include <QTimer>
#include <QDebug>

FormPlayerSlider::FormPlayerSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPlayerSlider)
{
    ui->setupUi(this);

    connect(ui->checkBox_hour, SIGNAL(stateChanged(int)), SLOT(slot_accuracyChanged(int)));
    connect(ui->checkBox_minute, SIGNAL(stateChanged(int)), SLOT(slot_accuracyChanged(int)));
    connect(ui->checkBox_second, SIGNAL(stateChanged(int)), SLOT(slot_accuracyChanged(int)));

    connect(ui->pushButton, SIGNAL(clicked()), SLOT(slot_btnClicked()));

    connect(ui->scrollArea->horizontalScrollBar(), SIGNAL(rangeChanged(int, int)),
            SLOT(slot_adjustVerticalScrollBarValue()));
    connect(ui->slider, SIGNAL(adjustScrollBar(int)), SLOT(slot_adjustVerticalScrollBarValue(int)));

    ui->slider->setScrollArea(ui->scrollArea);
}

FormPlayerSlider::~FormPlayerSlider()
{
    delete ui;
}

void FormPlayerSlider::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    ui->slider->setOriginWidth(width());
}

void FormPlayerSlider::slot_accuracyChanged(int state)
{
    if (Qt::Checked != state) {
        return;
    }

    QCheckBox *checkBox = qobject_cast<QCheckBox *>(sender());
    if (ui->checkBox_hour == checkBox) {
        ui->slider->setAccuracyType(ZPlayerSlider::Hour);
    } else if (ui->checkBox_minute == checkBox) {
        ui->slider->setAccuracyType(ZPlayerSlider::Minute);
    } else if (ui->checkBox_second == checkBox) {
        ui->slider->setAccuracyType(ZPlayerSlider::Second);
    }
}

void FormPlayerSlider::slot_btnClicked()
{
    qDebug() << ui->scrollArea->horizontalScrollBar()->maximum()
             << ui->slider->width()
             << ui->scrollArea->horizontalScrollBar()->value()
             << ui->scrollArea->horizontalScrollBar()->pageStep()
             << ui->slider->width()
             << ui->slider->getCurrentPosX();

}

void FormPlayerSlider::slot_adjustVerticalScrollBarValue()
{
    int pageStep = ui->scrollArea->horizontalScrollBar()->pageStep();
    ui->scrollArea->horizontalScrollBar()->setValue(int(ui->slider->getCurrentPosX()) - pageStep / 2);
    slot_btnClicked();
}

void FormPlayerSlider::slot_adjustVerticalScrollBarValue(int changedValue)
{
    int value = ui->scrollArea->horizontalScrollBar()->value();
    ui->scrollArea->horizontalScrollBar()->setValue(value + changedValue);
}
