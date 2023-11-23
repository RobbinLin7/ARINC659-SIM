#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "deviceModel/bodyFrameGraphicsItem.h"
#include "newprojectdialog.h"
#include "dialog/burntofpgadialog.h"
#include "deviceModel/innerbodyframescene.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->paraConfigWidget->hide();
    ui->projectTreeWidget->clear();
    QStringList headerList;
    headerList << tr("项目信息");
    //ui->graphicsView->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
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
    bodyFrameCfgWidget = std::make_shared<BodyFrameCfgWidget>(currentProject->getMinUnusedId(), this);
    connect(bodyFrameCfgWidget.get(), &BodyFrameCfgWidget::saveBodyFrameItemSignal, this, &MainWindow::saveBodyFrameItemSlot);
    bodyFrameCfgWidget->setBodyFrameID(bodyFrameNum);
    bodyFrameCfgWidget->setWindowFlags(Qt::Dialog);
    bodyFrameCfgWidget->setWindowModality(Qt::WindowModal);
    bodyFrameCfgWidget->show();
}
/**
 * @brief MainWindow::saveFrame
 */
void MainWindow::saveBodyFrameItemSlot(const BodyFrame& bodyFrameItem){
    currentProject->addBodyFrameItem(bodyFrameItem);
    std::shared_ptr<BodyFrameGraphicsItem> graphicsItem = std::shared_ptr<BodyFrameGraphicsItem>(new BodyFrameGraphicsItem(scene->getAx(),
                                                                                                                           scene->getAy(),
                                                                                                                           scene->getBx(),
                                                                                                                           scene->getBy(),
                                                                                                                       bodyFrameItem));
//    connect(graphicsItem.get(), &BodyFrameGraphicsItem::enterInBodyFrame, this, [=](uint id){
//       ui->graphicsView->setScene(new InnerBodyFrameScene(bodyFrameItem,this));
//    });
    connect(graphicsItem.get(), &BodyFrameGraphicsItem::enterInBodyFrame, this, &MainWindow::createNewInnerBodyFrameScene);
    bodyFrameGraphicsItems.insert(bodyFrameItem.getBodyFrameItemID(), graphicsItem);
    if(scene->addBodyFrameItem(graphicsItem) == false){
        qDebug() << "scene addbodyframeitem failed";
        return;
    }
    currentBodyFrameList.insert(bodyFrameItem.getBodyFrameItemID(), bodyFrameCfgWidget);
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


void MainWindow::updateBodyFrameSlot(const BodyFrame& bodyFrameItem)
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
}

void MainWindow::cfgBodyFrameItemSlot(uint frameId)
{
    if(!currentBodyFrameList.contains(frameId)){
        qDebug() << tr("该frameId不存在");
        return;
    }
    else{
        commandFileWidget = nullptr;
        BodyFrame item = currentProject->getBodyFrameItem(frameId);
        //currentWidget = std::make_shared<BodyFrameCfgWidget>(currentProject->getBodyFrameItem(frameId), this);
        bodyFrameCfgWidget = std::make_shared<BodyFrameCfgWidget>(currentProject->getBodyFrameItem(frameId), this);
        connect(bodyFrameCfgWidget.get(), &BodyFrameCfgWidget::saveBodyFrameItemSignal, this, [this](const BodyFrame& item){
            currentProject->addBodyFrameItem(item);
        });
        bodyFrameCfgWidget->setParent(ui->paraConfigWidget);
        //ui->paraConfigLayout->removeWidget()
        ui->paraConfigLayout->addWidget(bodyFrameCfgWidget.get());
        ui->paraConfigWidget->show();
        bodyFrameCfgWidget->show();
    }
}

void MainWindow::deleteBodyFrameItemSlot(uint id)
{
    currentProject->deleteBodyFrameItem(id);
    scene->deleteBodyFrameItem(bodyFrameGraphicsItems.value(id)->pos().x());
    bodyFrameGraphicsItems.remove(id);
    scene->update();
}

