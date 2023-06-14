#include "dataFrame.h"
#include "ui_dataFrame.h"

DataFrame::DataFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataFrame)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("帧配置"));
}

DataFrame::~DataFrame()
{
    delete ui;
}
