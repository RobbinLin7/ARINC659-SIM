#include "designinputwidget.h"
#include "ui_designinputwidget.h"


DesignInputWidget::DesignInputWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DesignInputWidget)
{
    ui->setupUi(this);
    QStringList headers = {QString(tr("模块号")), QString(tr("名称")), QString(tr("初始化等待时间"))};
    ui->modulesListWidget->setColumnCount(3);
    ui->modulesListWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->modulesListWidget->setHorizontalHeaderLabels(headers);

    ui->frameInfoListWidget->setColumnCount(5);
    ui->modulesListWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    headers = QStringList{QString(tr("帧标识")), QString(tr("窗口数")), QString(tr("帧周期")), QString(tr("空闲方式")), QString(tr("子帧"))};
    ui->frameInfoListWidget->setHorizontalHeaderLabels(headers);
    //ui->tableWidget->insertRow(0);
}

DesignInputWidget::~DesignInputWidget()
{
    delete ui;
}
