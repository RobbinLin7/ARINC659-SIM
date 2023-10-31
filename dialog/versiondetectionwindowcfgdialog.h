#ifndef VERSIONDETECTIONWINDOWCFGDIALOG_H
#define VERSIONDETECTIONWINDOWCFGDIALOG_H

#include "dialog/windowcfgdialog.h"
#include "data/bodyframe.h"

namespace Ui {
class VersionDetectionWindowCfgDialog;
}

class VersionDetectionWindowCfgDialog : public WindowCfgDialog
{
    Q_OBJECT

public:
    explicit VersionDetectionWindowCfgDialog(uint id,const std::map<uint, Module>& modules,QWidget *parent = nullptr);
    explicit VersionDetectionWindowCfgDialog(uint id,const FrameWindow &framewindow,const std::map<uint, Module>& modules,QWidget *parent = nullptr);
    ~VersionDetectionWindowCfgDialog();

private:
    Ui::VersionDetectionWindowCfgDialog *ui;
    const std::map<uint, Module>& modules;
    std::map<std::string, uint> nameToId;

protected slots:
    void on_okPushButton_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // VERSIONDETECTIONWINDOWCFGDIALOG_H
