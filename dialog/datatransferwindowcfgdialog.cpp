#include "datatransferwindowcfgdialog.h"
#include "ui_datatransferwindowcfgdialog.h"

DataTransferWindowCfgDialog::DataTransferWindowCfgDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataTransferWindowCfgDialog)
{
    ui->setupUi(this);
}

DataTransferWindowCfgDialog::~DataTransferWindowCfgDialog()
{
    delete ui;
}
