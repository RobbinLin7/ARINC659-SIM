#ifndef FRAMESWITCHWINDOWCFGDIALOG_H
#define FRAMESWITCHWINDOWCFGDIALOG_H

#include "dialog/windowcfgdialog.h"
namespace Ui {
class FrameSwitchWindowCfgDialog;
}

class FrameSwitchWindowCfgDialog : public WindowCfgDialog
{
    Q_OBJECT

public:
    explicit FrameSwitchWindowCfgDialog(uint id, QWidget *parent = nullptr);
    ~FrameSwitchWindowCfgDialog();

private:
    Ui::FrameSwitchWindowCfgDialog *ui;

    // WindowCfgDialog interface
protected slots:
    void on_okPushButton_clicked();
};

#endif // FRAMESWITCHWINDOWCFGDIALOG_H
