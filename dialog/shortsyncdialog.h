#ifndef SHORTSYNCDIALOG_H
#define SHORTSYNCDIALOG_H

#include <QDialog>
#include "data/framewindow.h"
#include "dialog/WindowCfgDialog.h"

namespace Ui {
class ShortSyncDialog;
}

class ShortSyncDialog : public WindowCfgDialog
{
    Q_OBJECT

public:
    explicit ShortSyncDialog(uint id, QWidget *parent = nullptr);
    ~ShortSyncDialog();

private:
    Ui::ShortSyncDialog *ui;

private slots:
    void on_okPushButton_clicked();
};

#endif // SHORTSYNCDIALOG_H
