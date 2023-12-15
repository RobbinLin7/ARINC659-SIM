#include "versiondetectionwindowcfgdialog.h"
#include "ui_versiondetectionwindowcfgdialog.h"
#include<sstream>
#include<QMessageBox>

VersionDetectionWindowCfgDialog::VersionDetectionWindowCfgDialog(uint id, const std::map<uint, Module>& modules, QWidget *parent) :
    WindowCfgDialog(parent),
    modules(modules),
    ui(new Ui::VersionDetectionWindowCfgDialog)
{
    ui->setupUi(this);
    window.setWindowType(FrameWindow::VERSION_SEND);
    for(auto x : modules){
        ui->mainLRM_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM1_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM2_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM3_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        nameToId[x.second.getModuleName()] = x.second.getModuleNumber();
    }
    ui->windowId_lineEdit->setText(QString::number(id));
}

VersionDetectionWindowCfgDialog::VersionDetectionWindowCfgDialog(uint id,const FrameWindow &framewindow ,const std::map<uint, Module>& modules, QWidget *parent) :
    WindowCfgDialog(parent),
    modules(modules),
    ui(new Ui::VersionDetectionWindowCfgDialog)
{
    ui->setupUi(this);
    for(auto x : modules){
        ui->mainLRM_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM1_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM2_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM3_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        nameToId[x.second.getModuleName()] = x.second.getModuleNumber();
    }
    ui->windowId_lineEdit->setText(QString::number(id));

    ui->mainLRM_comboBox->setCurrentText(QString::number(framewindow.getMainLRM()));
    ui->backupLRM1_comboBox->setCurrentText(QString::fromStdString(framewindow.getSupportLRM1()));
    ui->backupLRM2_comboBox->setCurrentText(QString::fromStdString(framewindow.getSupportLRM2()));
    ui->backupLRM3_comboBox->setCurrentText(QString::fromStdString(framewindow.getSupportLRM3()));

    //存在多次点击编辑重复的问题TODO
    QStringList receiveLRMList;
    for(auto x : framewindow.getReceiveLRMList()){
        receiveLRMList.append(QString::fromStdString(modules.at(x).getModuleName()));
    }
    ui->lineEdit_2->setText(receiveLRMList.join(" "));
}

VersionDetectionWindowCfgDialog::~VersionDetectionWindowCfgDialog()
{
    delete ui;
}

void VersionDetectionWindowCfgDialog::on_okPushButton_clicked()
{
    window.setNumOfTimeSlot(37);
    window.setMainLRM(nameToId[ui->mainLRM_comboBox->currentText().toStdString()]);
    window.setSupportLRM1(ui->backupLRM1_comboBox->currentText().toStdString());
    window.setSupportLRM2(ui->backupLRM2_comboBox->currentText().toStdString());
    window.setSupportLRM3(ui->backupLRM3_comboBox->currentText().toStdString());
    window.setStrReceiveAddr(ui->lineEdit_3->text().toStdString());

    //存设置好的接受LRM值
    std::istringstream iss(ui->lineEdit_2->text().toStdString());
    std::string LRM_name;
    while(std::getline(iss, LRM_name, ' ')){
        if(nameToId.find(LRM_name) != nameToId.end()){
            window.addReceiveLRM(nameToId[LRM_name]);
        }
        else{
            QMessageBox::critical(this, "错误", QString("LRM%1不存在"));
            return;
        }
    }
    addNewWindowfunc();
}

void VersionDetectionWindowCfgDialog::on_pushButton_clicked()
{
    QString text = ui->lineEdit_2->text();
    text += text == "" ? ui->comboBox->currentText() : " " +  ui->comboBox->currentText();
    ui->lineEdit_2->setText(text);
}

void VersionDetectionWindowCfgDialog::on_pushButton_2_clicked()
{
    ui->lineEdit_2->clear();
    window.removeAllOfReceiveLRMList();
}
