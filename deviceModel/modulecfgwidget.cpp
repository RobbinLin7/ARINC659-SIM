#include "modulecfgwidget.h"
#include "ui_modulecfgwidget.h"

ModuleCfgWidget::ModuleCfgWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModuleCfgWidget)
{
    ui->setupUi(this);

    installValidator();

    connect(ui->moduleNumber_lineEdit, &QLineEdit::textChanged, this, &ModuleCfgWidget::checkLineEditText);
    connect(ui->initialWaitTime_lineEdit, &QLineEdit::textChanged, this, &ModuleCfgWidget::checkLineEditText);

    this->setWindowTitle(tr("模块配置"));
}

ModuleCfgWidget::~ModuleCfgWidget()
{
    delete ui;
}

void ModuleCfgWidget::installValidator()
{
    moduleNumberValidator = new QIntValidator(minModuleNumber, maxModuleNumber, this);
    ui->moduleNumber_lineEdit->setValidator(moduleNumberValidator);

    QRegExp hexRegExp("(0x)?[0-9A-Fa-f]+");
    initialWaitTimeValidator = new QRegExpValidator(hexRegExp, this);
    ui->initialWaitTime_lineEdit->setValidator(initialWaitTimeValidator);
}

void ModuleCfgWidget::checkLineEditText()
{
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(sender());
    if(lineEdit == nullptr) return;
    QString input = lineEdit->text();
    if(input == ""){
        lineEdit->setStyleSheet("QLineEdit { border: 1px solid gray;}");
    }
    else if(lineEdit->validator()->validate(input, dummy) == QValidator::Acceptable){
        lineEdit->setStyleSheet("QLineEdit { border: 1px solid green; }");
    }
    else{
        lineEdit->setStyleSheet("QLineEdit { border: 1px solid red; }");
    }
}
