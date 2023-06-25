﻿#include "mainwindow.h"
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
    ui->paraConfigWidget->hide();
    scene = new DeviceModelScene();
    ui->graphicsView->setScene(scene);
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


void MainWindow::on_actionNewBodyFrame_triggered()
{
    bodyFrameNum++;
    BodyFrame *bodyFrame = new BodyFrame(this);
    connect(bodyFrame, &BodyFrame::saveFrameSignal, this, &MainWindow::saveFrameSlot);
    connect(bodyFrame, &BodyFrame::updateFrameSignal, this, &MainWindow::updateFrameSlot);
    myBodyFrameList.insert(bodyFrameNum, bodyFrame);
    bodyFrame->setBodyFrameID(bodyFrameNum);
    bodyFrame->setWindowFlags(Qt::Dialog);
    //bodyFrame->setWindowModality(Qt::WindowModal);
    bodyFrame->setWindowModality(Qt::WindowModal);
    bodyFrame->show();
}
/**
 * @brief MainWindow::saveFrame
 */
void MainWindow::saveFrameSlot(){
    BodyFrameItem *item = new BodyFrameItem(bodyFrameNum, scene);

    item->setPos(QPointF(10, 10));

    item->setBodyFrameID(bodyFrameNum);

    scene->addItem(item);

    connect(item, &BodyFrameItem::cfgBodyFrameItemSignal, this, &MainWindow::cfgBodyFrameItemSlot);

}

void MainWindow::updateFrameSlot()
{
    qDebug() << "update success";
}

/**
 * @brief MainWindow::changeStyleSheetSlot
 * @param styleSheet 样式名
 *
 */
void MainWindow::changeStyleSheetSlot(QString styleSheet)
{
    QFile qssFile(":/resources/qss/" + styleSheet);
    if(qssFile.open(QFile::ReadOnly)){
        qApp->setStyleSheet(qssFile.readAll());
        qssFile.close();
    }
    else{
        qDebug() << "hahaha";
    }
}

void MainWindow::cfgBodyFrameItemSlot(uint frameId)
{
    if(!myBodyFrameList.contains(frameId)){
        qDebug() << tr("该frameId不存在");
        return;
    }
    else{
        BodyFrame* bodyFrame = myBodyFrameList.value(frameId);
        bodyFrame->connectOkButtonToUpdateSignal();
        bodyFrame->setParent(ui->paraConfigWidget);
        bodyFrame->setMinimumHeight(501);
        ui->paraConfigLayout->addWidget(bodyFrame);
        ui->paraConfigWidget->show();
        bodyFrame->show();
    }
}

void MainWindow::on_actionChangeStyleSheet_triggered()
{
    StyleSheetDialog styleSheetDialog(this);
    //styleSheetDialog.setWindowFlags(Qt::Dialog);
    styleSheetDialog.setWindowModality(Qt::WindowModal);
    connect(&styleSheetDialog, &StyleSheetDialog::changeStyleSheetSignal, this, &MainWindow::changeStyleSheetSlot);
    styleSheetDialog.exec();
}

/**
 * @brief 初始化主窗口
 */
void MainWindow::initMainWindow()
{
    //设置名称
    this->setWindowTitle(tr("ARINC659配置工具"));

    //输出日志信息
    ui->logTextBrowser->append(tr("等待用户操作"));

    //qDebug() << ui->projectTreeWidget->width() << " " << ui->projectTreeWidget->height();

    ui->projectTreeWidget->setFixedWidth(150);

    //qDebug() << ui->projectTreeWidget->width() << " " << ui->projectTreeWidget->height();

    //ui->projectTreeWidget->setFixedSize(100, 20);

    //connect(ui->actionOpenProject, &QAction::triggered, this, )
}

void MainWindow::on_actionOpenMonitor_triggered()
{
    MonitorWidget *monitor = new MonitorWidget();

    monitor->show();
}

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
/**
 * @brief MainWindow::on_actionNewProject_triggered
 */
void MainWindow::on_actionNewProject_triggered()
{
    NewProjectDialog *newProj = new NewProjectDialog();

    newProj->show();

    connect(newProj, SIGNAL(sendProjInfo(QString, QString)), this, SLOT(addNewProjectSlot(QString, QString)));

}
/**
 * @brief MainWindow::on_actionOpenProject_triggered
 */

void MainWindow::on_actionOpenProject_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, QString(tr("打开项目文件")), QString(), QString("*.proj659"));
    if(filePath == ""){
        addLogToDockWidget(QString(tr("打开项目失败")));
        QMessageBox::critical(this, QString(tr("错误")), QString(tr("打开项目失败")));
    }
    else{
        if(myXml.loadXmlFile(filePath)){
            Proj659& project = myXml.getProject659(); //可以通过xml获取project信息
            project.setDescription("this is a new description");
            qDebug() << project.getName() << project.getDescription();

        }
        else{
            addLogToDockWidget(QString(tr("项目解析失败")));
            QMessageBox::critical(this, QString(tr("错误")), QString(tr("项目解析失败")));
        }
    }
}
/**
 * @brief MainWindow::addNewProjectSlot
 * @param name
 * @param info
 */
void MainWindow::addNewProjectSlot(QString name, QString info)
{

    ui->projectTreeWidget->setColumnCount(1);


    //拿到项目名称后更新树中的内容

    QStringList headerList;

    headerList << tr("项目信息");

    ui->projectTreeWidget->setHeaderLabels(headerList);

    QTreeWidgetItem *topItem = new QTreeWidgetItem();

    topItem->setText(0, name);

    ui->projectTreeWidget->addTopLevelItem(topItem);

    QTreeWidgetItem *item1 = new QTreeWidgetItem();
    item1->setText(0, tr("机架配置"));

    topItem->addChild(item1);

    QTreeWidgetItem *item11 = new QTreeWidgetItem();
    item11->setText(0, tr("机架1"));

    item1->addChild(item11);

    QTreeWidgetItem *item111 = new QTreeWidgetItem();
    item111->setText(0, tr("模块1"));

    item11->addChild(item111);


    QTreeWidgetItem *item2 = new QTreeWidgetItem();
    item2->setText(0, tr("输出文件"));

    topItem->addChild(item2);

    QTreeWidgetItem *item21 = new QTreeWidgetItem();
    item21->setText(0, tr("命令表文件"));

    item2->addChild(item21);

    QTreeWidgetItem *item22 = new QTreeWidgetItem();
    item22->setText(0, tr("编译结果"));

    item2->addChild(item22);

    QTreeWidgetItem *item23 = new QTreeWidgetItem();
    item23->setText(0, tr("生成文件"));

    item2->addChild(item23);

    QTreeWidgetItem *item24 = new QTreeWidgetItem();
    item24->setText(0, tr("烧录"));

    item2->addChild(item24);

    QTreeWidgetItem *item3 = new QTreeWidgetItem();
    item3->setText(0, tr("仿真"));

    topItem->addChild(item3);
}

void MainWindow::addLogToDockWidget(const QString log)
{
    QString currentTime;
    currentTime= QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->logTextBrowser->append(currentTime + log);
}
