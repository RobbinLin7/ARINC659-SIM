#include "burntofpgadialog.h"
#include "ui_burntofpgadialog.h"
#include "deviceModel/fpgadevicewidget.h"
#include <QMessageBox>

BurnToFPGADialog::BurnToFPGADialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BurnToFPGADialog)
{
    ui->setupUi(this);
    ui->okPushButton->setEnabled(false);
}

BurnToFPGADialog::~BurnToFPGADialog()
{
    delete ui;
}

void BurnToFPGADialog::addItem(QString manufactureName)
{
    FPGADeviceWidget* deviceInfo = new FPGADeviceWidget(manufactureName, this);
    QListWidgetItem *widgetItem = new QListWidgetItem();
    widgetItem->setSizeHint(deviceInfo->sizeHint());
    connect(deviceInfo, &FPGADeviceWidget::radioButton_clicked, this, [=](bool checked){
        if(checked) ++checkedDeviceCnt;
        else --checkedDeviceCnt, ui->selectAll_checkBox->setChecked(false);
        if(checkedDeviceCnt > 0){
            if(checkedDeviceCnt == (size_t)ui->listWidget->count()){
                ui->selectAll_checkBox->setChecked(true);
            }
            ui->okPushButton->setEnabled(true);
        }
        else{
            ui->okPushButton->setEnabled(false);
        }
    });
    ui->listWidget->addItem(widgetItem);
    ui->listWidget->setItemWidget(widgetItem, deviceInfo);
}

void BurnToFPGADialog::on_okPushButton_clicked()
{
    QStringList commandList;
    for(int i = 0; i < ui->listWidget->count(); ++i){
        QListWidgetItem* item = ui->listWidget->item(i);
        FPGADeviceWidget* customWidget = qobject_cast<FPGADeviceWidget*>(ui->listWidget->itemWidget(item));
        if(customWidget != nullptr){
            if(customWidget->getChecked()){
                if(customWidget->getConfigFilePath() == ""){
                    QMessageBox::warning(this, "错误", QString("%1未选择配置文件").arg(customWidget->getManuFacturerId()));
                    return;
                }
                QString command("assignFile -p %1 -file \"%2\"\n");
                commandList.append(command.arg(i + 1).arg(customWidget->getConfigFilePath()));
                commandList.append(QString("setAttribute -position %1 -attr readnextdevice -value \"(null)\"\n").arg(i + 1));
            }
        }
    }
    for(int i = 0; i < ui->listWidget->count(); ++i){
        QListWidgetItem* item = ui->listWidget->item(i);
        FPGADeviceWidget* customWidget = qobject_cast<FPGADeviceWidget*>(ui->listWidget->itemWidget(item));
        if(customWidget != nullptr){
            if(customWidget->getChecked()){
                QString command("Program -p %1 -e");
                command = command.arg(i + 1);
                if(customWidget->getVerifyChecked()) command.append(" -v");
                if(customWidget->getParallelChecked()) command.append(" -parallel");
                command.append("\n");
                commandList.append(command);
            }
        }
    }
    emit configFinished(commandList);
    this->close();
}

void BurnToFPGADialog::on_cancelPushButton_clicked()
{
    this->close();
}

void BurnToFPGADialog::on_selectAll_checkBox_clicked()
{
    for(int i = 0; i < ui->listWidget->count(); ++i){
        QListWidgetItem* item = ui->listWidget->item(i);
        FPGADeviceWidget* customWidget = qobject_cast<FPGADeviceWidget*>(ui->listWidget->itemWidget(item));
        if(customWidget != nullptr){
            customWidget->clickSelectRadioButton();
        }
    }
}