void MainWindow::on_actionChangeStyleSheet_triggered()
{
    StyleSheetDialog styleSheetDialog(this);
    styleSheetDialog.setWindowModality(Qt::WindowModal);
    connect(&styleSheetDialog, &StyleSheetDialog::changeStyleSheetSignal, this, &MainWindow::changeStyleSheetSlot);
    styleSheetDialog.exec();
}

void MainWindow::on_actionCreateCMDTable_triggered()
{
    if(CommandFile::createCommandFile(*currentProject) == true){
        addLogToDockWidget(QString("命令表生成成功"));
        if(currentProject->getCommandFilePath() == ""){
            currentProject->setCommandFilePath(currentProject->getName() + ".txt");
            auto p = ui->projectTreeWidget->findItems("命令表文件", Qt::MatchContains | Qt::MatchRecursive);
            for(auto &x : p){
                QTreeWidgetItem *treeWidgetItem = new QTreeWidgetItem(x);
                treeWidgetItem->setText(0, currentProject->getName() + ".txt");
                x->setExpanded(true);
            }
        }
    }
    else{
        addLogToDockWidget(QString("命令表生成失败"));
    }
}

void MainWindow::on_actionCompileCMDTable_triggered()
{
    if(CommandFile::compileCommandFile(*currentProject) == true){
        addLogToDockWidget(QString("命令表编译成功"));
    }
    else{
        addLogToDockWidget(QString("命令表生成失败"));
    }
}

void MainWindow::on_actionStartSim_triggered()
{
    qDebug() << "sim action";
    BodyFrame item1, item2;
    item1.setBodyFrameItemID(100);
    item2.setBodyFrameItemID(101);
    std::shared_ptr<BodyFrameGraphicsItem> graphicsItem1 = std::shared_ptr<BodyFrameGraphicsItem>(new BodyFrameGraphicsItem(scene->getAx(),
                                                                                                                           scene->getAy(),
                                                                                                                           scene->getBx(),
                                                                                                                           scene->getBy(),
                                                                                                                       item1));
    std::shared_ptr<BodyFrameGraphicsItem> graphicsItem2 = std::shared_ptr<BodyFrameGraphicsItem>(new BodyFrameGraphicsItem(scene->getAx(),
                                                                                                                           scene->getAy(),
                                                                                                                           scene->getBx(),
                                                                                                                           scene->getBy(),
                                                                                                                       item2));

    scene->addBodyFrameItem(graphicsItem1);
    scene->addBodyFrameItem(graphicsItem2);
    bodyFrameGraphicsItems.insert(item1.getBodyFrameItemID(), graphicsItem1);
    bodyFrameGraphicsItems.insert(item2.getBodyFrameItemID(), graphicsItem2);
    scene->addFrame(graphicsItem1, graphicsItem2);
}

void MainWindow::on_actionAbortSim_triggered()
{

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
    ui->projectTreeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void MainWindow::on_actionOpenMonitor_triggered()
{
    MonitorWidget *monitor = new MonitorWidget();

    monitor->show();
}

void MainWindow::on_actionBurnToFPGA_triggered()
{
    createBatchFile();
    QProcess *process = new QProcess(this);
    BurnToFPGADialog *dialog = new BurnToFPGADialog(this);
    connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
        QString output = process->readAllStandardOutput();
        QString inputString = output;
        QRegularExpression regex("Added Device (\\w+) successfully");
        QRegularExpressionMatchIterator matches = regex.globalMatch(inputString);

        while (matches.hasNext()) {
            QRegularExpressionMatch match = matches.next();
            if (match.hasMatch()) {
                QString matchedText = match.captured(1);
                qDebug() << matchedText;
            }
        }
        //addLogToDockWidget(output);
        //qDebug() << output;
        //std::cout << output.toStdString() << std::endl;
    });
    connect(process, &QProcess::readyReadStandardError, this, [=]() {
        QString output = process->readAllStandardError();
        addLogToDockWidget(output);
        QString inputString = output;
        QRegularExpression regex("Added Device (\\w+) successfully");
        QRegularExpressionMatchIterator matches = regex.globalMatch(inputString);

        while (matches.hasNext()) {
            QRegularExpressionMatch match = matches.next();
            if (match.hasMatch()) {
                QString matchedText = match.captured(1);
                qDebug() << matchedText;
                dialog->addItem(matchedText);
            }
        }
        //qDebug() << output;
        //std::cout << output.toStdString() << std::endl;
    });
    process->start("impact", QStringList() << "-batch");
    process->write("setmode -bs\n");
    process->write("setcable -port auto\n");
    process->write("identify\n");
    //process->closeWriteChannel();
    process->waitForReadyRead();

