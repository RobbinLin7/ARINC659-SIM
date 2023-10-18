#include "datatransferwindowcfgdialog.h"
#include "ui_datatransferwindowcfgdialog.h"

DataTransferWindowCfgDialog::DataTransferWindowCfgDialog(QWidget *parent) :
    WindowCfgDialog(parent),
    ui(new Ui::DataTransferWindowCfgDialog)
{
    ui->setupUi(this);
    window.setWindowType(FrameWindow::DATA_SEND);
}

DataTransferWindowCfgDialog::~DataTransferWindowCfgDialog()
{
    delete ui;
}

void DataTransferWindowCfgDialog::on_okPushButton_clicked()
{
    qDebug() << "DataTransferWindowCfgDialog";
    addNewWindowfunc();
}
