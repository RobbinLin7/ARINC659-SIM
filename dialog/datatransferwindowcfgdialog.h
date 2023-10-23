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
    explicit DataTransferWindowCfgDialog(const BodyFrame& dataFrame, QWidget *parent = nullptr);
    ~DataTransferWindowCfgDialog();

private:
    Ui::DataTransferWindowCfgDialog *ui;

    // WindowCfgDialog interface
protected slots:
    void on_okPushButton_clicked();
};

#endif // DATATRANSFERWINDOWCFGDIALOG_H
