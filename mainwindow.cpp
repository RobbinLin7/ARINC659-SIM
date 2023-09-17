#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "deviceModel/bodyFrameGraphicsItem.h"
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
    ui->projectTreeWidget->clear();
    QStringList headerList;
    headerList << tr("项目信息");
    ui->projectTreeWidget->setHeaderLabels(headerList);
    bodyFrameNum = 0;
    test = new QAction("关闭项目", ui->projectTreeWidget);
    this->initMainWindow();
    disableAllActionNeedAProject();
    connect(ui->projectTreeWidget, &QTreeWidget::itemPressed, this, &MainWindow::onProjectItemPressed);
    connect(ui->projectTreeWidget, &QTreeWidget::itemDoubleClicked, this, &MainWindow::onProjectItemDoubleClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::forTest()
{
    ui->statusbar->showMessage(tr("Position: (%1,%2)"));
}


void MainWindow::on_actionNewBodyFrameItem_triggered()
{
    //bodyFrameNum++;
    bodyFrame = std::make_shared<BodyFrameCfgWidget>(currentProject->getMinUnusedId(), this);
    //BodyFrame *bodyFrame = new BodyFrame(this);
    connect(bodyFrame.get(), &BodyFrameCfgWidget::saveBodyFrameItemSignal, this, &MainWindow::saveBodyFrameItemSlot);
    connect(bodyFrame.get(), &BodyFrameCfgWidget::updateBodyFrameItemSignal, this, &MainWindow::updateBodyFrameSlot);
    //myBodyFrameList.insert(bodyFrameNum, bodyFrame);
    bodyFrame->setBodyFrameID(bodyFrameNum);
    bodyFrame->setWindowFlags(Qt::Dialog);
    //bodyFrame->setWindowModality(Qt::WindowModal);
    bodyFrame->setWindowModality(Qt::WindowModal);
    bodyFrame->show();
}
/**
 * @brief MainWindow::saveFrame
 */
void MainWindow::saveBodyFrameItemSlot(const BodyFrameItem& bodyFrameItem){
    currentProject->addBodyFrameItem(bodyFrameItem);
    std::shared_ptr<BodyFrameGraphicsItem> graphicsItem = std::shared_ptr<BodyFrameGraphicsItem>(new BodyFrameGraphicsItem(bodyFrameItem, this));
    bodyFrameGraphicsItems.insert(bodyFrameItem.getBodyFrameItemID(), graphicsItem);
    if(scene->addBodyFrameItem(graphicsItem) == false){
        qDebug() << "scene addbodyframeitem failed";
        return;
    }
    currentBodyFrameList.insert(bodyFrameItem.getBodyFrameItemID(), bodyFrame);
    connect(graphicsItem.get(), &BodyFrameGraphicsItem::cfgBodyFrameItemSignal, this, &MainWindow::cfgBodyFrameItemSlot);
    connect(graphicsItem.get(), &BodyFrameGraphicsItem::deleteBodyFrameItemSignal, this, &MainWindow::deleteBodyFrameItemSlot);
    auto p = ui->projectTreeWidget->findItems("机架配置", Qt::MatchContains | Qt::MatchRecursive);
    for(auto &x : p){
        QTreeWidgetItem *treeWidgetItem = new QTreeWidgetItem(x);
        treeWidgetItem->setText(0, QString("机架%1").arg(bodyFrameItem.getBodyFrameItemID()));
        graphicsItem->setTreeWidgetItem(treeWidgetItem);
        x->setExpanded(true);
    }
}


void MainWindow::updateBodyFrameSlot(const BodyFrameItem& bodyFrameItem)
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
    qDebug() << "该frameId为" << frameId;
    if(!currentBodyFrameList.contains(frameId)){
        qDebug() << tr("该frameId不存在");
        return;
    }
    else{
        std::shared_ptr<BodyFrameCfgWidget> bodyFrameCfgWiget = currentBodyFrameList.value(frameId);
        BodyFrameCfgWidget* widget = new BodyFrameCfgWidget(*bodyFrameCfgWiget.get());
        widget->setParent(ui->paraConfigWidget);
        ui->paraConfigLayout->addWidget(widget);
        ui->paraConfigWidget->show();
        widget->show();
//        bodyFrameCfgWiget->setParent(ui->paraConfigWidget);
//        ui->paraConfigLayout->addWidget(bodyFrameCfgWiget.get());
//        ui->paraConfigWidget->show();
//        bodyFrameCfgWiget->show();
//        disconnect(bodyFrameCfgWiget.get());
//        bodyFrame->connectOkButtonToUpdateSignal();
//        bodyFrame->setParent(ui->paraConfigWidget);
//        bodyFrame->setMinimumHeight(501);
//        ui->paraConfigLayout->addWidget(bodyFrame.get());
//        ui->paraConfigWidget->show();
//        bodyFrame->show();
    }
}

