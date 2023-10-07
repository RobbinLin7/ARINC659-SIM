#ifndef DATATRANSFERWINDOWCFGDIALOG_H
#define DATATRANSFERWINDOWCFGDIALOG_H

#include <QDialog>

namespace Ui {
class DataTransferWindowCfgDialog;
}

class DataTransferWindowCfgDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataTransferWindowCfgDialog(QWidget *parent = nullptr);
    ~DataTransferWindowCfgDialog();

private:
    Ui::DataTransferWindowCfgDialog *ui;
};

#endif // DATATRANSFERWINDOWCFGDIALOG_H
