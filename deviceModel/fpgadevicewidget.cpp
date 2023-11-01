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


void FPGADeviceWidget::on_selectConfigFile_pushButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("选择上传文件"), "*.mcs");
    ui->filePath_lineEdit->setText(path);
}

void FPGADeviceWidget::on_select_radioButton_clicked()
{
    emit radioButton_clicked(ui->select_radioButton->isChecked());
}
