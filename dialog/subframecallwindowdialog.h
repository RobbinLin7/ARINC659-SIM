#ifndef SUBFRAMECALLWINDOWDIALOG_H
#define SUBFRAMECALLWINDOWDIALOG_H

#include "dialog/windowcfgdialog.h"
namespace Ui {
class SubFrameCallWindowDialog;
}

class SubFrameCallWindowDialog : public WindowCfgDialog
{
    Q_OBJECT

public:
    explicit SubFrameCallWindowDialog(uint id, QWidget *parent = nullptr);
    ~SubFrameCallWindowDialog();

private:
    Ui::SubFrameCallWindowDialog *ui;
protected slots:
    void on_okPushButton_clicked();
};

#endif // SUBFRAMECALLWINDOWDIALOG_H
