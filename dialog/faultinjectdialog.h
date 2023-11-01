#ifndef FAULTINJECTDIALOG_H
#define FAULTINJECTDIALOG_H

#include <QDialog>

namespace Ui {
class FaultInjectDialog;
}

class FaultInjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FaultInjectDialog(QWidget *parent = nullptr);
    ~FaultInjectDialog();

private:
    Ui::FaultInjectDialog *ui;
};

#endif // FAULTINJECTDIALOG_H
