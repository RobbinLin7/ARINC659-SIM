#include "dataFrameCfgWidget.h"
#include "ui_dataFrameCfgWidget.h"
#include "dialog/shortsyncdialog.h"
#include <QMessageBox>

DataFrameCfgWidget::DataFrameCfgWidget(const BodyFrame& bodyFrame, const DataFrames& dataframes, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataFrameCfgWidget),
    dataframes(dataframes),
    frameIdentificationValidator(QRegExp("[a-zA-Z][a-zA-Z0-9]*"), dataframes),
    bodyFrame(bodyFrame)
{
    ui->setupUi(this);
    ui->equalAllocRadioButton->setChecked(true);
    ui->framePeriod_lineEdit->setText("30");
    ui->framePeriod_lineEdit->setStyleSheet("QLineEdit { border: 1px solid green; }");
    this->setWindowTitle(tr("帧配置"));
    installValidator();
    connect(ui->framePeriod_lineEdit, &QLineEdit::textChanged, this, &DataFrameCfgWidget::checkLineEditText);
    connect(ui->frameIdentification_lineEdit, &QLineEdit::textChanged, this, &DataFrameCfgWidget::checkLineEditText);
    ui->editWindowPushButton->setEnabled(false);
    ui->deleteWindowPushButton->setEnabled(false);
    ui->moveDownWindowPushButton->setEnabled(false);
    ui->moveUpWindowPushButton->setEnabled(false);

}

DataFrameCfgWidget::DataFrameCfgWidget(const BodyFrame& bodyFrame, const DataFrame &dataFrame, const DataFrames &dataframes, QWidget *parent):
    QWidget(parent),
    ui(new Ui::DataFrameCfgWidget),
    dataframes(dataframes),
    frameIdentificationValidator(QRegExp("[a-zA-Z][a-zA-Z0-9]*"), dataframes),
    dataFrame(dataFrame),
    bodyFrame(bodyFrame)
{
    ui->setupUi(this);
    ui->frameIdentification_lineEdit->setEnabled(false);
    this->setWindowTitle(tr("帧配置"));
    setForm();
    connect(ui->framePeriod_lineEdit, &QLineEdit::textChanged, this, &DataFrameCfgWidget::checkLineEditText);
    installValidator();
    ui->frameIdentification_lineEdit->setValidator(nullptr);
    ui->totalWindow_lineEdit->setText(QString::number(dataFrame.getFrameWindows().size()));
}

DataFrameCfgWidget::~DataFrameCfgWidget()
{
    delete ui;
}

void DataFrameCfgWidget::installValidator()
{
    framePeriodValidator = new QIntValidator(minFramePeriod, maxFramePeriod, this);
    ui->framePeriod_lineEdit->setValidator(framePeriodValidator);
    ui->frameIdentification_lineEdit->setValidator(&frameIdentificationValidator);
    ui->windowTableWidget->viewport()->installEventFilter(this);
}

