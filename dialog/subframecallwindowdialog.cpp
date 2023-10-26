#include "subframecallwindowdialog.h"
#include "ui_subframecallwindowdialog.h"

SubFrameCallWindowDialog::SubFrameCallWindowDialog(uint id, QWidget *parent) :
    WindowCfgDialog(parent),
    ui(new Ui::SubFrameCallWindowDialog)
{
    ui->setupUi(this);
    window.setWindowType(FrameWindow::CALL_SUBFRRAME);
    ui->windowId_lineEdit->setText(QString::number(id));
}

SubFrameCallWindowDialog::~SubFrameCallWindowDialog()
{
    delete ui;
}

void SubFrameCallWindowDialog::on_okPushButton_clicked()
{
    addNewWindowfunc();
}
