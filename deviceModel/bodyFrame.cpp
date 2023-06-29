#include "bodyFrame.h"
#include "ui_bodyFrame.h"
#include <QDebug>

BodyFrame::BodyFrame(QWidget *parent, QWidget *paraConfigWidget) :
    QWidget(parent),
    ui(new Ui::BodyFrame)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("机架配置"));
    this->paraConfigWidget = paraConfigWidget;
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
        emit(saveFrameSignal());
        this->close();
    });
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->lineEdit->setStyleSheet("QLineEdit { border: 1px solid red; }");
    installEventFilter();
    installValidator();

}

BodyFrame::~BodyFrame()
{
    delete ui;
}


void BodyFrame::setBodyFrameID(const uint &id)
{
    this->myBodyFrameID = id;

    //ui->lineEdit->setText(QString("编辑机架%1").arg(this->myBodyFrameID));
}

void BodyFrame::connectOkButtonToUpdateSignal()
{
    disconnect(ui->buttonBox->button(QDialogButtonBox::Ok), 0, 0, 0);
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, [=](){
        ok = true;
        emit(updateFrameSignal());
        this->close();
    });
}

void BodyFrame::setStdTableHeader(QTableWidget *widget, const QStringList &headerList)
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

    //ui->tableWidget->resizeColumnsToContents();
    widget->horizontalHeader()->sectionResizeMode(QHeaderView::Interactive);
}


void BodyFrame::on_addModelBtn_clicked()
{
    HardwareModel *hm = new HardwareModel();

    hm->show();
}

void BodyFrame::on_newFrameBtn_clicked()
{
    DataFrame *frame = new DataFrame();

    frame->show();
}

void BodyFrame::closeEvent(QCloseEvent *e)
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

bool BodyFrame::eventFilter(QObject *watched, QEvent *event)
{
    //qDebug() << event->type();
    QString input1 = ui->lineEdit->text();
    int pos = 0;
    if(event->type() == QEvent::KeyRelease){
        if(validator->validate(input1, pos) == QValidator::Acceptable){

            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
            ui->lineEdit->setStyleSheet("QLineEdit { border: 1px solid green; }");
        }
    }
    return QWidget::eventFilter(watched, event);
}

void BodyFrame::installEventFilter()
{
    ui->lineEdit->installEventFilter(this);
    ui->lineEdit_2->installEventFilter(this);
}

void BodyFrame::installValidator()
{
    validator = new QIntValidator(this);
    ui->lineEdit->setValidator(validator);
}



