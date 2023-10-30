#ifndef LONGSYNCWINDOWCFGDIALOG_H
#define LONGSYNCWINDOWCFGDIALOG_H

#include "dialog/windowcfgdialog.h"
#include "data/bodyframe.h"
namespace Ui {
class LongSyncWindowCfgDialog;
}

class LongSyncWindowCfgDialog : public WindowCfgDialog
{
    Q_OBJECT

public:
    explicit LongSyncWindowCfgDialog(uint id, const std::map<uint, Module> &modules, QWidget *parent = nullptr);
    explicit LongSyncWindowCfgDialog(uint id,const FrameWindow &framework ,const std::map<uint, Module> &modules, QWidget *parent = nullptr);
    ~LongSyncWindowCfgDialog();

private:
    Ui::LongSyncWindowCfgDialog *ui;
    const std::map<uint, Module>& modules;

protected slots:
    void on_okPushButton_clicked();
};

#endif // LONGSYNCWINDOWCFGDIALOG_H
