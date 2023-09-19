#include "modulecfgwidget.h"
#include "ui_modulecfgwidget.h"
#include <QMessageBox>

ModuleCfgWidget::ModuleCfgWidget(const Module &module, QWidget *parent):
    QWidget(parent),
    ui(new Ui::ModuleCfgWidget)
{
    ui->setupUi(this);
    installValidator();
    this->module = module;
    setForm();
    connect(ui->moduleNumber_lineEdit, &QLineEdit::textChanged, this, &ModuleCfgWidget::checkLineEditTextSlot);
    connect(ui->initialWaitTime_lineEdit, &QLineEdit::textChanged, this, &ModuleCfgWidget::checkLineEditTextSlot);
    this->setWindowTitle(tr("模块配置"));
}

ModuleCfgWidget::ModuleCfgWidget(uint moduleNumber, QWidget *parent):
    QWidget(parent),
    ui(new Ui::ModuleCfgWidget)
{
    ui->setupUi(this);
    installValidator();
    ui->moduleNumber_lineEdit->setText(QString::number(moduleNumber));
    ui->moduleNumber_lineEdit->setEnabled(false);
    connect(ui->moduleNumber_lineEdit, &QLineEdit::textChanged, this, &ModuleCfgWidget::checkLineEditTextSlot);
    connect(ui->initialWaitTime_lineEdit, &QLineEdit::textChanged, this, &ModuleCfgWidget::checkLineEditTextSlot);
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

bool ModuleCfgWidget::check(QWidget *widget)
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

void ModuleCfgWidget::setForm()
{
    ui->moduleName_lineEdit->setText(QString::fromStdString(module.getModuleName()));
    ui->moduleNumber_lineEdit->setText(QString::number(module.getModuleNumber()));
    ui->moduleNumber_lineEdit->setEnabled(false);
    ui->initialWaitTime_lineEdit->setText(QString::number(module.getInitialWaitTime()));
    ui->moduleTypeComboBox->setCurrentIndex(module.getModuleType() == Module::physicalModule ? 0 : 1);
}

void ModuleCfgWidget::checkLineEditTextSlot()
{
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(sender());
    if(lineEdit == nullptr) return;
    bool flag = check(lineEdit);
    if(flag == true){
        lineEdit->setStyleSheet("QLineEdit { border: 1px solid green; }");
    }
    else{
        lineEdit->setStyleSheet("QLineEdit { border: 1px solid red; }");
    }
}

void ModuleCfgWidget::on_okPushButton_clicked(bool)
{
    module.setModuleName(ui->moduleName_lineEdit->text().toStdString());
    if(check(ui->moduleNumber_lineEdit) == false){
        QMessageBox::warning(this, "错误", "模块号参数有误");
        return;
    }
    module.setModuleNumber(ui->moduleNumber_lineEdit->text().toUInt());
    if(check(ui->initialWaitTime_lineEdit) == false){
        QMessageBox::warning(this, "错误", "初始等待时间参数有误");
        return;
    }
    module.setInitialWaitTime(ui->initialWaitTime_lineEdit->text().toUInt());
    module.setModuleType(ui->moduleTypeComboBox->currentIndex() == 0 ? Module::physicalModule : Module::simulationModule);
    emit saveModuleSignal(module);
    this->close();
}

void ModuleCfgWidget::on_cancelPushButton_clicked(bool)
{
    this->close();
}

