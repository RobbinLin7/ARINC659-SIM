#include "longsyncwindowcfgdialog.h"
#include "ui_longsyncwindowcfgdialog.h"
#include <QMessageBox>

LongSyncWindowCfgDialog::LongSyncWindowCfgDialog(uint id,const std::map<uint, Module> &modules, QWidget *parent) :
    WindowCfgDialog(parent),
    modules(modules),
    ui(new Ui::LongSyncWindowCfgDialog)
{
    ui->setupUi(this);
    installValidator();
    window.setWindowType(FrameWindow::LONG_SYNC);
    for(auto x : modules){
        ui->mainLRM_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM1_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM2_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM3_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
    }

    ui->windowId_lineEdit->setText(QString::number(id));
    connect(ui->syncCode_lineEdit,&QLineEdit::textChanged,this,&LongSyncWindowCfgDialog::dynamicSetLineEdit);
}

LongSyncWindowCfgDialog::LongSyncWindowCfgDialog(uint id,const FrameWindow &framewindow, const std::map<uint, Module> &modules, QWidget *parent) :
    WindowCfgDialog(parent),
    modules(modules),
    ui(new Ui::LongSyncWindowCfgDialog)
{
    ui->setupUi(this);
    installValidator();
    for(auto x : modules){
        ui->mainLRM_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM1_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM2_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->backupLRM3_comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
    }


    ui->windowId_lineEdit->setText(QString::number(id));
    ui->mainLRM_comboBox->setCurrentText(QString::number(framewindow.getMainLRM()));
    ui->syncCode_lineEdit->setText(QString::fromStdString(framewindow.getSyncCode()));
    ui->backupLRM1_comboBox->setCurrentText(QString::fromStdString(framewindow.getSupportLRM1()));
    ui->backupLRM2_comboBox->setCurrentText(QString::fromStdString(framewindow.getSupportLRM2()));
    ui->backupLRM3_comboBox->setCurrentText(QString::fromStdString(framewindow.getSupportLRM3()));
    ui->checkBox->setChecked(framewindow.getFlag());
    connect(ui->syncCode_lineEdit,&QLineEdit::textChanged,this,&LongSyncWindowCfgDialog::dynamicSetLineEdit);
}

LongSyncWindowCfgDialog::~LongSyncWindowCfgDialog()
{
    delete ui;
}

void LongSyncWindowCfgDialog::installValidator()
{
    QIntValidator* validator = new QIntValidator(1, 255, this);
    ui->syncCode_lineEdit->setValidator(validator);
}

void LongSyncWindowCfgDialog::dynamicSetLineEdit()
{
//    qDebug()<<"执行了该函数";
    int dim = 0;
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(sender());
    if(lineEdit==nullptr) return;
    QString input = lineEdit->text();
    if(input == ""){
        lineEdit->setStyleSheet("QLineEdit { border: 1px solid gray;}");
    }else if(lineEdit->validator()->validate(input,dim) == QValidator::Acceptable){
        lineEdit->setStyleSheet("QLineEdit { border: 1px solid green;}");
    }else{
        lineEdit->setStyleSheet("QLineEdit { border: 1px solid red;}");
    }
}

bool LongSyncWindowCfgDialog::check(QWidget *widget)
{
    int dummy = 0;
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(widget);
    if(lineEdit != nullptr){
        QString input = lineEdit->text();
        if(lineEdit->validator() == nullptr) return true;
        if(lineEdit->validator()->validate(input, dummy) == QValidator::Acceptable){
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

void LongSyncWindowCfgDialog::on_okPushButton_clicked()
{

    if(check(ui->syncCode_lineEdit) == false){
        QMessageBox::warning(this, "错误", "同步码参数超出范围");
        return;
    }
    window.setMainLRM(ui->mainLRM_comboBox->currentText().toInt());
    window.setStrSyncCode(ui->syncCode_lineEdit->text().toStdString());
    window.setSupportLRM1(ui->backupLRM1_comboBox->currentText().toStdString());
    window.setSupportLRM2(ui->backupLRM2_comboBox->currentText().toStdString());
    window.setSupportLRM3(ui->backupLRM3_comboBox->currentText().toStdString());
    window.setFlag(ui->checkBox->isChecked());



    addNewWindowfunc();
}

