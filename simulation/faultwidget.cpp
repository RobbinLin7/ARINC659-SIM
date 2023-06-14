#include "faultwidget.h"
#include "ui_faultwidget.h"

FaultWidget::FaultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FaultWidget)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("故障注入"));
}

FaultWidget::~FaultWidget()
{
    delete ui;
}
