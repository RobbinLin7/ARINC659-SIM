#ifndef LONGSYNCWINDOWCFGDIALOG_H
#define LONGSYNCWINDOWCFGDIALOG_H

#include "dialog/windowcfgdialog.h"
namespace Ui {
class LongSyncWindowCfgDialog;
}

class LongSyncWindowCfgDialog : public WindowCfgDialog
{
    Q_OBJECT

public:
    explicit LongSyncWindowCfgDialog(uint id, QWidget *parent = nullptr);
    ~LongSyncWindowCfgDialog();

private:
    Ui::LongSyncWindowCfgDialog *ui;
protected slots:
    void on_okPushButton_clicked();
};

#endif // LONGSYNCWINDOWCFGDIALOG_H
