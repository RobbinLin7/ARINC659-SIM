#include "frameswitchwindowcfgdialog.h"
#include "ui_frameswitchwindowcfgdialog.h"

FrameSwitchWindowCfgDialog::FrameSwitchWindowCfgDialog(QWidget *parent) :
    WindowCfgDialog(parent),
    ui(new Ui::FrameSwitchWindowCfgDialog)
{
    ui->setupUi(this);
    window.setWindowType(FrameWindow::FRAME_SWITCH);
}

FrameSwitchWindowCfgDialog::~FrameSwitchWindowCfgDialog()
{
    delete ui;
}

void FrameSwitchWindowCfgDialog::on_okPushButton_clicked()
{
    addNewWindowfunc();
}
