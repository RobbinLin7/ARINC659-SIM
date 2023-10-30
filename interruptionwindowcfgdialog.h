#ifndef INTERRUPTIONWINDOWCFGDIALOG_H
#define INTERRUPTIONWINDOWCFGDIALOG_H

#include <QDialog>

namespace Ui {
class InterruptionWindowCfgDialog;
}

class InterruptionWindowCfgDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InterruptionWindowCfgDialog(QWidget *parent = nullptr);
    ~InterruptionWindowCfgDialog();

private:
    Ui::InterruptionWindowCfgDialog *ui;
};

#endif // INTERRUPTIONWINDOWCFGDIALOG_H
