#ifndef FRAMESWITCHWINDOWCFGDIALOG_H
#define FRAMESWITCHWINDOWCFGDIALOG_H

#include "dialog/windowcfgdialog.h"
#include "data/bodyframe.h"

namespace Ui {
class FrameSwitchWindowCfgDialog;
}

class FrameSwitchWindowCfgDialog : public WindowCfgDialog
{
    Q_OBJECT

public:
    explicit FrameSwitchWindowCfgDialog(uint id,const std::map<uint, Module>& modules,QWidget *parent = nullptr);
    explicit FrameSwitchWindowCfgDialog(uint id,const FrameWindow &framewindow,const std::map<uint, Module>& modules,QWidget *parent = nullptr);
    ~FrameSwitchWindowCfgDialog();

private:
    Ui::FrameSwitchWindowCfgDialog *ui;
    const std::map<uint, Module>& modules;
    std::map<std::string, uint> nameToId;

    // WindowCfgDialog interface
protected slots:
    void on_okPushButton_clicked();
    void installValidator();
    bool check(QWidget* widget);
    void dynamicSetLineEdit();
};

#endif // FRAMESWITCHWINDOWCFGDIALOG_H
