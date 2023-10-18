#include "interruptionwindowcfgdialog.h"
#include "ui_interruptionwindowcfgdialog.h"

InterruptionWindowCfgDialog::InterruptionWindowCfgDialog(QWidget *parent) :
    WindowCfgDialog(parent),
    ui(new Ui::InterruptionWindowCfgDialog)
{
    ui->setupUi(this);
    window.setWindowType(FrameWindow::INT_SEND);
}

InterruptionWindowCfgDialog::~InterruptionWindowCfgDialog()
{
    delete ui;
}

void InterruptionWindowCfgDialog::on_okPushButton_clicked()
{
    addNewWindowfunc();
}