void MainWindow::deleteBodyFrameItemSlot(uint id)
{
    currentProject->deleteBodyFrameItem(id);
    bodyFrameGraphicsItems.remove(id);
    scene->update();
    //bodyFrameGraphicsItems.value(id);
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
    ui->logTextBrowser->append(tr("开始"));

    //qDebug() << ui->projectTreeWidget->width() << " " << ui->projectTreeWidget->height();

    qDebug() << ui->projectTreeWidget->sizeHint().width() << ui->projectTreeWidget->sizeHint().height();
    qDebug() << ui->projectTreeWidget->width() << ui->projectTreeWidget->height();
    ui->projectTreeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);



    //ui->projectTreeWidget->setFixedWidth(150);

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

    if((currentProject = std::shared_ptr<Proj659>(new Proj659(name, info))) != nullptr){
        enableAllActionNeedAProject();
    }

    ui->projectTreeWidget->setColumnCount(1);

    //拿到项目名称后更新树中的内容

    QTreeWidgetItem *topItem = new QTreeWidgetItem();

    topItem->setText(0, QString("项目\"%1\"").arg(name));

    ui->projectTreeWidget->addTopLevelItem(topItem);

    QTreeWidgetItem *item1 = new QTreeWidgetItem();

    item1->setText(0, tr("机架配置"));

    topItem->addChild(item1);

//    QTreeWidgetItem *item11 = new QTreeWidgetItem();

//    item11->setText(0, tr("机架1"));

//    item1->addChild(item11);

//    QTreeWidgetItem *item111 = new QTreeWidgetItem();

//    item111->setText(0, tr("模块1"));

//    item11->addChild(item111);

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

void MainWindow::disableAllActionNeedAProject()
{
    this->ui->menuDeviceManagement->setEnabled(false);
    this->ui->menuCMDTableManagement->setEnabled(false);
    this->ui->menuMonitor->setEnabled(false);
    this->ui->menuSimulink->setEnabled(false);
}

void MainWindow::enableAllActionNeedAProject()
{
    this->ui->menuDeviceManagement->setEnabled(true);
    this->ui->menuCMDTableManagement->setEnabled(true);
    this->ui->menuMonitor->setEnabled(true);
    this->ui->menuSimulink->setEnabled(true);
}


void MainWindow::onProjectItemPressed(QTreeWidgetItem *item, int column)
{
    //if(item->parent() != nullptr) return;
    QString str = item->text(column);
    QRegularExpression regex("机架(\\d+)"); // 匹配"机架"后面的数字串
    QRegularExpressionMatch match = regex.match(str);

    if(str == "机架配置"){
        if(qApp->mouseButtons() == Qt::RightButton){
            QMenu *menu = new QMenu(ui->projectTreeWidget);
            menu->addAction(ui->actionNewBodyFrameItem);
            menu->exec(QCursor::pos());
        }
    }
    else if(str.startsWith("项目")){
        if(qApp->mouseButtons() == Qt::RightButton){
            QAction* closeAction = new QAction(QString("关闭%1").arg(item->text(column)), ui->projectTreeWidget);
            QAction* deleteAction = new QAction(QString("删除%1").arg(item->text(column)), ui->projectTreeWidget);
            QMenu *menu = new QMenu(ui->projectTreeWidget);
            connect(closeAction, &QAction::triggered, this, [=](){
                ui->projectTreeWidget->removeItemWidget(item, column);
                delete item;
                currentProject = nullptr;
                disableAllActionNeedAProject();
                currentBodyFrameList.clear();
            });
            connect(deleteAction, &QAction::triggered, this, [=](){
                ui->projectTreeWidget->removeItemWidget(item, column);
                delete item;
                currentProject->setSave(false);
                currentProject = nullptr;
                currentBodyFrameList.clear();
                disableAllActionNeedAProject();
            });
            menu->addAction(closeAction);
            menu->addAction(deleteAction);
            menu->exec(QCursor::pos());
        }
    }
    else if(match.hasMatch()){
        if(qApp->mouseButtons() == Qt::RightButton){
            uint bodyFrameItemId = match.captured(1).toUInt();
            QAction* cfgAction = new QAction(QString("配置机架%1").arg(bodyFrameItemId), ui->projectTreeWidget);
            QAction* deleteAction = new QAction(QString("删除机架%1").arg(bodyFrameItemId), ui->projectTreeWidget);
            connect(cfgAction, &QAction::triggered, this, [=](){
                cfgBodyFrameItemSlot(bodyFrameItemId);
            });
            connect(deleteAction, &QAction::triggered, this, [=](){
                this->deleteBodyFrameItemSlot(bodyFrameItemId);
            });
            QMenu *menu = new QMenu(ui->projectTreeWidget);
            menu->addAction(cfgAction);
            menu->addAction(deleteAction);
            menu->exec(QCursor::pos());
        }
    }
}

void MainWindow::onProjectItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    qDebug() << "double clicked";

}
