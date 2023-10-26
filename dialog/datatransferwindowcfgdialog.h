#ifndef DATATRANSFERWINDOWCFGDIALOG_H
#define DATATRANSFERWINDOWCFGDIALOG_H

#include "dialog/windowcfgdialog.h"
#include "data/bodyframe.h"

namespace Ui {
class DataTransferWindowCfgDialog;
}

class DataTransferWindowCfgDialog : public WindowCfgDialog
{
    Q_OBJECT

public:
    explicit DataTransferWindowCfgDialog(uint id, const std::map<uint, Module>& modules, QWidget *parent = nullptr);
    explicit DataTransferWindowCfgDialog(uint id, const FrameWindow& frameWindow, const std::map<uint, Module>& modules, QWidget *parent = nullptr);
    ~DataTransferWindowCfgDialog();

private:
    Ui::DataTransferWindowCfgDialog *ui;
    const std::map<uint, Module>& modules;
    std::map<std::string, uint> nameToId;
    // WindowCfgDialog interface
protected slots:
    void on_okPushButton_clicked();
    void on_addReceiveLRM_pushButton_clicked();
    void on_resetReceiveLRM_pushButton_clicked();
};

#endif // DATATRANSFERWINDOWCFGDIALOG_H
