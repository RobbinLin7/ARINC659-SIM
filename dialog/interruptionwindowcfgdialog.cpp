#include "interruptionwindowcfgdialog.h"
#include "ui_interruptionwindowcfgdialog.h"

InterruptionWindowCfgDialog::InterruptionWindowCfgDialog(uint id, QWidget *parent) :
    WindowCfgDialog(parent),
    ui(new Ui::InterruptionWindowCfgDialog)
{
    ui->setupUi(this);
    window.setWindowType(FrameWindow::INT_SEND);
    ui->windowId_lineEdit->setText(QString::number(id));
}

InterruptionWindowCfgDialog::~InterruptionWindowCfgDialog()
{
    delete ui;
}

void InterruptionWindowCfgDialog::on_okPushButton_clicked()
{
    addNewWindowfunc();
}
