#include "hardwareModel.h"
#include "ui_hardwareModel.h"

HardwareModel::HardwareModel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HardwareModel)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("模块配置"));
}

HardwareModel::~HardwareModel()
{
    delete ui;
}
