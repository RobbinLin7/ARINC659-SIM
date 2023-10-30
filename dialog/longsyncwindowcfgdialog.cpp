#include "longsyncwindowcfgdialog.h"
#include "ui_longsyncwindowcfgdialog.h"

LongSyncWindowCfgDialog::LongSyncWindowCfgDialog(uint id,const std::map<uint, Module> &modules, QWidget *parent) :
    WindowCfgDialog(parent),
    modules(modules),
    ui(new Ui::LongSyncWindowCfgDialog)
{
    ui->setupUi(this);
    window.setWindowType(FrameWindow::LONG_SYNC);
    for(auto x : modules){
        ui->mainLRM_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM1_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM2_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM3_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
    }


    ui->windowId_lineEdit->setText(QString::number(id));
}

LongSyncWindowCfgDialog::LongSyncWindowCfgDialog(uint id,const FrameWindow &framewindow, const std::map<uint, Module> &modules, QWidget *parent) :
    WindowCfgDialog(parent),
    modules(modules),
    ui(new Ui::LongSyncWindowCfgDialog)
{
    ui->setupUi(this);

    for(auto x : modules){
        ui->mainLRM_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM1_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM2_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM3_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
    }


    ui->windowId_lineEdit->setText(QString::number(id));
    ui->mainLRM_comboBox->setCurrentText(QString::number(framewindow.getMainLRM()));
    ui->lineEdit_2->setText(QString::fromStdString(framewindow.getSyncCode()));
    ui->backupLRM1_comboBox->setCurrentText(QString::fromStdString(framewindow.getSupportLRM1()));
    ui->backupLRM2_comboBox->setCurrentText(QString::fromStdString(framewindow.getSupportLRM2()));
    ui->backupLRM3_comboBox->setCurrentText(QString::fromStdString(framewindow.getSupportLRM3()));
    ui->checkBox->setChecked(framewindow.getFlag());
}

LongSyncWindowCfgDialog::~LongSyncWindowCfgDialog()
{
    delete ui;
}

void LongSyncWindowCfgDialog::on_okPushButton_clicked()
{
    window.setMainLRM(ui->mainLRM_comboBox->currentText().toInt());
    window.setStrSyncCode(ui->lineEdit_2->text().toStdString());
    window.setSupportLRM1(ui->backupLRM1_comboBox->currentText().toStdString());
    window.setSupportLRM2(ui->backupLRM2_comboBox->currentText().toStdString());
    window.setSupportLRM3(ui->backupLRM3_comboBox->currentText().toStdString());
    window.setFlag(ui->checkBox->isChecked());



    addNewWindowfunc();
}
