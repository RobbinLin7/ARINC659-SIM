#include "simulinkwidget.h"
#include "ui_simulinkwidget.h"

#include "faultwidget.h"

SimulinkWidget::SimulinkWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimulinkWidget)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("仿真"));
}

SimulinkWidget::~SimulinkWidget()
{
    delete ui;
}

void SimulinkWidget::on_faultInjectionBtn_clicked()
{
    FaultWidget *faultInjection = new FaultWidget();


    faultInjection->show();
}
