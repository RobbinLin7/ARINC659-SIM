#include "freewindowcfgdialog.h"
#include "ui_freewindowcfgdialog.h"

FreeWindowCfgDialog::FreeWindowCfgDialog(uint id, QWidget *parent) :
    WindowCfgDialog(parent),
    ui(new Ui::FreeWindowCfgDialog)
{
    ui->setupUi(this);
    window.setWindowType(FrameWindow::FREE);
    ui->windowId_lineEdit->setText(QString::number(id));
}

FreeWindowCfgDialog::~FreeWindowCfgDialog()
{
    delete ui;
}

void FreeWindowCfgDialog::on_okPushButton_clicked()
{
    addNewWindowfunc();
}
