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

    enum ErrorType{CLK_ERROR, D0_ERROR, D1_ERROR} errorType;

private:
    Ui::FaultInjectDialog *ui;

private slots:
    void on_okPushButton_clicked();

signals:
    void faultInjectSignal(ErrorType);
};

#endif // FAULTINJECTDIALOG_H
