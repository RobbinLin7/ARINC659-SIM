#include "bodyFrameCfgWidget.h"
#include "ui_bodyFrameCfgWidget.h"
#include <QDebug>

BodyFrameCfgWidget::BodyFrameCfgWidget(uint frameId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BodyFrameCfgWidget)
{
    ui->setupUi(this);
    ui->id_lineEdit->setText(QString::number(frameId));
    ui->id_lineEdit->setEnabled(false);
    this->setWindowTitle(tr("机架配置"));
//    this->paraConfigWidget = paraConfigWidget;
    QStringList headerList;
    headerList << tr("模块号") << tr("名称") << tr("初始化等待时间");
    setStdTableHeader(ui->modelTableWidget, headerList);

    headerList.clear();
    headerList << tr("帧标识") << tr("窗口数") << tr("帧周期") << tr("空闲方式") << tr("子帧");
    setStdTableHeader(ui->frameTableWidget, headerList);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(QString(tr("确认")));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(QString(tr("取消")));
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, [=](){
        this->close();
    });
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, [=](){
        ok = true;
        updateBodyFrameItem();
        emit(saveFrameItemSignal(bodyFrameItem));
        this->close();
    });
    //ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
   // ui->id_lineEdit->setStyleSheet("QLineEdit { border: 1px solid red; }");
    installEventFilter();
    installValidator();

}

BodyFrameCfgWidget::~BodyFrameCfgWidget()
{
    delete ui;
}


void BodyFrameCfgWidget::setBodyFrameID(const uint &id)
{
    this->myBodyFrameID = id;

    //ui->lineEdit->setText(QString("编辑机架%1").arg(this->myBodyFrameID));
}

void BodyFrameCfgWidget::connectOkButtonToUpdateSignal()
{
    disconnect(ui->buttonBox->button(QDialogButtonBox::Ok), 0, 0, 0);
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, [=](){
        ok = true;
        emit(updateFrameSignal());
        this->close();
    });
}

void BodyFrameCfgWidget::setStdTableHeader(QTableWidget *widget, const QStringList &headerList)
{
    //QStringList headerList;
    QTableWidgetItem *headerItem;
    //headerList << tr("模块号") << tr("名称") << tr("初始化等待时间");
    //ui->tableWidget

    widget->setColumnCount(headerList.count());//列数设置为与 headerText的行数相等
    for (int i = 0; i < widget->columnCount(); i++)//列编号从0开始
    {
        //cellItem=ui->tableInfo->horizontalHeaderItem(i);
        headerItem=new QTableWidgetItem(headerList.at(i)); //新建一个QTableWidgetItem， headerText.at(i)获取headerText的i行字符串
        QFont font=headerItem->font();//获取原有字体设置
        font.setBold(true);//设置为粗体
        font.setPointSize(12);//字体大小
        //headerItem->setTextColor(Qt::black);//字体颜色
        headerItem->setForeground(Qt::black);
        headerItem->setFont(font);//设置字体
        widget->setHorizontalHeaderItem(i, headerItem); //设置表头单元格的Item
    }
    //ui->tableWidget->setItemDelegateForColumn(colScore,&spinDelegate);//设置自定义代理组件

    widget->resizeColumnsToContents();
    widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    widget->horizontalHeader()->sectionResizeMode(QHeaderView::Interactive);
}


void BodyFrameCfgWidget::on_addModelBtn_clicked()
{
    ModuleCfgWidget *hm = new ModuleCfgWidget();

    hm->show();
}

void BodyFrameCfgWidget::on_newFrameBtn_clicked()
{
    DataFrameCfgWidget *frame = new DataFrameCfgWidget();

    frame->show();
}

void BodyFrameCfgWidget::closeEvent(QCloseEvent *e)
{
    if(ok){
        e->accept();
        return;
    }
    bool exit = QMessageBox::question(this,
                                  tr("退出"),
                                  tr("确定要放弃新建机架吗?"),
                                  QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::No) == QMessageBox::Yes;
    if(exit){
        e->accept();
    }
    else{
        e->ignore();
    }
}

bool BodyFrameCfgWidget::eventFilter(QObject *watched, QEvent *event)
{
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(watched);
    if(lineEdit != nullptr && (event->type() == QEvent::KeyRelease)){
        dynamicSetLineEdit(lineEdit);
    }
    return QWidget::eventFilter(watched, event);
}

void BodyFrameCfgWidget::installEventFilter()
{
    ui->id_lineEdit->installEventFilter(this);
    ui->arbitrationStepDuration_lineEdit->installEventFilter(this);
    ui->timeCalibrationFactor_lineEdit->installEventFilter(this);
    ui->messageInterval_lineEdit->installEventFilter(this);
    ui->majorVersionNumber_lineEdit->installEventFilter(this);
    ui->subVersionNumber_lineEdit->installEventFilter(this);
}

void BodyFrameCfgWidget::installValidator()
{
    idValidator = new QIntValidator(this);
    idValidator->setRange(0, maxFrameId);
    ui->id_lineEdit->setValidator(idValidator);

    arbitrationStepDurationValidator = new QIntValidator(this);
    arbitrationStepDurationValidator->setRange(minArbitrationStepDuration, maxArbitrationStepDuration);
    ui->arbitrationStepDuration_lineEdit->setValidator(arbitrationStepDurationValidator);

    timeCalibrationFactorValidator = new QIntValidator(this);
    timeCalibrationFactorValidator->setRange(minTimeCalibrationFactor, maxTimeCalibrationFactor);
    ui->timeCalibrationFactor_lineEdit->setValidator(timeCalibrationFactorValidator);

    messageIntervalValidator = new QIntValidator(this);
    messageIntervalValidator->setRange(minMessageInterval, maxMessageInterval);
    ui->messageInterval_lineEdit->setValidator(messageIntervalValidator);

    QRegExp hexRegExp("[0-9A-Fa-f]+");
    majorAndSubVersionNumerValidator = new QRegExpValidator(hexRegExp, this);
    ui->majorVersionNumber_lineEdit->setValidator(majorAndSubVersionNumerValidator);
    ui->subVersionNumber_lineEdit->setValidator(majorAndSubVersionNumerValidator);
}

void BodyFrameCfgWidget::updateBodyFrameItem()
{
    bodyFrameItem.setArbitrationStepDuration(ui->arbitrationStepDuration_lineEdit->text().toUInt());
    bodyFrameItem.setTimeCalibrationFactor(ui->timeCalibrationFactor_lineEdit->text().toUInt());
    bodyFrameItem.setMajorVersionNumber(ui->majorVersionNumber_lineEdit->text().toUInt());
    bodyFrameItem.setSubVersionNumber(ui->subVersionNumber_lineEdit->text().toUInt());
    bodyFrameItem.setMessageInterval(ui->messageInterval_lineEdit->text().toUInt());
}

void BodyFrameCfgWidget::dynamicSetLineEdit(QLineEdit *lineEdit)
{
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



