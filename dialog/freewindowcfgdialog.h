#ifndef FREEWINDOWCFGDIALOG_H
#define FREEWINDOWCFGDIALOG_H

#include "dialog/windowcfgdialog.h"
namespace Ui {
class FreeWindowCfgDialog;
}

class FreeWindowCfgDialog : public WindowCfgDialog
{
    Q_OBJECT

public:
    explicit FreeWindowCfgDialog(uint id, QWidget *parent = nullptr);
    explicit FreeWindowCfgDialog(uint id,const FrameWindow &framewindow ,QWidget *parent = nullptr);
    ~FreeWindowCfgDialog();

private:
    Ui::FreeWindowCfgDialog *ui;
protected slots:
    void on_okPushButton_clicked();
};

#endif // FREEWINDOWCFGDIALOG_H
