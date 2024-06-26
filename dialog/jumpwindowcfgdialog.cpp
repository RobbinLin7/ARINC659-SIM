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

JumpWindowCfgDialog::JumpWindowCfgDialog(uint id,const FrameWindow &framewindow, QWidget *parent) :
    WindowCfgDialog(parent),
    ui(new Ui::JumpWindowCfgDialog)
{
    ui->setupUi(this);
    window.setWindowType(FrameWindow::FRAME_JUMP);
    ui->windowId_lineEdit->setText(QString::number(id));
    ui->lineEdit_2->setText(QString::fromStdString(framewindow.getNewFrameID()));
    ui->checkBox->setChecked(framewindow.getFlag());

}

JumpWindowCfgDialog::~JumpWindowCfgDialog()
{
    delete ui;
}

void JumpWindowCfgDialog::on_okPushButton_clicked()
{
    window.setFlag(ui->checkBox->isChecked());//故有空闲
    window.setStrNewFrameID(ui->lineEdit_2->text().toStdString());//帧标识
    addNewWindowfunc();
}
