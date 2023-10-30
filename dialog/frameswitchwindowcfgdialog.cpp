#include "frameswitchwindowcfgdialog.h"
#include "ui_frameswitchwindowcfgdialog.h"

FrameSwitchWindowCfgDialog::FrameSwitchWindowCfgDialog(uint id, QWidget *parent) :
    WindowCfgDialog(parent),
    ui(new Ui::FrameSwitchWindowCfgDialog)
{
    ui->setupUi(this);
    window.setWindowType(FrameWindow::FRAME_SWITCH);
    ui->windowId_lineEdit->setText(QString::number(id));
}

FrameSwitchWindowCfgDialog::~FrameSwitchWindowCfgDialog()
{
    delete ui;
}

void FrameSwitchWindowCfgDialog::on_okPushButton_clicked()
{
    addNewWindowfunc();
}
