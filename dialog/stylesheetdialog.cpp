#include "stylesheetdialog.h"
#include "ui_stylesheetdialog.h"

StyleSheetDialog::StyleSheetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StyleSheetDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(QString(tr("确定")));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(QString(tr("取消")));
    //获取所有资源文件
    QDir qssDir(":/resources/qss");
//    QResource
    if(qssDir.exists()){
         qssDir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
         QStringList filters;
         filters << "*.qss";
         qssDir.setNameFilters(filters);
         QStringList qssList = qssDir.entryList();
         for(QString qss : qssList){
             ui->comboBox->addItem(qss);
         }
    }
    else{
        qDebug() << "获取资源文件失败\n";
    }
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, [=](){
        emit(changeStyleSheetSignal(ui->comboBox->currentText()));
    });
}

StyleSheetDialog::~StyleSheetDialog()
{
    delete ui;
}
