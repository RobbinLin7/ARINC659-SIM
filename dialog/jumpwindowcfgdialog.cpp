#include "jumpwindowcfgdialog.h"
#include "ui_jumpwindowcfgdialog.h"

JumpWindowCfgDialog::JumpWindowCfgDialog(uint id, QWidget *parent) :
    WindowCfgDialog(parent),
    ui(new Ui::JumpWindowCfgDialog)
{
    ui->setupUi(this);
    window.setWindowType(FrameWindow::FRAME_JUMP);
    ui->windowId_lineEdit->setText(QString::number(id));
}

JumpWindowCfgDialog::~JumpWindowCfgDialog()
{
    delete ui;
}

void JumpWindowCfgDialog::on_okPushButton_clicked()
{
    addNewWindowfunc();
}
