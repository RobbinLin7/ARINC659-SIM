#include "faultinjectdialog.h"
#include "ui_faultinjectdialog.h"

FaultInjectDialog::FaultInjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FaultInjectDialog)
{
    ui->setupUi(this);
}

FaultInjectDialog::~FaultInjectDialog()
{
    delete ui;
}

void FaultInjectDialog::on_okPushButton_clicked()
{
    emit faultInjectSignal(static_cast<ErrorType>(ui->faultType_comboBox->currentIndex()));
    this->close();
}
