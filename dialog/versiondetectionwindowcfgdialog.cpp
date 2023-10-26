#include "versiondetectionwindowcfgdialog.h"
#include "ui_versiondetectionwindowcfgdialog.h"

VersionDetectionWindowCfgDialog::VersionDetectionWindowCfgDialog(uint id, QWidget *parent):
    WindowCfgDialog(parent),
    ui(new Ui::VersionDetectionWindowCfgDialog)
{
    ui->setupUi(this);
    window.setWindowType(FrameWindow::VERSION_SEND);
    ui->windowId_lineEdit->setText(QString::number(id));
}

VersionDetectionWindowCfgDialog::~VersionDetectionWindowCfgDialog()
{
    delete ui;
}

void VersionDetectionWindowCfgDialog::on_okPushButton_clicked()
{
    addNewWindowfunc();
}
