#include "versiondetectionwindowcfgdialog.h"
#include "ui_versiondetectionwindowcfgdialog.h"

VersionDetectionWindowCfgDialog::VersionDetectionWindowCfgDialog(QWidget *parent) :
    WindowCfgDialog(parent),
    ui(new Ui::VersionDetectionWindowCfgDialog)
{
    ui->setupUi(this);
    window.setWindowType(FrameWindow::VERSION_SEND);
}

VersionDetectionWindowCfgDialog::~VersionDetectionWindowCfgDialog()
{
    delete ui;
}

void VersionDetectionWindowCfgDialog::on_okPushButton_clicked()
{
    addNewWindowfunc();
}