//    dialog->addItem("xc2v1000");
//    dialog->addItem("xc2v1000");
    dialog->setWindowFlag(Qt::Dialog);
    connect(dialog, &BurnToFPGADialog::configFinished, this, [=](QStringList commandList){
       for(QString command : commandList){
           qDebug() << command;
           process->write(command.toUtf8());
           process->waitForReadyRead();
       }
    });
    dialog->exec();
    process->kill();
    //process->waitForFinished(-1);
//    QByteArray stdcout = process->readAllStandardOutput();
//    QByteArray stdcerr = process->readAllStandardError();
//    std::cout << stdcout.toStdString() << std::endl;
//    if(process->exitCode() != 0){
//        addLogToDockWidget(stdcerr);
//    }
//    else{
//        addLogToDockWidget("命令表编译成功");
//    }
}


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
        Proj659 project = MyXml::loadProjectFromXml(filePath);
        currentProject = std::make_shared<Proj659>(project);
    }
}

void MainWindow::on_actionSaveProject_triggered()
{
    MyXml::saveProjectToXml(QString("%1.proj659").arg(currentProject->getName()), *currentProject);
    currentProject->setStatus(Proj659::saved);
}
/**
 * @brief MainWindow::addNewProjectSlot
 * @param name
 * @param info
 */
void MainWindow::addNewProjectSlot(QString name, QString info)
{
    QTreeWidgetItem *topItem = createProjectTree(name);

    if((currentProject = std::shared_ptr<Proj659>(new Proj659(name, info, topItem))) != nullptr){
        enableAllActionNeedAProject();
    }
    else{
        delete topItem;
    }
    ui->projectTreeWidget->setColumnCount(1);
    createNewScene();
}

void MainWindow::addLogToDockWidget(const QString log)
{
    QString currentTime;
    currentTime= QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->logTextBrowser->append(currentTime +  ": " + log);
}

void MainWindow::disableAllActionNeedAProject()
{
    //this->ui->menuDeviceManagement->setEnabled(false);
    this->ui->menuCMDTableManagement->setEnabled(false);
    this->ui->menuMonitor->setEnabled(false);
    this->ui->menuSimulink->setEnabled(false);
}

void MainWindow::enableAllActionNeedAProject()
{
    //this->ui->menuDeviceManagement->setEnabled(true);
    this->ui->menuCMDTableManagement->setEnabled(true);
    this->ui->menuMonitor->setEnabled(true);
    this->ui->menuSimulink->setEnabled(true);
}

QTreeWidgetItem* MainWindow::createProjectTree(QString name)
{
    ui->projectTreeWidget->setColumnCount(1);

    //拿到项目名称后更新树中的内容
    QTreeWidgetItem *topItem = new QTreeWidgetItem();

    topItem->setText(0, QString("项目\"%1\"").arg(name));

    ui->projectTreeWidget->addTopLevelItem(topItem);

    QTreeWidgetItem *item1 = new QTreeWidgetItem();

    item1->setText(0, tr("机架配置"));

    topItem->addChild(item1);

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

    return topItem;
}

