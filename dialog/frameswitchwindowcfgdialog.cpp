#include "frameswitchwindowcfgdialog.h"
#include "ui_frameswitchwindowcfgdialog.h"
#include <QMessageBox>

FrameSwitchWindowCfgDialog::FrameSwitchWindowCfgDialog(uint id, const std::map<uint, Module>& modules,QWidget *parent) :
    WindowCfgDialog(parent),
    modules(modules),
    ui(new Ui::FrameSwitchWindowCfgDialog)
{
    ui->setupUi(this);
    window.setWindowType(FrameWindow::FRAME_SWITCH);
    installValidator();
    for(auto x : modules){
        ui->comboBox->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->comboBox_2->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->comboBox_3->addItem(QString::fromStdString(x.second.getModuleName()));
        ui->comboBox_4->addItem(QString::fromStdString(x.second.getModuleName()));
        nameToId[x.second.getModuleName()] = x.second.getModuleNumber();
    }
    ui->windowId_lineEdit->setText(QString::number(id));
    connect(ui->syncCode_LineEdit,&QLineEdit::textChanged,this,&FrameSwitchWindowCfgDialog::dynamicSetLineEdit);
}

FrameSwitchWindowCfgDialog::FrameSwitchWindowCfgDialog(uint id,const FrameWindow &framewindow,const std::map<uint, Module>& modules,QWidget *parent) :
    WindowCfgDialog(parent),
    modules(modules),
    ui(new Ui::FrameSwitchWindowCfgDialog)
{
    ui->setupUi(this);
    installValidator();
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
    ui->syncCode_LineEdit->setText(QString::fromStdString(framewindow.getSyncCode()));
    ui->checkBox->setChecked(framewindow.getFlag());
    ui->spinBox_5->setValue(QString::fromStdString(framewindow.getNewFrameID()).toInt());
    connect(ui->syncCode_LineEdit,&QLineEdit::textChanged,this,&FrameSwitchWindowCfgDialog::dynamicSetLineEdit);


}


FrameSwitchWindowCfgDialog::~FrameSwitchWindowCfgDialog()
{
    delete ui;
}

void FrameSwitchWindowCfgDialog::on_okPushButton_clicked()
{
    if(check(ui->syncCode_LineEdit) == false){
        QMessageBox::warning(this, "错误", "同步码参数超出范围");
        return;
    }
    window.setMainLRM(nameToId[ui->comboBox->currentText().toStdString()]);
    window.setSupportLRM1(ui->comboBox_2->currentText().toStdString());
    window.setSupportLRM2(ui->comboBox_3->currentText().toStdString());
    window.setSupportLRM3(ui->comboBox_4->currentText().toStdString());
    window.setStrSyncCode(ui->syncCode_LineEdit->text().toStdString());
    window.setFlag(ui->checkBox->isChecked());
    window.setStrNewFrameID(QString::number(ui->spinBox_5->value()).toStdString());
    window.setNumOfTimeSlot(184);
    addNewWindowfunc();
}

void FrameSwitchWindowCfgDialog::installValidator()
{
    QIntValidator* intValidator = new QIntValidator(1,255,this);
    ui->syncCode_LineEdit->setValidator(intValidator);
}

bool FrameSwitchWindowCfgDialog::check(QWidget *widget)
{
    int dim = 0;
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(widget);
    if(lineEdit != nullptr){
        QString inPut = lineEdit->text();
        if(lineEdit->validator() == nullptr) return false;
        if(lineEdit->validator()->validate(inPut,dim) == QValidator::Acceptable){
            return true;
        }else {
            return  false;
        }
    }
    return false;

}

void FrameSwitchWindowCfgDialog::dynamicSetLineEdit()
{
    int dim = 0;
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(sender());
    if(lineEdit == nullptr) return;
    QString inPut = lineEdit->text();
    if(inPut == ""){
        lineEdit->setStyleSheet("QLineEdit { border: 1px solid gray;}");
    }
    else if(lineEdit->validator()->validate(inPut,dim) == QValidator::Acceptable){
        lineEdit->setStyleSheet("QLineEdit { border: 1px solid green;}");
    }else{
        lineEdit->setStyleSheet("QLineEdit { border: 1px solid red;}");
    }
}