bool DataFrameCfgWidget::check(QWidget *widget)
{
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(widget);
    if(lineEdit != nullptr){
        QString input = lineEdit->text();
        if(lineEdit->validator() == nullptr) return true;
        if(lineEdit->validator()->validate(input, dummy) == QValidator::Acceptable){
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

void DataFrameCfgWidget::setForm()
{
    ui->frameIdentification_lineEdit->setText(QString::fromStdString(dataFrame.getFrameIdentification()));
    ui->framePeriod_lineEdit->setText(QString::number(dataFrame.getFramePeriod()));
    ui->idleWaitTime_lineEdit->setText(QString::number(dataFrame.getIdleWaitTime()));
    dataFrame.getTimeAllocationType() == DataFrame::equalAlloc ? ui->equalAllocRadioButton->setChecked(true) : ui->downConcentrationAllocRadioButton->setChecked(true);
    for(auto& window : dataFrame.getFrameWindows()){
        addWindow(window);
    }
}

void DataFrameCfgWidget::addWindow(const FrameWindow &window)
{
    int rowIndex = ui->windowTableWidget->rowCount();
    QTableWidgetItem* windowId = new QTableWidgetItem(QString::number(rowIndex));
    //TableItem* windowIdItem = new TableItem(TableItem{0, new QTableWidgetItem(QString::number(rowIndex))});
    windowId->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem* windowType = nullptr;
    //QTableWidgetItem* windowInterruptCodeItem = nullptr;
    //QTableWidgetItem* unEditableItem = new QTableWidgetItem();
    //unEditableItem->setFlags(0);
    switch (window.getWindowType()) {
//    DATA_SEND = 0,  /* 数据传送窗口 */
//    VERSION_SEND = 1,  /* 版本校验窗口 */
//    LONG_SYNC = 2,     /* 长同步窗口 */
//    FRAME_SWITCH = 3, /* 帧切换窗口 */
//    CALL_SUBFRRAME = 4,  /* 调用子帧窗口 */
//    INT_SEND = 5,           /*中断发送窗口 */
//    FRAME_JUMP = 6,   /* 帧跳转窗口*/
//    FREE = 7,   /* 空闲等待窗口 */
//    SHORT_SYNC = 8,   /* 短同步窗口 */
    //finish
    case FrameWindow::DATA_SEND:
    {
        windowType = new QTableWidgetItem("数据传送窗口");
        QStringList receiveBlockList,sendBlockList;
        for(auto x : window.getReceiveLRMList()){
            receiveBlockList.append(QString::fromStdString(bodyFrame.getModules().at(x).getModuleName()));
        }

        sendBlockList.append(QString::fromStdString(bodyFrame.getModules().at(window.getMainLRM()).getModuleName()));
        if(window.getSupportLRM1().size() != 0) sendBlockList.append(QString::fromStdString(window.getSupportLRM1()));
        if(window.getSupportLRM2().size() != 0) sendBlockList.append(QString::fromStdString(window.getSupportLRM2()));
        if(window.getSupportLRM3().size() != 0) sendBlockList.append(QString::fromStdString(window.getSupportLRM3()));


        QTableWidgetItem* sendAddress = new QTableWidgetItem(QString::fromStdString(window.getSendAddr()));
        QTableWidgetItem* receiveAddress = new QTableWidgetItem(QString::fromStdString(window.getReceiveAddr()));
        QTableWidgetItem* sendBlock = new QTableWidgetItem(sendBlockList.join(" "));
        QTableWidgetItem* receiveBlock = new QTableWidgetItem(receiveBlockList.join(" "));
        QTableWidgetItem* numOfTimeSlot = new QTableWidgetItem(QString::number(window.getNumOfTimeSlot()));

        TableItem* windowIdItem = new TableItem{0, windowId};
        TableItem* windowTypeItem = new TableItem{1, windowType};
        TableItem* sendBlockItem = new TableItem{3,sendBlock};
        TableItem* sendAddressItem = new TableItem{4,sendAddress};
        TableItem* receiveBlockItem = new TableItem{5,receiveBlock};
        TableItem* receiveAddressItem = new TableItem{6,receiveAddress};
        TableItem* numOfTimeSlotItem = new TableItem{9,numOfTimeSlot};
        windowType->setTextAlignment(Qt::AlignCenter);
        sendAddress->setTextAlignment(Qt::AlignCenter);
        receiveAddress->setTextAlignment(Qt::AlignCenter);
        sendBlock->setTextAlignment(Qt::AlignCenter);
        receiveBlock->setTextAlignment(Qt::AlignCenter);
        numOfTimeSlot->setTextAlignment(Qt::AlignCenter);
        addTableItems(ui->windowTableWidget, rowIndex,windowIdItem,windowTypeItem,sendAddressItem,receiveAddressItem,sendBlockItem,receiveBlockItem,numOfTimeSlotItem,nullptr);


        break;
    }
    //finish
    case FrameWindow::VERSION_SEND:
    {
        windowType = new QTableWidgetItem("版本校验窗口");
        QStringList receiveBlockList,sendBlockList;
        for(auto x : window.getReceiveLRMList()){
            receiveBlockList.append(QString::fromStdString(bodyFrame.getModules().at(x).getModuleName()));
        }
        sendBlockList.append(QString::fromStdString(bodyFrame.getModules().at(window.getMainLRM()).getModuleName()));
        if(window.getSupportLRM1().size() != 0) sendBlockList.append(QString::fromStdString(window.getSupportLRM1()));
        if(window.getSupportLRM2().size() != 0) sendBlockList.append(QString::fromStdString(window.getSupportLRM2()));
        if(window.getSupportLRM3().size() != 0) sendBlockList.append(QString::fromStdString(window.getSupportLRM3()));

//        for(auto x : sendBlockList){
//            qDebug()<<x;
//        }
        QTableWidgetItem* versionReceiveAdress = new QTableWidgetItem(QString::fromStdString(window.getReceiveAddr()));
        QTableWidgetItem* versionSendBlock = new QTableWidgetItem(sendBlockList.join(" "));
        QTableWidgetItem* versionReceiveBlock = new QTableWidgetItem(receiveBlockList.join(" "));
        QTableWidgetItem* numOfTimeSlot = new QTableWidgetItem(QString::number(window.getNumOfTimeSlot()));

        TableItem* windowIdItem = new TableItem{0, windowId};
        TableItem* windowTypeItem = new TableItem{1, windowType};

        TableItem* versionSendBlockItem = new TableItem{3, versionSendBlock};
        TableItem* versionReceiveBlockItem = new TableItem{5, versionReceiveBlock};
        TableItem* versionReceiveAdressItem = new TableItem{6, versionReceiveAdress};
        TableItem* numOfTimeSlotItem = new TableItem{9,numOfTimeSlot};

        windowType->setTextAlignment(Qt::AlignCenter);
        versionSendBlock->setTextAlignment(Qt::AlignCenter);
        versionReceiveBlock->setTextAlignment(Qt::AlignCenter);
        versionReceiveAdress->setTextAlignment(Qt::AlignCenter);
        numOfTimeSlot->setTextAlignment(Qt::AlignCenter);
        addTableItems(ui->windowTableWidget, rowIndex,windowIdItem,windowTypeItem,versionSendBlockItem,versionReceiveBlockItem,versionReceiveAdressItem,numOfTimeSlotItem,nullptr);
        break;
    }
    //finish
    case FrameWindow::LONG_SYNC:
    {
        windowType = new QTableWidgetItem("长同步窗口");
        QString flag = window.getFlag()==true?"版本校验":"非版本校验";
        QStringList sendBlockList;
        sendBlockList.append(QString::fromStdString(bodyFrame.getModules().at(window.getMainLRM()).getModuleName()));
        if(window.getSupportLRM1().size() != 0) sendBlockList.append(QString::fromStdString(window.getSupportLRM1()));
        if(window.getSupportLRM2().size() != 0) sendBlockList.append(QString::fromStdString(window.getSupportLRM2()));
        if(window.getSupportLRM3().size() != 0) sendBlockList.append(QString::fromStdString(window.getSupportLRM3()));



        QTableWidgetItem* sendBlock = new QTableWidgetItem(sendBlockList.join(" "));
        QTableWidgetItem* syncCode = new QTableWidgetItem(QString::fromStdString(window.getSyncCode()));
        QTableWidgetItem* other = new QTableWidgetItem(flag);
        QTableWidgetItem* numOfTimeSlot = new QTableWidgetItem(QString::number(window.getNumOfTimeSlot()));


        TableItem* windowIdItem = new TableItem{0, windowId};
        TableItem* windowTypeItem = new TableItem{1, windowType};
        TableItem* sendBlockItem = new TableItem{3, sendBlock};
        TableItem* syncCodeItem = new TableItem{7,syncCode};
        TableItem* otherItem = new TableItem{8, other};
        TableItem* numOfTimeSlotItem = new TableItem{9,numOfTimeSlot};

        windowType->setTextAlignment(Qt::AlignCenter);
        sendBlock->setTextAlignment(Qt::AlignCenter);
        syncCode->setTextAlignment(Qt::AlignCenter);
        other->setTextAlignment(Qt::AlignCenter);
        numOfTimeSlot->setTextAlignment(Qt::AlignCenter);

        addTableItems(ui->windowTableWidget, rowIndex,windowIdItem,windowTypeItem,syncCodeItem,sendBlockItem,otherItem,numOfTimeSlotItem,nullptr);
        break;
    }
    //finish
    case FrameWindow::FRAME_SWITCH:
    {
        windowType = new QTableWidgetItem("帧切换窗口");
        QString flag = window.getFlag()==true?"版本校验":"非版本校验";

        QStringList sendBlockList;
        sendBlockList.append(QString::fromStdString(bodyFrame.getModules().at(window.getMainLRM()).getModuleName()));
        if(window.getSupportLRM1().size() != 0) sendBlockList.append(QString::fromStdString(window.getSupportLRM1()));
        if(window.getSupportLRM2().size() != 0) sendBlockList.append(QString::fromStdString(window.getSupportLRM2()));
        if(window.getSupportLRM3().size() != 0) sendBlockList.append(QString::fromStdString(window.getSupportLRM3()));

        QTableWidgetItem* other = new QTableWidgetItem(flag);
        QTableWidgetItem* sendBlock = new QTableWidgetItem(sendBlockList.join(" "));
        QTableWidgetItem* syncCode = new QTableWidgetItem(QString::fromStdString(window.getSyncCode()));
        QTableWidgetItem* newFrameId = new QTableWidgetItem(QString::fromStdString("新帧标识：" + window.getNewFrameID()));
        QTableWidgetItem* numOfTimeSlot = new QTableWidgetItem(QString::number(window.getNumOfTimeSlot()));

        TableItem* windowIdItem = new TableItem{0, windowId};
        TableItem* windowTypeItem = new TableItem{1, windowType};
        TableItem* sendBlockItem = new TableItem{3, sendBlock};
        TableItem* newFrameIdItem = new TableItem{5, newFrameId};
        TableItem* syncCodeItem = new TableItem{7,syncCode};
        TableItem* otherItem = new TableItem{8, other};
        TableItem* numOfTimeSlotItem = new TableItem{9,numOfTimeSlot};

        syncCode->setTextAlignment(Qt::AlignCenter);
        other->setTextAlignment(Qt::AlignCenter);
        sendBlock->setTextAlignment(Qt::AlignCenter);
        newFrameId->setTextAlignment(Qt::AlignCenter);
        windowType->setTextAlignment(Qt::AlignCenter);
        numOfTimeSlot->setTextAlignment(Qt::AlignCenter);

        addTableItems(ui->windowTableWidget, rowIndex,windowIdItem,windowTypeItem,syncCodeItem,sendBlockItem,newFrameIdItem,otherItem,numOfTimeSlotItem,nullptr);
        break;
    }
    //finish
    case FrameWindow::CALL_SUBFRRAME:
    {
        QString flag = window.getFlag()==true?"固有空闲":"非固有空闲";
        windowType = new QTableWidgetItem("调用子帧窗口");
        QTableWidgetItem* other = new QTableWidgetItem(flag);
        QTableWidgetItem* newFrameId = new QTableWidgetItem(QString::fromStdString("帧标识：" + window.getNewFrameID()));
        QTableWidgetItem* numOfTimeSlot = new QTableWidgetItem(QString::number(window.getNumOfTimeSlot()));


        TableItem* windowIdItem = new TableItem{0, windowId};
        TableItem* windowTypeItem = new TableItem{1, windowType};
        TableItem* newFrameIdItem = new TableItem{5, newFrameId};
        TableItem* otherItem = new TableItem{8, other};
        TableItem* numOfTimeSlotItem = new TableItem{9,numOfTimeSlot};


        windowType->setTextAlignment(Qt::AlignCenter);
        newFrameId->setTextAlignment(Qt::AlignCenter);
        numOfTimeSlot->setTextAlignment(Qt::AlignCenter);
        other->setTextAlignment(Qt::AlignCenter);
        addTableItems(ui->windowTableWidget,rowIndex,windowIdItem,windowTypeItem,newFrameIdItem,otherItem,numOfTimeSlotItem,nullptr);
        break;
    }
    //中断接受模块需整体更改
    case FrameWindow::INT_SEND:
    {
        windowType = new QTableWidgetItem("中断发送窗口");
        QTableWidgetItem* windowInterruptCode = new QTableWidgetItem(QString::number(window.getIntNum1()));
        QTableWidgetItem* interruptReceiver = new QTableWidgetItem(QString::number(window.getIntNum2()));
        QTableWidgetItem* numOfTimeSlot = new QTableWidgetItem(QString::number(window.getNumOfTimeSlot()));


        TableItem* windowIdItem = new TableItem{0, windowId};
        TableItem* windowTypeItem = new TableItem{1, windowType};
        TableItem* windowInterruptCodeItem = new TableItem{2,windowInterruptCode};
        TableItem* interruptReceiverItem = new TableItem{5,interruptReceiver};
        TableItem* numOfTimeSlotItem = new TableItem{9,numOfTimeSlot};

        windowType->setTextAlignment(Qt::AlignCenter);
        windowInterruptCode->setTextAlignment(Qt::AlignCenter);
        interruptReceiver->setTextAlignment(Qt::AlignCenter);
        numOfTimeSlot->setTextAlignment(Qt::AlignCenter);
        addTableItems(ui->windowTableWidget, rowIndex,windowIdItem,windowTypeItem,windowInterruptCodeItem,interruptReceiverItem,numOfTimeSlotItem,nullptr);
        break;
    }
    //finish
    case FrameWindow::FRAME_JUMP:
    {
        QString flag = window.getFlag()==true?"固有空闲":"非固有空闲";
        windowType = new QTableWidgetItem("帧跳转窗口");
        QTableWidgetItem* other = new QTableWidgetItem(flag);
        QTableWidgetItem* newFrameId = new QTableWidgetItem(QString::fromStdString("新帧标识：" + window.getNewFrameID()));
        QTableWidgetItem* numOfTimeSlot = new QTableWidgetItem(QString::number(window.getNumOfTimeSlot()));


        TableItem* windowIdItem = new TableItem{0, windowId};
        TableItem* windowTypeItem = new TableItem{1, windowType};
        TableItem* newFrameIdItem = new TableItem{5, newFrameId};
        TableItem* otherItem = new TableItem{8, other};
        TableItem* numOfTimeSlotItem = new TableItem{9,numOfTimeSlot};


        windowType->setTextAlignment(Qt::AlignCenter);
        newFrameId->setTextAlignment(Qt::AlignCenter);
        numOfTimeSlot->setTextAlignment(Qt::AlignCenter);
        other->setTextAlignment(Qt::AlignCenter);
        addTableItems(ui->windowTableWidget,rowIndex,windowIdItem,windowTypeItem,newFrameIdItem,otherItem,numOfTimeSlotItem,nullptr);
        break;
    }
    //finish
    case FrameWindow::FREE:
    {
        windowType = new QTableWidgetItem("空闲等待窗口");
        QTableWidgetItem* sendTimeScale = new QTableWidgetItem(QString::number(window.getSendTimeScale()));
        QTableWidgetItem* numOfTimeSlot = new QTableWidgetItem(QString::number(window.getNumOfTimeSlot()));


        TableItem* windowIdItem = new TableItem{0, windowId};
        TableItem* windowTypeItem = new TableItem{1, windowType};
        TableItem* sendTimeScaleItem = new TableItem{8, sendTimeScale};
        TableItem* numOfTimeSlotItem = new TableItem{9,numOfTimeSlot};


        windowType->setTextAlignment(Qt::AlignCenter);
        sendTimeScale->setTextAlignment(Qt::AlignCenter);
        numOfTimeSlot->setTextAlignment(Qt::AlignCenter);
        addTableItems(ui->windowTableWidget,rowIndex,windowIdItem,windowTypeItem,sendTimeScaleItem,numOfTimeSlotItem,nullptr);
        break;
    }
    case FrameWindow::SHORT_SYNC:{
        windowType = new QTableWidgetItem("短同步窗口");
        QTableWidgetItem* numOfTimeSlot = new QTableWidgetItem(QString::number(window.getNumOfTimeSlot()));


        TableItem* windowIdItem = new TableItem{0, windowId};
        TableItem* windowTypeItem = new TableItem{1, windowType};
        TableItem* numOfTimeSlotItem = new TableItem{9,numOfTimeSlot};

        windowType->setTextAlignment(Qt::AlignCenter);
        numOfTimeSlot->setTextAlignment(Qt::AlignCenter);
        addTableItems(ui->windowTableWidget, rowIndex, windowIdItem, windowTypeItem,numOfTimeSlotItem ,nullptr);
        break;
    }
    default:
        break;
    }

//    uint32_t totalTimeUpdate;
//    totalTimeUpdate = this->dataFrame.getTotalTimeSlot()+ window.getNumOfTimeSlot();
//    this->dataFrame.setTotalTimeSlot(totalTimeUpdate);
//    ui->totalWindow_lineEdit->setText(QString::number(totalTimeUpdate));

}

bool DataFrameCfgWidget::addTableItems(QTableWidget *tableWidget, int rowIndex, TableItem *firstItem, ...)
{
    //int colIndex = 0;
    TableItem* currentItem = firstItem;
    va_list args;
    va_start(args, firstItem);
    tableWidget->insertRow(rowIndex);
    while(currentItem){
        tableWidget->setItem(rowIndex, currentItem->index, currentItem->item);
        currentItem = va_arg(args, TableItem*);
    }
    va_end(args);
    return true;
}

bool DataFrameCfgWidget::addTableItems(QTableWidget *tableWidget, int rowIndex, QTableWidgetItem *firstItem, ...)
{
    int colIndex = 0;
    QTableWidgetItem* currentItem = firstItem;
    va_list args;
    va_start(args, firstItem);
    tableWidget->insertRow(rowIndex);
    while(currentItem){
        tableWidget->setItem(rowIndex, colIndex++, currentItem);
        currentItem = va_arg(args, QTableWidgetItem*);
    }
    va_end(args);
    return true;
}

WindowCfgDialog *DataFrameCfgWidget::newWindowCfgDialog()
{
        int row = ui->windowTableWidget->currentRow();
        const FrameWindow& window = dataFrame.getFrameWindows().at(row);
        WindowCfgDialog* dialog = nullptr;
        switch (window.getWindowType()) {
    //    DATA_SEND = 0,  /* 数据传送窗口 */
    //    VERSION_SEND = 1,  /* 版本校验窗口 */
    //    LONG_SYNC = 2,     /* 长同步窗口 */
    //    FRAME_SWITCH = 3, /* 帧切换窗口 */
    //    CALL_SUBFRRAME = 4,  /* 调用子帧窗口 */
    //    INT_SEND = 5,           /*中断发送窗口 */
    //    FRAME_JUMP = 6,   /* 帧跳转窗口*/
    //    FREE = 7,   /* 空闲等待窗口 */
    //    SHORT_SYNC = 8,   /* 短同步窗口 */
        case FrameWindow::DATA_SEND:{
            dialog = new DataTransferWindowCfgDialog(row, window, bodyFrame.getModules(), this);
            dialog->setWindowFlag(Qt::Dialog);
            break;
        }
        case FrameWindow::VERSION_SEND:{
            dialog = new VersionDetectionWindowCfgDialog(row, window,bodyFrame.getModules(),this);
            dialog->setWindowFlag(Qt::Dialog);
            break;
        }
        case FrameWindow::LONG_SYNC:{
            dialog = new LongSyncWindowCfgDialog(row, window,bodyFrame.getModules(),this);
            dialog->setWindowFlag(Qt::Dialog);
            break;
        }
        case FrameWindow::FRAME_SWITCH:{
            dialog = new FrameSwitchWindowCfgDialog(row, window,bodyFrame.getModules(),this);
            dialog->setWindowFlag(Qt::Dialog);
            break;
        }
        case FrameWindow::CALL_SUBFRRAME:{
            dialog = new SubFrameCallWindowDialog(row,window,this);
            dialog->setWindowFlag(Qt::Dialog);
            break;
        }
        case FrameWindow::INT_SEND:{
            dialog = new InterruptionWindowCfgDialog(row, window ,this);
            dialog->setWindowFlag(Qt::Dialog);
            break;
        }
        case FrameWindow::FRAME_JUMP:{
            dialog = new JumpWindowCfgDialog(row,window,this);
            dialog->setWindowFlag(Qt::Dialog);
            break;
        }
        case FrameWindow::FREE:{
            dialog = new FreeWindowCfgDialog(row, this);
            dialog->setWindowFlag(Qt::Dialog);
            break;
        }
        case FrameWindow::SHORT_SYNC:{
            break;
        }
        default:
            break;
        }
        return dialog;
}

void DataFrameCfgWidget::changeOrderOfTwoRows(int row1, int row2)
{
    for (int column = 1; column < ui->windowTableWidget->columnCount(); ++column) {
        QTableWidgetItem* item1 = ui->windowTableWidget->takeItem(row1, column);
        QTableWidgetItem* item2 = ui->windowTableWidget->takeItem(row2, column);

        // 将单元格项设置回表格中的另一个行
        ui->windowTableWidget->setItem(row1, column, item2);
        ui->windowTableWidget->setItem(row2, column, item1);
    }
}

void DataFrameCfgWidget::checkLineEditText()
{
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(sender());
    if(lineEdit == nullptr) return;
    QString input = lineEdit->text();
    if(input == ""){
        lineEdit->setStyleSheet("QLineEdit { border: 1px solid gray;}");
    }
    else if(lineEdit->validator() == nullptr){
        qDebug() << "hhhhhh";
    }
    else if(lineEdit->validator()->validate(input, dummy) == QValidator::Acceptable){
        lineEdit->setStyleSheet("QLineEdit { border: 1px solid green; }");
    }
    else{
        lineEdit->setStyleSheet("QLineEdit { border: 1px solid red; }");
    }
}

void DataFrameCfgWidget::on_okPushButton_clicked(bool)
{
    if(check(ui->framePeriod_lineEdit) == false){
        QMessageBox::warning(this, "错误", "帧周期参数有误");
        return;
    }
    else if(check(ui->frameIdentification_lineEdit) == false){
        QMessageBox::warning(this, "错误", "模标识参数有误");
        return;
    }
    dataFrame.setFramePeriod(ui->framePeriod_lineEdit->text().toUInt());
    dataFrame.setFrameIdentification(ui->frameIdentification_lineEdit->text().toStdString());
    dataFrame.setIdleWaitTime(ui->idleWaitTime_lineEdit->text().toUInt());
    dataFrame.setTotalWindow(ui->totalWindow_lineEdit->text().toUInt());
    dataFrame.setTimeAllocationType(ui->equalAllocRadioButton->isChecked() ? DataFrame::equalAlloc : DataFrame::downConcentrationAlloc);
    emit saveDataFrameSignal(dataFrame);
    this->close();
}

void DataFrameCfgWidget::on_cancelPushButton_clicked(bool)
{
    this->close();
}

void DataFrameCfgWidget::on_dataTransferPushButton_clicked(bool)
{
    DataTransferWindowCfgDialog* dialog = new DataTransferWindowCfgDialog(ui->windowTableWidget->rowCount(), bodyFrame.getModules(), this);
    connect(dialog, &DataTransferWindowCfgDialog::addNewWindow, this, &DataFrameCfgWidget::addNewWindow);
    dialog->setWindowFlag(Qt::Dialog);
    dialog->exec();
}

void DataFrameCfgWidget::on_interruptionPushButton_clicked(bool)
{
   InterruptionWindowCfgDialog* dialog = new InterruptionWindowCfgDialog(ui->windowTableWidget->rowCount(), this);
   connect(dialog, &WindowCfgDialog::addNewWindow, this, &DataFrameCfgWidget::addNewWindow);
   dialog->setWindowFlag(Qt::Dialog);
   dialog->exec();
}

void DataFrameCfgWidget::on_longSyncPushButton_clicked(bool)
{
    LongSyncWindowCfgDialog* dialog = new LongSyncWindowCfgDialog(ui->windowTableWidget->rowCount(),bodyFrame.getModules() ,this);
    connect(dialog, &WindowCfgDialog::addNewWindow, this, &DataFrameCfgWidget::addNewWindow);
    dialog->setWindowFlag(Qt::Dialog);
    dialog->exec();
}

void DataFrameCfgWidget::on_jumpPushButton_clicked(bool)
{
    JumpWindowCfgDialog* dialog = new JumpWindowCfgDialog(ui->windowTableWidget->rowCount(), this);
    connect(dialog, &WindowCfgDialog::addNewWindow, this, &DataFrameCfgWidget::addNewWindow);
    dialog->setWindowFlag(Qt::Dialog);
    dialog->exec();
}

void DataFrameCfgWidget::on_versionDetectionPushButton_clicked(bool)
{
    VersionDetectionWindowCfgDialog* dialog = new VersionDetectionWindowCfgDialog(ui->windowTableWidget->rowCount(),bodyFrame.getModules() ,this);
    connect(dialog, &WindowCfgDialog::addNewWindow, this, &DataFrameCfgWidget::addNewWindow);
    dialog->setWindowFlag(Qt::Dialog);
    dialog->exec();
}

void DataFrameCfgWidget::on_subFrameCallPushButton_clicked(bool)
{
    SubFrameCallWindowDialog* dialog = new SubFrameCallWindowDialog(ui->windowTableWidget->rowCount(), this);
    connect(dialog, &WindowCfgDialog::addNewWindow, this, &DataFrameCfgWidget::addNewWindow);
    dialog->setWindowFlag(Qt::Dialog);
    dialog->exec();
}

void DataFrameCfgWidget::on_freePushButton_clicked(bool)
{
    FreeWindowCfgDialog* dialog = new FreeWindowCfgDialog(ui->windowTableWidget->rowCount(), this);
    connect(dialog, &WindowCfgDialog::addNewWindow, this, &DataFrameCfgWidget::addNewWindow);
    dialog->setWindowFlag(Qt::Dialog);
    dialog->exec();
}

void DataFrameCfgWidget::on_frameSwitchPushButton_clicked(bool)
{
    FrameSwitchWindowCfgDialog* dialog = new FrameSwitchWindowCfgDialog(ui->windowTableWidget->rowCount(),bodyFrame.getModules(), this);
    connect(dialog, &WindowCfgDialog::addNewWindow, this, &DataFrameCfgWidget::addNewWindow);
    dialog->setWindowFlag(Qt::Dialog);
    dialog->exec();
}

void DataFrameCfgWidget::on_shortSyncPushButton_clicked(bool)
{
    ShortSyncDialog* dialog = new ShortSyncDialog(ui->windowTableWidget->rowCount(), this);
    connect(dialog, &WindowCfgDialog::addNewWindow, this, &DataFrameCfgWidget::addNewWindow);
    dialog->setWindowFlag(Qt::Dialog);
    dialog->exec();
    //FrameWindow window;
    //window.setWindowType(FrameWindow::SHORT_SYNC);
    //addNewWindow(window);
}

void DataFrameCfgWidget::on_editWindowPushButton_clicked(bool)
{
    WindowCfgDialog *dialog = newWindowCfgDialog();
    dialog->exec();
}

void DataFrameCfgWidget::on_deleteWindowPushButton_clicked(bool)
{
    if(ui->windowTableWidget->currentRow() >= 0){
        int row = ui->windowTableWidget->currentRow();
        uint32_t totalTimeUpdate;
        totalTimeUpdate = this->dataFrame.getTotalTimeSlot() - ui->windowTableWidget->item(row, 9)->text().toUInt();
        this->dataFrame.setTotalTimeSlot(totalTimeUpdate);
        ui->totalTimeSlot_LineEdit->setText(QString::number(totalTimeUpdate));
        ui->windowTableWidget->removeRow(row);
        dataFrame.deleteFrameWindowAtIndex(row);
        ui->totalWindow_lineEdit->setText(QString::number(ui->totalWindow_lineEdit->text().toUInt() - 1));
//        uint32_t totalTimeUpdate;
//        totalTimeUpdate = this->dataFrame.getTotalTimeSlot() - ui->windowTableWidget->item(row, 9)->text().toUInt();
//        this->dataFrame.setTotalTimeSlot(totalTimeUpdate);
//        ui->totalTimeSlot_LineEdit->setText(QString::number(totalTimeUpdate));
        while(row < ui->windowTableWidget->rowCount()){
            uint id = ui->windowTableWidget->item(row, 0)->text().toUInt() - 1;
            ui->windowTableWidget->item(row, 0)->setText(QString::number(id));
            ++row;
        }
    }
    else{
        qDebug() << "选中项目为空";
    }
}

void DataFrameCfgWidget::on_moveUpWindowPushButton_clicked(bool)
{
    int row = ui->windowTableWidget->currentRow();
    changeOrderOfTwoRows(row, row - 1);
    dataFrame.moveUpFrameWindowAtIndex(row);
}

void DataFrameCfgWidget::on_moveDownWindowPushButton_clicked(bool)
{
    int row = ui->windowTableWidget->currentRow();
    changeOrderOfTwoRows(row, row + 1);
    dataFrame.moveDownFrameWindowAtIndex(row);
}

void DataFrameCfgWidget::addNewWindow(const FrameWindow &frameWindow)
{
    dataFrame.insertFrameWindow(frameWindow);
    addWindow(frameWindow);
    ui->totalWindow_lineEdit->setText(QString::number(ui->totalWindow_lineEdit->text().toUInt() + 1));
    uint32_t totalTimeUpdate;
    totalTimeUpdate = this->dataFrame.getTotalTimeSlot()+ frameWindow.getNumOfTimeSlot();
    this->dataFrame.setTotalTimeSlot(totalTimeUpdate);
    ui->totalTimeSlot_LineEdit->setText(QString::number(totalTimeUpdate));
}

bool DataFrameCfgWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->windowTableWidget->viewport() && event->type() == QEvent::MouseButtonPress){
        QPoint globalPos = QCursor::pos();
        QPoint localPos = ui->windowTableWidget->viewport()->mapFromGlobal(globalPos);
        int row = ui->windowTableWidget->rowAt(localPos.y());
        if(row >= 0){
            ui->editWindowPushButton->setEnabled(true);
            ui->deleteWindowPushButton->setEnabled(true);
            if(row > 0){
                ui->moveUpWindowPushButton->setEnabled(true);
            }
            else{
                ui->moveUpWindowPushButton->setEnabled(false);
            }
            if(row < ui->windowTableWidget->rowCount() - 1){
                ui->moveDownWindowPushButton->setEnabled(true);
            }
            else{
                ui->moveDownWindowPushButton->setEnabled(false);
            }
        }
        else{
            ui->editWindowPushButton->setEnabled(false);
            ui->deleteWindowPushButton->setEnabled(false);
            ui->moveUpWindowPushButton->setEnabled(false);
            ui->moveDownWindowPushButton->setEnabled(false);
        }
    }
    return QWidget::eventFilter(watched, event);
}




