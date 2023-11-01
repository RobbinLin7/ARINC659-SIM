#ifndef FPGADEVICEWIDGET_H
#define FPGADEVICEWIDGET_H

#include <QWidget>

namespace Ui {
class FPGADeviceWidget;
}

class FPGADeviceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FPGADeviceWidget(QString manufacturerId, QWidget *parent = nullptr);
    ~FPGADeviceWidget();
    bool getChecked()const;
    void clickSelectRadioButton();
    bool getVerifyChecked()const;
    bool getParallelChecked()const;
    QString getConfigFilePath()const;
    QString getManuFacturerId()const;
private:
    Ui::FPGADeviceWidget *ui;

private slots:
    void on_selectConfigFile_pushButton_clicked();
    void on_select_radioButton_clicked();

signals:
    void radioButton_clicked(bool);
};

#endif // FPGADEVICEWIDGET_H
