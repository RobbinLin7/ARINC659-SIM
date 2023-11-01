#ifndef BURNTOFPGADIALOG_H
#define BURNTOFPGADIALOG_H

#include <QDialog>

namespace Ui {
class BurnToFPGADialog;
}

class BurnToFPGADialog : public QDialog
{
    Q_OBJECT

public:
    explicit BurnToFPGADialog(QWidget *parent = nullptr);
    ~BurnToFPGADialog();
    void addItem(QString);

private:
    Ui::BurnToFPGADialog *ui;
    size_t checkedDeviceCnt = 0;
private slots:
    void on_okPushButton_clicked();
    void on_cancelPushButton_clicked();
    void on_selectAll_checkBox_clicked();
signals:
    void configFinished(QStringList commandList);
};

#endif // BURNTOFPGADIALOG_H
