#include "frameswitchwindowcfgdialog.h"
#include "ui_frameswitchwindowcfgdialog.h"

FrameSwitchWindowCfgDialog::FrameSwitchWindowCfgDialog(uint id, const std::map<uint, Module>& modules,QWidget *parent) :
    WindowCfgDialog(parent),
    modules(modules),
    ui(new Ui::FrameSwitchWindowCfgDialog)
{
    ui->setupUi(this);
    window.setWindowType(FrameWindow::FRAME_SWITCH);
    for(auto x : modules){
        ui->comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->comboBox_2->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->comboBox_3->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->comboBox_4->addItem(QString::fromStdString(x.second.getModuleName()));
        nameToId[x.second.getModuleName()] = x.second.getModuleNumber();
    }
    ui->windowId_lineEdit->setText(QString::number(id));
}

FrameSwitchWindowCfgDialog::FrameSwitchWindowCfgDialog(uint id,const FrameWindow &framewindow,const std::map<uint, Module>& modules,QWidget *parent) :
    WindowCfgDialog(parent),
    modules(modules),
    ui(new Ui::FrameSwitchWindowCfgDialog)
{
    ui->setupUi(this);
    for(auto x : modules){
        ui->comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->comboBox_2->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->comboBox_3->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->comboBox_4->addItem(QString::fromStdString(x.second.getModuleName()));
    }
    ui->windowId_lineEdit->setText(QString::number(id));

    ui->comboBox->setCurrentText(QString::number(framewindow.getMainLRM()));
    ui->comboBox_2->setCurrentText(QString::fromStdString(framewindow.getSupportLRM1()));
    ui->comboBox_3->setCurrentText(QString::fromStdString(framewindow.getSupportLRM2()));
    ui->comboBox_4->setCurrentText(QString::fromStdString(framewindow.getSupportLRM3()));
    ui->lineEdit_2->setText(QString::fromStdString(framewindow.getSyncCode()));
    ui->checkBox->setChecked(framewindow.getFlag());
    ui->spinBox_5->setValue(QString::fromStdString(framewindow.getNewFrameID()).toInt());



}


FrameSwitchWindowCfgDialog::~FrameSwitchWindowCfgDialog()
{
    delete ui;
}

void FrameSwitchWindowCfgDialog::on_okPushButton_clicked()
{
    window.setMainLRM(nameToId[ui->comboBox->currentText().toStdString()]);
    window.setSupportLRM1(ui->comboBox_2->currentText().toStdString());
    window.setSupportLRM2(ui->comboBox_3->currentText().toStdString());
    window.setSupportLRM3(ui->comboBox_4->currentText().toStdString());
    window.setStrSyncCode(ui->lineEdit_2->text().toStdString());
    window.setFlag(ui->checkBox->isChecked());
    window.setStrNewFrameID(QString::number(ui->spinBox_5->value()).toStdString());
    addNewWindowfunc();
}
