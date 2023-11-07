#include "datatransferwindowcfgdialog.h"
#include "ui_datatransferwindowcfgdialog.h"
#include<sstream>
#include<QMessageBox>

DataTransferWindowCfgDialog::DataTransferWindowCfgDialog(uint id, const std::map<uint, Module>& modules, QWidget *parent) :
    WindowCfgDialog(parent),
    modules(modules),
    ui(new Ui::DataTransferWindowCfgDialog)
{
    ui->setupUi(this);
    window.setWindowType(FrameWindow::DATA_SEND);
    //ui->mainLRM_comboBox->set
    for(auto x : modules){
        ui->mainLRM_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->receiveLRM_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM1_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM2_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM3_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        nameToId[x.second.getModuleName()] = x.second.getModuleNumber();
    }
    ui->windowId_lineEdit->setText(QString::number(id));
}

DataTransferWindowCfgDialog::DataTransferWindowCfgDialog(uint id, const FrameWindow &frameWindow, const std::map<uint, Module> &modules, QWidget *parent):
    WindowCfgDialog(parent),
    modules(modules),
    ui(new Ui::DataTransferWindowCfgDialog)
{
    ui->setupUi(this);
    for(auto x : modules){
        ui->mainLRM_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->receiveLRM_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM1_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM2_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM3_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        nameToId[x.second.getModuleName()] = x.second.getModuleNumber();
    }
    ui->mainLRM_comboBox->setCurrentText(QString::fromStdString(modules.at(frameWindow.getMainLRM()).getModuleName()));
    ui->windowId_lineEdit->setText(QString::number(id));
    ui->backupLRM1_comboBox->setCurrentText(QString::fromStdString(frameWindow.getSupportLRM1()));
    QStringList receiveLRMList;
    for(auto x : frameWindow.getReceiveLRMList()){
        receiveLRMList.append(QString::fromStdString(modules.at(x).getModuleName()));
    }
    ui->receiveLRM_lineEdit->setText(receiveLRMList.join(" "));
}

DataTransferWindowCfgDialog::~DataTransferWindowCfgDialog()
{
    qDebug()<< "调用了析构函数";
    delete ui;
}

void DataTransferWindowCfgDialog::on_okPushButton_clicked()
{
    qDebug() << "DataTransferWindowCfgDialog";
    window.setMainLRM(nameToId[ui->mainLRM_comboBox->currentText().toStdString()]);
    window.setDataCount(ui->dataCount_lineEdit->text().toInt());
    window.setStrSendAddr(ui->sendAddr_lineEdit->text().toStdString());
    window.setStrReceiveAddr(ui->receiveAddr_lineEdit->text().toStdString());
    window.setSupportLRM1(ui->backupLRM1_comboBox->currentText().toStdString());
    window.setSupportLRM2(ui->backupLRM2_comboBox->currentText().toStdString());
    window.setSupportLRM3(ui->backupLRM3_comboBox->currentText().toStdString());

    //存设置好的接受LRM值
    std::istringstream iss(ui->receiveLRM_lineEdit->text().toStdString());
    std::string LRM_name;
    while(std::getline(iss, LRM_name, ' ')){
        qDebug() << QString::fromStdString(LRM_name);
        if(nameToId.find(LRM_name) != nameToId.end()&&!window.isExistInReceiveLRMList(nameToId[LRM_name])){//判断模块是否存在
            window.addReceiveLRM(nameToId[LRM_name]);
        }
        else{
            QMessageBox::critical(this, "错误", QString("LRM:%1不存在").arg(QString::fromStdString(LRM_name)));
            return;
        }
    }

    addNewWindowfunc();
}

void DataTransferWindowCfgDialog::on_addReceiveLRM_pushButton_clicked()
{
    QString text = ui->receiveLRM_lineEdit->text();
    text += text == "" ? ui->receiveLRM_comboBox->currentText() : " " +  ui->receiveLRM_comboBox->currentText();
    ui->receiveLRM_lineEdit->setText(text);
}

void DataTransferWindowCfgDialog::on_resetReceiveLRM_pushButton_clicked()
{
    ui->receiveLRM_lineEdit->clear();
    window.removeAllOfReceiveLRMList();
}
