#include "dataFrameCfgWidget.h"
#include "ui_dataFrameCfgWidget.h"

DataFrameCfgWidget::DataFrameCfgWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataFrameCfgWidget)
{
    ui->setupUi(this);
    ui->framePeriod_lineEdit->setText("30");
    ui->framePeriod_lineEdit->setStyleSheet("QLineEdit { border: 1px solid green; }");
    this->setWindowTitle(tr("帧配置"));
    installValidator();
    connect(ui->framePeriod_lineEdit, &QLineEdit::textChanged, this, &DataFrameCfgWidget::checkLineEditText);
    connect(ui->frameIdentification_lineEdit, &QLineEdit::textChanged, this, &DataFrameCfgWidget::checkLineEditText);
}

DataFrameCfgWidget::~DataFrameCfgWidget()
{
    delete ui;
}

void DataFrameCfgWidget::installValidator()
{
    framePeriodValidator = new QIntValidator(minFramePeriod, maxFramePeriod, this);
    ui->framePeriod_lineEdit->setValidator(framePeriodValidator);

    QRegExp regExp("[a-zA-Z][a-zA-Z0-9]*");

    frameIdentificationValidator = new QRegExpValidator(regExp, this);
    if(frameIdentificationValidator == nullptr) qDebug() << "wssb";
    ui->frameIdentification_lineEdit->setValidator(frameIdentificationValidator);
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
