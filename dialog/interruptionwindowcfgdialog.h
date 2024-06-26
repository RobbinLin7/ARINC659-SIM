#ifndef INTERRUPTIONWINDOWCFGDIALOG_H
#define INTERRUPTIONWINDOWCFGDIALOG_H

#include "dialog/windowcfgdialog.h"
namespace Ui {
class InterruptionWindowCfgDialog;
}

class InterruptionWindowCfgDialog : public WindowCfgDialog
{
    Q_OBJECT

public:
    explicit InterruptionWindowCfgDialog(uint id, QWidget *parent = nullptr);
    explicit InterruptionWindowCfgDialog(uint id,const FrameWindow &framewindow ,QWidget *parent = nullptr);
    ~InterruptionWindowCfgDialog();

private:
    Ui::InterruptionWindowCfgDialog *ui;
protected slots:
    void on_okPushButton_clicked();
};

#endif // INTERRUPTIONWINDOWCFGDIALOG_H
