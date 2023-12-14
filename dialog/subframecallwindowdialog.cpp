#include "subframecallwindowdialog.h"
#include "ui_subframecallwindowdialog.h"

SubFrameCallWindowDialog::SubFrameCallWindowDialog(uint id, QWidget *parent) :
    WindowCfgDialog(parent),
    ui(new Ui::SubFrameCallWindowDialog)
{
    ui->setupUi(this);
    window.setWindowType(FrameWindow::CALL_SUBFRAME);
    ui->windowId_lineEdit->setText(QString::number(id));
}

SubFrameCallWindowDialog::SubFrameCallWindowDialog(uint id,const FrameWindow &framewindow ,QWidget *parent) :
    WindowCfgDialog(parent),
    ui(new Ui::SubFrameCallWindowDialog)
{
    ui->setupUi(this);
    ui->windowId_lineEdit->setText(QString::number(id));
    ui->checkBox->setChecked(framewindow.getFlag());
    ui->spinBox->setValue(QString::fromStdString(framewindow.getNewFrameID()).toInt());
}

SubFrameCallWindowDialog::~SubFrameCallWindowDialog()
{
    delete ui;
}

void SubFrameCallWindowDialog::on_okPushButton_clicked()
{
    window.setStrNewFrameID(QString::number(ui->spinBox->value()).toStdString());
    window.setFlag(ui->checkBox->isChecked());
    addNewWindowfunc();
}
