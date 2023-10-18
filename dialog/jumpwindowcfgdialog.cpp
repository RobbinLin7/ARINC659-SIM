#include "jumpwindowcfgdialog.h"
#include "ui_jumpwindowcfgdialog.h"

JumpWindowCfgDialog::JumpWindowCfgDialog(QWidget *parent) :
    WindowCfgDialog(parent),
    ui(new Ui::JumpWindowCfgDialog)
{
    ui->setupUi(this);
    window.setWindowType(FrameWindow::FRAME_JUMP);
}

JumpWindowCfgDialog::~JumpWindowCfgDialog()
{
    delete ui;
}

void JumpWindowCfgDialog::on_okPushButton_clicked()
{
    addNewWindowfunc();
}
