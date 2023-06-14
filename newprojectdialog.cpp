#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"


NewProjectDialog::NewProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("新建"));

    ui->infoLabel->setText("");
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}

void NewProjectDialog::on_okBtn_clicked()
{
    if(ui->projNameEdit->text().isEmpty())
    {
        ui->infoLabel->setText("请输入项目名称!");
    }
    else
    {
        emit sendProjInfo(ui->projNameEdit->text(), ui->projInfoEdit->document()->toPlainText());


        this->close();
    }
}
