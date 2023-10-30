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

FreeWindowCfgDialog::FreeWindowCfgDialog(uint id,const FrameWindow &framewindow ,QWidget *parent) :
    WindowCfgDialog(parent),
    ui(new Ui::FreeWindowCfgDialog)
{
    ui->setupUi(this);
    ui->windowId_lineEdit->setText(QString::number(id));
    ui->lineEdit_2->setText(QString::number(framewindow.getSendTimeScale()));
}

FreeWindowCfgDialog::~FreeWindowCfgDialog()
{
    delete ui;
}

void FreeWindowCfgDialog::on_okPushButton_clicked()
{
    window.setSendTimeScale(ui->lineEdit_2->text().toInt());
    addNewWindowfunc();
}
