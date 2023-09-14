#include "dataFrameCfgWidget.h"
#include "ui_dataFrameCfgWidget.h"
#include <QMessageBox>

DataFrameCfgWidget::DataFrameCfgWidget(const std::map<std::string, DataFrame>& dataframes, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataFrameCfgWidget),
    dataframes(dataframes),
    frameIdentificationValidator(QRegExp("[a-zA-Z][a-zA-Z0-9]*"), dataframes)
{
    ui->setupUi(this);
    ui->radioButton->setChecked(true);
    ui->framePeriod_lineEdit->setText("30");
    ui->framePeriod_lineEdit->setStyleSheet("QLineEdit { border: 1px solid green; }");
    this->setWindowTitle(tr("帧配置"));
    installValidator();
    connect(ui->framePeriod_lineEdit, &QLineEdit::textChanged, this, &DataFrameCfgWidget::checkLineEditText);
    connect(ui->frameIdentification_lineEdit, &QLineEdit::textChanged, this, &DataFrameCfgWidget::checkLineEditText);
    //ui->label_6->setText("你好呀");
}

DataFrameCfgWidget::~DataFrameCfgWidget()
{
    delete ui;
}

void DataFrameCfgWidget::installValidator()
{
    framePeriodValidator = new QIntValidator(minFramePeriod, maxFramePeriod, this);
    ui->framePeriod_lineEdit->setValidator(framePeriodValidator);
    ui->frameIdentification_lineEdit->setValidator(&frameIdentificationValidator);
}

bool DataFrameCfgWidget::check(QWidget *widget)
{
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(widget);
    if(lineEdit != nullptr){
        QString input = lineEdit->text();
        if(lineEdit->validator()->validate(input, dummy) == QValidator::Acceptable){
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

void DataFrameCfgWidget::checkLineEditText()
{
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(sender());
    if(lineEdit == nullptr) return;
    QString input = lineEdit->text();
    if(input == ""){
        lineEdit->setStyleSheet("QLineEdit { border: 1px solid gray;}");
    }
    else if(lineEdit->validator() == nullptr){
        qDebug() << "hhhhhh";
    }
    else if(lineEdit->validator()->validate(input, dummy) == QValidator::Acceptable){
        lineEdit->setStyleSheet("QLineEdit { border: 1px solid green; }");
    }
    else{
        lineEdit->setStyleSheet("QLineEdit { border: 1px solid red; }");
    }
}

void DataFrameCfgWidget::on_okPushButton_clicked(bool)
{
    if(check(ui->framePeriod_lineEdit) == false){
        QMessageBox::warning(this, "错误", "帧周期参数有误");
        return;
    }
    else if(check(ui->frameIdentification_lineEdit) == false){
        QMessageBox::warning(this, "错误", "模标识参数有误");
        return;
    }
    dataFrame.setFramePeriod(ui->framePeriod_lineEdit->text().toUInt());
    dataFrame.setFrameIdentification(ui->frameIdentification_lineEdit->text().toStdString());
    dataFrame.setIdleWaitTime(ui->idleWaitTime_lineEdit->text().toUInt());
    dataFrame.setTotalWindow(ui->totalWindow_lineEdit->text().toUInt());
    dataFrame.setTimeAllocationType(ui->radioButton->isChecked() ? DataFrame::equalAlloc : DataFrame::downConcentrationAlloc);
    emit saveDataFrameSignal(dataFrame);
    this->close();
}

void DataFrameCfgWidget::on_cancelPushButton_clicked(bool)
{
    this->close();
}
