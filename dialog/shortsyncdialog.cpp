#include "shortsyncdialog.h"
#include "ui_shortsyncdialog.h"

ShortSyncDialog::ShortSyncDialog(uint id, QWidget *parent) :
    WindowCfgDialog(parent),
    ui(new Ui::ShortSyncDialog)
{
    window.setWindowId(id);
    window.setWindowType(FrameWindow::SHORT_SYNC);
    ui->setupUi(this);
    ui->windowId_lineEdit->setText(QString::number(id));
}

ShortSyncDialog::~ShortSyncDialog()
{
    delete ui;
}

void ShortSyncDialog::on_okPushButton_clicked()
{
    addNewWindowfunc();
}
