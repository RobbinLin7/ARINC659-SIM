#include "freewindowcfgdialog.h"
#include "ui_freewindowcfgdialog.h"

FreeWindowCfgDialog::FreeWindowCfgDialog(QWidget *parent) :
    WindowCfgDialog(parent),
    ui(new Ui::FreeWindowCfgDialog)
{
    ui->setupUi(this);
    window.setWindowType(FrameWindow::FREE);
}

FreeWindowCfgDialog::~FreeWindowCfgDialog()
{
    delete ui;
}

void FreeWindowCfgDialog::on_okPushButton_clicked()
{
    addNewWindowfunc();
}
