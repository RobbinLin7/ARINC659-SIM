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

InterruptionWindowCfgDialog::InterruptionWindowCfgDialog(uint id,const FrameWindow& framewindow,QWidget *parent) :
    WindowCfgDialog(parent),
    ui(new Ui::InterruptionWindowCfgDialog)
{
    ui->setupUi(this);
    //window.setWindowType(FrameWindow::INT_SEND);
    ui->windowId_lineEdit->setText(QString::number(id));

    ui->lineEdit_3->setText(QString::number(framewindow.getIntNum1()));
    ui->lineEdit_2->setText(QString::number(framewindow.getIntNum2()));
}

InterruptionWindowCfgDialog::~InterruptionWindowCfgDialog()
{
    delete ui;
}

void InterruptionWindowCfgDialog::on_okPushButton_clicked()
{
    window.setNumOfTimeSlot(0);
    window.setIntNum1(ui->lineEdit_3->text().toInt());//发送中断号
    window.setIntNum2(ui->lineEdit_2->text().toInt());//接受中断号
    addNewWindowfunc();
}
