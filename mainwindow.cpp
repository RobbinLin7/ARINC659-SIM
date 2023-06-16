#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "deviceModel/bodyFrameItem.h"
#include "newprojectdialog.h"

#include <QDebug>

#include <QFileDialog>
#include <QMessageBox>

#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new DeviceModelScene();
    ui->graphicsView->setScene(scene);
    //layout = new QVBoxLayout(this->ui->paraConfigWidget);
    bodyFrameNum = 0;


    this->initMainWindow();

    connect(scene, &DeviceModelScene::cfgBFSignal, [=](BodyFrameItem *selectedItem){
        qDebug() << "this: " << selectedItem->getBodyFrameID();
        this->myBodyFrameList.value(selectedItem->getBodyFrameID())->show();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::forTest()
{
    ui->statusbar->showMessage(tr("Position: (%1,%2)"));
}


//void MainWindow::on_actionNewBodyFrame_triggered()
//{
////    bodyFrameNum++;
////    BodyFrame *bodyFrame = new BodyFrame(this, ui->paraConfigWidget);
////    connect(bodyFrame, &BodyFrame::saveFrame, this, &MainWindow::saveFrame);
////    myBodyFrameList.insert(bodyFrameNum, bodyFrame);
////    bodyFrame->setBodyFrameID(bodyFrameNum);
////    bodyFrame->setWindowFlags(Qt::Dialog);
////    bodyFrame->setWindowModality(Qt::WindowModal);
////    bodyFrame->show();
////    //connect(bodyFrame, &BodyFrame::)
//}

//void MainWindow::saveFrame(){
//    BodyFrameItem *item = new BodyFrameItem(scene);

//    item->setPos(QPointF(10, 10));

//    item->setBodyFrameID(bodyFrameNum);

//    scene->addItem(item);

//    connect(item, &BodyFrameItem::cfgBodyFrame, this, [=](uint frameId){
//        if(!myBodyFrameList.contains(frameId)){
//            qDebug() << tr("该frameId不存在");
//            return;
//        }
//        else{
//            BodyFrame* bodyFrame = myBodyFrameList.value(frameId);

//            //layout->addWidget(bodyFrame);
//            bodyFrame->setWindowFlags(Qt::Dialog);
//            bodyFrame->setWindowModality(Qt::WindowModal);
//            bodyFrame->show();
//        }
//        qDebug() << "cfgBodyFrame";
//    });
//}

/**
 * @brief 初始化主窗口
 */
void MainWindow::initMainWindow()
{
    //设置名称
    this->setWindowTitle(tr("ARINC659配置工具"));

    //输出日志信息
    ui->textBrowser->append(tr("等待用户操作"));

    //qDebug() << ui->projectTreeWidget->width() << " " << ui->projectTreeWidget->height();

    ui->projectTreeWidget->setFixedWidth(150);

    //qDebug() << ui->projectTreeWidget->width() << " " << ui->projectTreeWidget->height();

    //ui->projectTreeWidget->setFixedSize(100, 20);
}

//void MainWindow::on_actionOpenMonitor_triggered()
//{
//    MonitorWidget *monitor = new MonitorWidget();

//    monitor->show();
//}

//void MainWindow::on_actionOpenCMDTable_triggered()
//{
//    QString filename = QFileDialog::getOpenFileName(this, QStringLiteral("打开命令表文件"), "./", QStringLiteral("(*txt)"));


//    if(filename.isEmpty())
//    {

//    }else
//    {

//    }
//}

//void MainWindow::on_actionStartSim_triggered()
//{
//    SimulinkWidget *simulinkWidget = new SimulinkWidget();


//    simulinkWidget->show();
//}

//void MainWindow::on_actionBurnToFPGA_triggered()
//{
//    QProcess process(this);
//    QString execIMPACT = "C:\\Xilinx\\14.7\\ISE_DS\\ISE\\bin\\nt64\\impact.exe";

//    if(process.startDetached(execIMPACT))
//    {
//        qDebug() << "on_actionBurnToFPGA_triggered() true!";
//    }else
//    {
//        QMessageBox::warning(this, tr("警告"), "启动IMPACT.exe失败,请检查是否存在!");
//    }
//}

//void MainWindow::on_actionNewProject_triggered()
//{
//    NewProjectDialog *newProj = new NewProjectDialog();

//    newProj->show();

//    connect(newProj, SIGNAL(sendProjInfo(QString, QString)), this, SLOT(addNewProjectSlot(QString, QString)));


//}

//void MainWindow::addNewProjectSlot(QString name, QString info)
//{

//    ui->treeWidget->setColumnCount(1);


//    //拿到项目名称后更新树中的内容

//    QStringList headerList;

//    headerList << tr("项目信息");

//    ui->treeWidget->setHeaderLabels(headerList);

//    QTreeWidgetItem *topItem = new QTreeWidgetItem();

//    topItem->setText(0, name);

//    ui->treeWidget->addTopLevelItem(topItem);

//    QTreeWidgetItem *item1 = new QTreeWidgetItem();
//    item1->setText(0, tr("机架配置"));

//    topItem->addChild(item1);

//    QTreeWidgetItem *item11 = new QTreeWidgetItem();
//    item11->setText(0, tr("机架1"));

//    item1->addChild(item11);

//    QTreeWidgetItem *item111 = new QTreeWidgetItem();
//    item111->setText(0, tr("模块1"));

//    item11->addChild(item111);


//    QTreeWidgetItem *item2 = new QTreeWidgetItem();
//    item2->setText(0, tr("输出文件"));

//    topItem->addChild(item2);

//    QTreeWidgetItem *item21 = new QTreeWidgetItem();
//    item21->setText(0, tr("命令表文件"));

//    item2->addChild(item21);

//    QTreeWidgetItem *item22 = new QTreeWidgetItem();
//    item22->setText(0, tr("编译结果"));

//    item2->addChild(item22);

//    QTreeWidgetItem *item23 = new QTreeWidgetItem();
//    item23->setText(0, tr("生成文件"));

//    item2->addChild(item23);

//    QTreeWidgetItem *item24 = new QTreeWidgetItem();
//    item24->setText(0, tr("烧录"));

//    item2->addChild(item24);

//    QTreeWidgetItem *item3 = new QTreeWidgetItem();
//    item3->setText(0, tr("仿真"));

//    topItem->addChild(item3);
//}
