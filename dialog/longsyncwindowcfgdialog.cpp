#include "longsyncwindowcfgdialog.h"
#include "ui_longsyncwindowcfgdialog.h"

LongSyncWindowCfgDialog::LongSyncWindowCfgDialog(uint id, QWidget *parent) :
    WindowCfgDialog(parent),
    ui(new Ui::LongSyncWindowCfgDialog)
{
    ui->setupUi(this);
    window.setWindowType(FrameWindow::LONG_SYNC);
    ui->windowId_lineEdit->setText(QString::number(id));
}

LongSyncWindowCfgDialog::~LongSyncWindowCfgDialog()
{
    delete ui;
}

void LongSyncWindowCfgDialog::on_okPushButton_clicked()
{
    addNewWindowfunc();
}
