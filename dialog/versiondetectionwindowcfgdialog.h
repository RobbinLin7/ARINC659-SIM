#ifndef VERSIONDETECTIONWINDOWCFGDIALOG_H
#define VERSIONDETECTIONWINDOWCFGDIALOG_H

#include "dialog/windowcfgdialog.h"
namespace Ui {
class VersionDetectionWindowCfgDialog;
}

class VersionDetectionWindowCfgDialog : public WindowCfgDialog
{
    Q_OBJECT

public:
    explicit VersionDetectionWindowCfgDialog(uint id, QWidget *parent = nullptr);
    ~VersionDetectionWindowCfgDialog();

private:
    Ui::VersionDetectionWindowCfgDialog *ui;
protected slots:
    void on_okPushButton_clicked();
};

#endif // VERSIONDETECTIONWINDOWCFGDIALOG_H
