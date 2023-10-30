#include "interruptionwindowcfgdialog.h"
#include "ui_interruptionwindowcfgdialog.h"

InterruptionWindowCfgDialog::InterruptionWindowCfgDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InterruptionWindowCfgDialog)
{
    ui->setupUi(this);
}

InterruptionWindowCfgDialog::~InterruptionWindowCfgDialog()
{
    delete ui;
}