void MainWindow::createNewScene()
{
    scene = std::shared_ptr<DeviceModelScene>(new DeviceModelScene());
    scene->setSceneRect(0, 0, 5000, 5000);
    ui->graphicsView->setScene(scene.get());
    ui->graphicsView->centerOn(1000, 2350);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

void MainWindow::createNewInnerBodyFrameScene(uint bodyFrameId)
{
    std::shared_ptr<InnerBodyFrameScene> innerBodyFrameScene(new InnerBodyFrameScene(currentProject->getBodyFrameItem(bodyFrameId)));
    bodyFrameScenes[bodyFrameId] = innerBodyFrameScene;
    connect(innerBodyFrameScene.get(), &InnerBodyFrameScene::exitBodyFrameSignal, this, [=](){
        ui->graphicsView->setScene(scene.get());
    });
    scene->setSceneRect(0,0,5000,5000);
    ui->graphicsView->setScene(innerBodyFrameScene.get());
    ui->graphicsView->centerOn(1000,2350);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

QString MainWindow::createBatchFile()
{
    std::ofstream os("batfile.txt");

    os << "setMode -bs" << std::endl;
    os << "setCable -port auto" << std::endl;
    os << "Identify";

    os.close();

    return QString();

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(currentProject == nullptr || currentProject->getStatus() == Proj659::saved){
        event->accept();
    }
    else{
        QString message = "项目尚未保存，确认要退出吗？";
        //QMessageBox::question(this, "退出", message, QPushButton("保存并退出"), QPushButton("取消"));
        QMessageBox box(QMessageBox::Warning, "退出", message);
        box.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        box.setDefaultButton(QMessageBox::Cancel);
        box.setButtonText(QMessageBox::Save, QString("保存并退出"));
        box.setButtonText(QMessageBox::Discard, QString("放弃修改"));
        box.setButtonText(QMessageBox::Cancel, QString("取消"));
        int choosedBtn = box.exec();
        if(choosedBtn == QMessageBox::Save){
            MyXml::saveProjectToXml(QString("%1.proj659").arg(currentProject->getName()), *currentProject);
            event->accept();
        }
        else if(choosedBtn == QMessageBox::Discard){
            event->accept();
        }
        else{
            event->ignore();
        }
    }
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
                //ui->projectTreeWidget->removeItemWidget(item, column);
                //delete item;
                currentProject = nullptr;
                scene = nullptr;
                disableAllActionNeedAProject();
                //currentBodyFrameList.clear();
            });
            connect(deleteAction, &QAction::triggered, this, [=](){
                //ui->projectTreeWidget->removeItemWidget(item, column);
                //delete item;
                //currentProject->setSave(false);
                scene = nullptr;
                currentProject = nullptr;
                //currentBodyFrameList.clear();
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
    else if(item->parent()->text(0) == "命令表文件"){
//        BodyFrame item = currentProject->getBodyFrameItem(frameId);
//        bodyFrameCfgWidget = std::make_shared<BodyFrameCfgWidget>(currentProject->getBodyFrameItem(frameId), this);
//        connect(bodyFrameCfgWidget.get(), &BodyFrameCfgWidget::saveBodyFrameItemSignal, this, [=](const BodyFrame& item){
//            currentProject->addBodyFrameItem(item);
//        });
//        bodyFrameCfgWidget->setParent(ui->paraConfigWidget);
//        ui->paraConfigLayout->addWidget(bodyFrameCfgWidget.get());
//        ui->paraConfigWidget->show();
//        bodyFrameCfgWidget->show();
        bodyFrameCfgWidget = nullptr;
        commandFileWidget = std::shared_ptr<CommandFileWidget>(new CommandFileWidget(currentProject->getCommandFilePath()));
        commandFileWidget->setParent(ui->paraConfigWidget);
        ui->paraConfigLayout->addWidget(commandFileWidget.get());
        ui->paraConfigWidget->show();
        commandFileWidget->show();
        qDebug() << "命令表文件";
    }
}

void MainWindow::onProjectItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    qDebug() << "double clicked";
}


