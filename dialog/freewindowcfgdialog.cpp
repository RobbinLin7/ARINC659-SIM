#include "freewindowcfgdialog.h"
#include "ui_freewindowcfgdialog.h"
#include <QIntValidator>
#include <QMessageBox>

FreeWindowCfgDialog::FreeWindowCfgDialog(uint id, QWidget *parent) :
    WindowCfgDialog(parent),
    ui(new Ui::FreeWindowCfgDialog)
{
    ui->setupUi(this);
    window.setWindowType(FrameWindow::FREE);
    installValidator();
    ui->windowId_lineEdit->setText(QString::number(id));
    connect(ui->numOfTimeSlot_lineEdit,&QLineEdit::textChanged,this,&FreeWindowCfgDialog::dynamicSetLineEdit);
}

FreeWindowCfgDialog::FreeWindowCfgDialog(uint id,const FrameWindow &framewindow ,QWidget *parent) :
    WindowCfgDialog(parent),
    ui(new Ui::FreeWindowCfgDialog)
{
    ui->setupUi(this);
    installValidator();
    ui->windowId_lineEdit->setText(QString::number(id));
    ui->numOfTimeSlot_lineEdit->setText(QString::number(framewindow.getSendTimeScale()));
}

FreeWindowCfgDialog::~FreeWindowCfgDialog()
{
    delete ui;
}

void FreeWindowCfgDialog::installValidator()
{
    QIntValidator* validtor = new QIntValidator(0,20000,this);
    ui->numOfTimeSlot_lineEdit->setValidator(validtor);
}

bool FreeWindowCfgDialog::check(QWidget *qWidget)
{
    int dim = 0;
    QLineEdit* checkingLineEdit = dynamic_cast<QLineEdit*>(qWidget);
    if(checkingLineEdit != nullptr){
        QString inPut = checkingLineEdit->text();
        if(checkingLineEdit->validator() == nullptr) return false;
        if(checkingLineEdit->validator()->validate(inPut,dim) == QValidator::Acceptable ) return true;
        else {
            return  false;
        }
    }
    return false;

}

void FreeWindowCfgDialog::on_okPushButton_clicked()
{
    if(check(ui->numOfTimeSlot_lineEdit) == false){
       QMessageBox::warning(this, "错误", "空闲等待时限参数超出范围");
       return;
    }
    window.setNumOfTimeSlot(ui->numOfTimeSlot_lineEdit->text().toInt());
    addNewWindowfunc();
}

void FreeWindowCfgDialog::dynamicSetLineEdit()
{
    int dim = 0;
    QLineEdit* targetLineEdit = dynamic_cast<QLineEdit*>(sender());
    if(targetLineEdit != nullptr){
        QString inPut = targetLineEdit->text();
        if(targetLineEdit->validator() == nullptr) return;
        else{
            if(inPut == "") targetLineEdit->setStyleSheet("QLineEdit { border: 1px solid gray;}");
            else if(targetLineEdit->validator()->validate(inPut,dim) == QValidator::Acceptable)
                targetLineEdit->setStyleSheet("QLineEdit { border: 1px solid green;}");
            else targetLineEdit->setStyleSheet("QLineEdit { border: 1px solid red;}");
        }
    }
    return;
}
