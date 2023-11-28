#include "fpgadevicewidget.h"
#include "ui_fpgadevicewidget.h"
#include <QListWidgetItem>
#include <QFileDialog>

FPGADeviceWidget::FPGADeviceWidget(QString manufacturerId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FPGADeviceWidget)
{
    ui->setupUi(this);
    ui->select_radioButton->setText(manufacturerId);
    disableAll();
}

FPGADeviceWidget::~FPGADeviceWidget()
{
    delete ui;
}

bool FPGADeviceWidget::getChecked() const
{
    return ui->select_radioButton->isChecked();
}

void FPGADeviceWidget::clickSelectRadioButton()
{
    ui->select_radioButton->click();
}

bool FPGADeviceWidget::getVerifyChecked() const
{
    return ui->verify_checkBox->isChecked();
}

bool FPGADeviceWidget::getParallelChecked() const
{
    return ui->parallel_checkBox->isChecked();
}

QString FPGADeviceWidget::getConfigFilePath() const
{
    return ui->filePath_lineEdit->text();
}

QString FPGADeviceWidget::getManuFacturerId() const
{
    return ui->select_radioButton->text();
}

void FPGADeviceWidget::setEnabled(bool enabled)
{
    ui->filePath_lineEdit->setEnabled(enabled);
    ui->selectConfigFile_pushButton->setEnabled(enabled);
    ui->verify_checkBox->setEnabled(enabled);
    ui->parallel_checkBox->setEnabled(enabled);
}

void FPGADeviceWidget::enableAll()
{
    this->setEnabled(true);
}

void FPGADeviceWidget::disableAll()
{
    this->setEnabled(false);
}


void FPGADeviceWidget::on_selectConfigFile_pushButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("选择上传文件"), "*.mcs");
    ui->filePath_lineEdit->setText(path);
}

void FPGADeviceWidget::on_select_radioButton_clicked()
{
    if(ui->select_radioButton->isChecked()){
        enableAll();
    }
    else{
        disableAll();
    }
    emit radioButton_clicked(ui->select_radioButton->isChecked());
}
