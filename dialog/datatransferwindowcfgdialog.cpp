#include "datatransferwindowcfgdialog.h"
#include "ui_datatransferwindowcfgdialog.h"

DataTransferWindowCfgDialog::DataTransferWindowCfgDialog(const BodyFrame& bodyFrame, QWidget *parent) :
    WindowCfgDialog(parent),
    ui(new Ui::DataTransferWindowCfgDialog)
{
    ui->setupUi(this);
    window.setWindowType(FrameWindow::DATA_SEND);
    //ui->mainLRM_comboBox->set
    for(auto x : bodyFrame.getModules()){
        ui->mainLRM_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->receiveLRM_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM1_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM2_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM3_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
    }
}

DataTransferWindowCfgDialog::~DataTransferWindowCfgDialog()
{
    delete ui;
}

void DataTransferWindowCfgDialog::on_okPushButton_clicked()
{
    qDebug() << "DataTransferWindowCfgDialog";
    addNewWindowfunc();
}
