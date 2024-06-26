#ifndef JUMPWINDOWCFGDIALOG_H
#define JUMPWINDOWCFGDIALOG_H

#include "dialog/windowcfgdialog.h"
namespace Ui {
class JumpWindowCfgDialog;
}

class JumpWindowCfgDialog : public WindowCfgDialog
{
    Q_OBJECT

public:
    explicit JumpWindowCfgDialog(uint id, QWidget *parent = nullptr);
    explicit JumpWindowCfgDialog(uint id,const FrameWindow &framewindow,QWidget *parent = nullptr);
    ~JumpWindowCfgDialog();

private:
    Ui::JumpWindowCfgDialog *ui;
protected slots:
    void on_okPushButton_clicked();
};

#endif // JUMPWINDOWCFGDIALOG_H
