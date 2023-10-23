#include "bodyFrameCfgWidget.h"
#include "ui_bodyFrameCfgWidget.h"
#include <QDebug>

BodyFrameCfgWidget::BodyFrameCfgWidget(uint frameId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BodyFrameCfgWidget),
    openMode(New)
{
    ui->setupUi(this);
    ui->id_lineEdit->setText(QString::number(frameId));
    ui->tabWidget->setCurrentIndex(0);
    this->initWidget();
    installEventFilter();
    installValidator();
}

BodyFrameCfgWidget::BodyFrameCfgWidget(const BodyFrame bodyFrameItem, QWidget *parent):
    QWidget(parent),
    ui(new Ui::BodyFrameCfgWidget),
    bodyFrameItem(bodyFrameItem)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    this->initWidget();
    setForm();
    installEventFilter();
    installValidator();
}

//BodyFrameCfgWidget::BodyFrameCfgWidget(const BodyFrameItem &bodyFrameItem, QWidget *parent):
//    QWidget(parent),
//    ui(new Ui::BodyFrameCfgWidget),
//    openMode(Modified),
//    bodyFrameItem(bodyFrameItem.getBodyFrameItemID())
//{
//    ui->setupUi(this);
//    this->initWidget();
//    installEventFilter();
//    installValidator();
//}


//BodyFrameCfgWidget::BodyFrameCfgWidget(const BodyFrameCfgWidget &bodyFrameWidget):
//    QWidget(bodyFrameWidget.parentWidget()),
//    ui(new Ui::BodyFrameCfgWidget),
//    openMode(Modified),
//    bodyFrameItem(bodyFrameWidget.getBodyFrameItem())

//{
//    ui->setupUi(this);
//    this->initWidget();
//    installEventFilter();
//    installValidator();
//}

//BodyFrameCfgWidget::BodyFrameCfgWidget(BodyFrameItem &bodyFrameItem, QWidget *parent):
//    QWidget(parent),
//    ui(new Ui::BodyFrameCfgWidget),
//    bodyFrameItem(bodyFrameItem)
//{
//    ui->setupUi(this);
//    this->bodyFrameItem = bodyFrameItem;
//    this->initWidget(bodyFrameItem);
//    installEventFilter();
//    installValidator();
//}


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
    //disconnect(ui->buttonBox->button(QDialogButtonBox::Ok), 0, 0, 0);
//    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, [=](){
//        ok = true;
//        emit(updateBodyFrameItemSignal(bodyFrameItem));
//        this->close();
//    });
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

void BodyFrameCfgWidget::addModuleSlot(const Module& module)
{
    bodyFrameItem.addModule(module);
    addModuleToTableWidget(module);
//    int rowIndex = ui->moduleTableWidget->rowCount();
//    ui->moduleTableWidget->insertRow(rowIndex);
//    QTableWidgetItem *moduleNumberItem = new QTableWidgetItem(QString::number(module.getModuleNumber()));
//    QTableWidgetItem *initialWaitTimeItem = new QTableWidgetItem(QString::number(module.getInitialWaitTime()));
//    QTableWidgetItem *moduleNameItem = new QTableWidgetItem(QString::fromStdString(module.getModuleName()));
//    moduleNumberItem->setTextAlignment(Qt::AlignCenter);
//    initialWaitTimeItem->setTextAlignment(Qt::AlignCenter);
//    moduleNameItem->setTextAlignment(Qt::AlignCenter);
//    ui->moduleTableWidget->setItem(rowIndex, 0, moduleNumberItem);
//    ui->moduleTableWidget->setItem(rowIndex, 1, initialWaitTimeItem);
//    ui->moduleTableWidget->setItem(rowIndex, 2, moduleNameItem);
//    addTableItems(ui->moduleTableWidget, rowIndex, moduleNameItem, initialWaitTimeItem, moduleNameItem, nullptr);
}

void BodyFrameCfgWidget::addDataFrameSlot(const DataFrame &dataFrame)
{
    bodyFrameItem.addDataFrame(dataFrame);
    addDataFrameToTableWidget(dataFrame);
//    int rowIndex = ui->dataFrameTableWidget->rowCount();
//    ui->dataFrameTableWidget->insertRow(rowIndex);
//    QTableWidgetItem* dataFrameIdentificationItem = nullptr;
//    (dataFrameIdentificationItem = new QTableWidgetItem(QString::fromStdString(dataFrame.getFrameIdentification())))->setTextAlignment(Qt::AlignCenter);
//    QTableWidgetItem* dataFrameTotalWindowItem = nullptr;
//    (dataFrameTotalWindowItem = new QTableWidgetItem(QString::number(dataFrame.getTotalWindow())))->setTextAlignment(Qt::AlignCenter);
//    QTableWidgetItem* dataFramePeriodItem = nullptr;
//    (dataFramePeriodItem = new QTableWidgetItem(QString::number(dataFrame.getFramePeriod())))->setTextAlignment(Qt::AlignCenter);
//    QTableWidgetItem* dataFrameIdleWayItem = nullptr;
//    (dataFrameIdleWayItem = new QTableWidgetItem("不知道是什么东西"))->setTextAlignment(Qt::AlignCenter);
//    QTableWidgetItem* dataFrameChildFramesItem = nullptr;
//    (dataFrameChildFramesItem = new QTableWidgetItem("不知道啥*2"))->setTextAlignment(Qt::AlignCenter);
//    addTableItems(ui->dataFrameTableWidget, rowIndex, dataFrameIdentificationItem, dataFrameTotalWindowItem, dataFramePeriodItem, dataFrameIdleWayItem,
//                  dataFrameChildFramesItem, nullptr);
//    ui->dataFrameTableWidget->setItem(rowIndex, 0, dataFrameIdentificationItem);
//    ui->dataFrameTableWidget->setItem(rowIndex, 1, dataFrameTotalWindowItem);
//    ui->dataFrameTableWidget->setItem(rowIndex, 2, dataFramePeriodItem);
//    ui->dataFrameTableWidget->setItem(rowIndex, 3, dataFrameIdleWayItem);
//    ui->dataFrameTableWidget->setItem(rowIndex, 4, dataFrameChildFramesItem);
}

void BodyFrameCfgWidget::modifyModuleSlot(const Module& module)
{
    for(int i = 0; i < ui->moduleTableWidget->rowCount(); i++){
        if(ui->moduleTableWidget->item(i, 0)->text() == QString::number(module.getModuleNumber())){
            ui->moduleTableWidget->item(i, 1)->setText(QString::number(module.getInitialWaitTime()));
            ui->moduleTableWidget->item(i, 2)->setText(QString::fromStdString(module.getModuleName()));
        }
    }
}

void BodyFrameCfgWidget::modifyDataFrameSlot(const DataFrame &dataFrame)
{
    bodyFrameItem.modifyDataFrame(dataFrame);
}

void BodyFrameCfgWidget::on_modifyModuleBtn_clicked(bool)
{
    if(ui->moduleTableWidget->currentItem() != nullptr){
        int row = ui->moduleTableWidget->currentRow();
        qDebug() << ui->moduleTableWidget->item(row, 0)->text();
        uint moduleId = ui->moduleTableWidget->item(row, 0)->text().toUInt();
        Module& module = bodyFrameItem.getModule(moduleId);
        ModuleCfgWidget *hm = new ModuleCfgWidget(module, this);
        connect(hm, &ModuleCfgWidget::saveModuleSignal, this, &BodyFrameCfgWidget::modifyModuleSlot);
        hm->setWindowFlag(Qt::Dialog);
        hm->show();
    }
    else{
        qDebug() << "currentItem is null";
    }
}

void BodyFrameCfgWidget::on_deleteModuleBtn_clicked(bool)
{
    if(ui->moduleTableWidget->currentItem() != nullptr){
        int row = ui->moduleTableWidget->currentRow();
        qDebug() << ui->moduleTableWidget->item(row, 0)->text();
        uint moduleNumber = ui->moduleTableWidget->item(row, 0)->text().toUInt();
        ui->moduleTableWidget->removeRow(row);
        bodyFrameItem.deleteModule(moduleNumber);
    }
    else{
        qDebug() << "currentItem is null";
    }
}


void BodyFrameCfgWidget::on_addModuleBtn_clicked()
{
    module = std::shared_ptr<Module>(new Module());
    ModuleCfgWidget *hm = new ModuleCfgWidget(bodyFrameItem.getMinUnusedModuleId(), this);
    hm->setWindowFlag(Qt::Dialog);
    connect(hm, &ModuleCfgWidget::saveModuleSignal, this, &BodyFrameCfgWidget::addModuleSlot);
    hm->show();
}

void BodyFrameCfgWidget::on_deleteDataFrameBtn_clicked()
{
    if(ui->dataFrameTableWidget->currentItem() != nullptr){
        int row = ui->dataFrameTableWidget->currentRow();
        qDebug() << ui->dataFrameTableWidget->item(row, 0)->text();
        std::string dataFrameIdentification = ui->dataFrameTableWidget->item(row, 0)->text().toStdString();
        ui->moduleTableWidget->removeRow(row);
        bodyFrameItem.deleteDataFrame(dataFrameIdentification);
    }
    else{
        qDebug() << "currentItem is null";
    }
}

void BodyFrameCfgWidget::on_modifyDataFrameBtn_clicked()
{
    int row = ui->dataFrameTableWidget->currentRow();
    std::string bodyFrameIdentification = ui->dataFrameTableWidget->item(row, 0)->text().toStdString();
    DataFrameCfgWidget* dataFrameCfgWidget = new DataFrameCfgWidget(bodyFrameItem, bodyFrameItem.getDataFrame(bodyFrameIdentification), bodyFrameItem.getDataFrames(), this);
    connect(dataFrameCfgWidget, &DataFrameCfgWidget::saveDataFrameSignal, this, &BodyFrameCfgWidget::modifyDataFrameSlot);
    dataFrameCfgWidget->setWindowFlag(Qt::Dialog);
    dataFrameCfgWidget->show();
}

void BodyFrameCfgWidget::on_addDataFrameBtn_clicked()
{
    DataFrameCfgWidget *frame = new DataFrameCfgWidget(bodyFrameItem, bodyFrameItem.getDataFrames(), this);
    connect(frame, &DataFrameCfgWidget::saveDataFrameSignal, this, &BodyFrameCfgWidget::addDataFrameSlot);
    frame->setWindowFlag(Qt::Dialog);
    frame->show();
}

void BodyFrameCfgWidget::on_okButton_clicked(bool)
{
    if(check(ui->arbitrationStepDuration_lineEdit) == false){
        QMessageBox::warning(this, "错误", "主/后备仲裁步进时长参数有误");
        return;
    }
    else if(check(ui->timeCalibrationFactor_lineEdit) == false){
        QMessageBox::warning(this, "错误", "时间标定因子参数有误");
        return;
    }
    else if(check(ui->messageInterval_lineEdit) == false){
        QMessageBox::warning(this, "错误", "消息间隔参数有误");
        return;
    }
    else if(check(ui->majorVersionNumber_lineEdit) == false){
        QMessageBox::warning(this, "错误", "主版本号参数有误");
        return;
    }
    else if(check(ui->subVersionNumber_lineEdit) == false){
        QMessageBox::warning(this, "错误", "次版本号参数有误");
        return;
    }
    else{
        ok = true;
        bodyFrameItem.setBodyFrameItemID(ui->id_lineEdit->text().toUInt());
        bodyFrameItem.setArbitrationStepDuration(ui->arbitrationStepDuration_lineEdit->text().toUInt());
        bodyFrameItem.setTimeCalibrationFactor(ui->timeCalibrationFactor_lineEdit->text().toUInt());
        bodyFrameItem.setMajorVersionNumber(ui->majorVersionNumber_lineEdit->text().toUInt());
        bodyFrameItem.setSubVersionNumber(ui->subVersionNumber_lineEdit->text().toUInt());
        bodyFrameItem.setMessageInterval(ui->messageInterval_lineEdit->text().toUInt());
        emit saveBodyFrameItemSignal(bodyFrameItem);
        this->close();
    }
}

void BodyFrameCfgWidget::on_cancelButton_clicked(bool)
{
    this->close();
}

void BodyFrameCfgWidget::on_moveUpBtn_clicked()
{
    int row1 = ui->dataFrameTableWidget->currentRow();
    int row2 = row1 - 1;
    exchangeDataFrameItemOrder(row1, row2);
}

void BodyFrameCfgWidget::on_moveDownBtn_clicked()
{
    int row1 = ui->dataFrameTableWidget->currentRow();
    int row2 = row1 + 1;
    exchangeDataFrameItemOrder(row1, row2);
}

void BodyFrameCfgWidget::showEvent(QShowEvent *event)
{
    if(openMode == Modified){
        ui->id_lineEdit->setText(QString::number(bodyFrameItem.getBodyFrameItemID()));
        ui->arbitrationStepDuration_lineEdit->setText(QString::number(bodyFrameItem.getArbitrationStepDuration()));
        ui->timeCalibrationFactor_lineEdit->setText(QString::number(bodyFrameItem.getTimeCalibrationFactor()));
        ui->messageInterval_lineEdit->setText(QString::number(bodyFrameItem.getMessageInterval()));
        ui->majorVersionNumber_lineEdit->setText(QString::number(bodyFrameItem.getMajorVersionNumber()));
        ui->subVersionNumber_lineEdit->setText(QString::number(bodyFrameItem.getSubVersionNumber()));
        auto dataFrames = bodyFrameItem.getDataFrames();
        ui->dataFrameTableWidget->clear();
        ui->moduleTableWidget->clear();
        for(const std::string &dataFrameId: bodyFrameItem.getDataFramesOrder()){
            addDataFrameSlot(dataFrames.at(dataFrameId));
        }
        for(auto& module : bodyFrameItem.getModules()){
            addModuleSlot(module.second);
        }
    }
    QWidget::showEvent(event);
}

void BodyFrameCfgWidget::closeEvent(QCloseEvent *e)
{
    openMode = Modified;
    if(ok){
        ok = false;
        e->accept();
        return;
    }
    QString message = (openMode == New ? "确定要放弃新建机架吗?" : "确定要放弃修改机架吗?");
    QMessageBox box(QMessageBox::Warning, "退出", message);
    box.setStandardButtons(QMessageBox::Discard | QMessageBox::Cancel);
    box.setDefaultButton(QMessageBox::Cancel);
    box.setButtonText(QMessageBox::Discard, QString("放弃"));
    box.setButtonText(QMessageBox::Cancel, QString("取消"));
    int choosedBtn = box.exec();
    if(choosedBtn == QMessageBox::Discard){
        e->accept();
    }
    else{
        e->ignore();
    }
}

bool BodyFrameCfgWidget::eventFilter(QObject *watched, QEvent *event)
{
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(watched);
    if(watched == ui->moduleTableWidget->viewport() && event->type() == QEvent::MouseButtonPress){
        QPoint globalPos = QCursor::pos();
        QPoint localPos = ui->moduleTableWidget->viewport()->mapFromGlobal(globalPos);
        if(ui->moduleTableWidget->itemAt(localPos) != nullptr){
            ui->modifyModuleBtn->setEnabled(true);
            ui->deleteModuleBtn->setEnabled(true);
        }
        else{
            ui->modifyModuleBtn->setEnabled(false);
            ui->deleteModuleBtn->setEnabled(false);
        }
    }
    else if(watched == ui->dataFrameTableWidget->viewport() && event->type() == QEvent::MouseButtonPress){
        QPoint globalPos = QCursor::pos();
        QPoint localPos = ui->dataFrameTableWidget->viewport()->mapFromGlobal(globalPos);
        if(ui->dataFrameTableWidget->itemAt(localPos) != nullptr){
            ui->modifyDataFrameBtn->setEnabled(true);
            ui->deleteDataFrameBtn->setEnabled(true);
            if(ui->dataFrameTableWidget->itemAt(localPos)->row() > 0){
                ui->moveUpBtn->setEnabled(true);
            }
            else{
                ui->moveUpBtn->setEnabled(false);
            }
            if(ui->dataFrameTableWidget->itemAt(localPos)->row() < ui->dataFrameTableWidget->rowCount() - 1){
                ui->moveDownBtn->setEnabled(true);
            }
            else{
                ui->moveDownBtn->setEnabled(false);
            }
        }
        else{
            ui->modifyDataFrameBtn->setEnabled(false);
            ui->deleteDataFrameBtn->setEnabled(false);
            ui->moveUpBtn->setEnabled(false);
            ui->moveDownBtn->setEnabled(false);
        }
    }
    else if(lineEdit != nullptr && (event->type() == QEvent::KeyRelease)){
        dynamicSetLineEdit(lineEdit);
    }
    return QWidget::eventFilter(watched, event);
}

void BodyFrameCfgWidget::setForm()
{
    ui->id_lineEdit->setText(QString::number(bodyFrameItem.getBodyFrameItemID()));
    ui->arbitrationStepDuration_lineEdit->setText(QString::number(bodyFrameItem.getArbitrationStepDuration()));
    ui->timeCalibrationFactor_lineEdit->setText(QString::number(bodyFrameItem.getTimeCalibrationFactor()));
    ui->messageInterval_lineEdit->setText(QString::number(bodyFrameItem.getMessageInterval()));
    ui->majorVersionNumber_lineEdit->setText(QString::number(bodyFrameItem.getMajorVersionNumber()));
    ui->subVersionNumber_lineEdit->setText(QString::number(bodyFrameItem.getSubVersionNumber()));
    auto dataFrames = bodyFrameItem.getDataFrames();
//    removeTableWidgetItems(ui->dataFrameTableWidget);
//    removeTableWidgetItems(ui->moduleTableWidget);
//    ui->moduleTableWidget->clear();
    for(const std::string &dataFrameId: bodyFrameItem.getDataFramesOrder()){
        addDataFrameToTableWidget(dataFrames.at(dataFrameId));
    }
    for(auto& module : bodyFrameItem.getModules()){
        addModuleToTableWidget(module.second);
    }
}

const BodyFrame &BodyFrameCfgWidget::getBodyFrameItem() const
{
    return bodyFrameItem;
}

void BodyFrameCfgWidget::removeTableWidgetItems(QTableWidget *tableWidget)
{
    // 获取表格的行数和列数
    int rowCount = tableWidget->rowCount();

    // 清除除标题行以外的所有行
    for (int row = rowCount - 1; row > 0; row--) {
        tableWidget->removeRow(row);
    }
}

void BodyFrameCfgWidget::installEventFilter()
{
    ui->id_lineEdit->installEventFilter(this);
    ui->arbitrationStepDuration_lineEdit->installEventFilter(this);
    ui->timeCalibrationFactor_lineEdit->installEventFilter(this);
    ui->messageInterval_lineEdit->installEventFilter(this);
    ui->majorVersionNumber_lineEdit->installEventFilter(this);
    ui->subVersionNumber_lineEdit->installEventFilter(this);
    //ui->moduleTableWidget->viewport->installEventFilter(this);
    ui->moduleTableWidget->viewport()->installEventFilter(this);
    ui->dataFrameTableWidget->viewport()->installEventFilter(this);
}

bool BodyFrameCfgWidget::check(QWidget *widget)
{
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(widget);
    if(lineEdit != nullptr){
        QString input = lineEdit->text();
        if(lineEdit->validator()->validate(input, dummy) == QValidator::Acceptable){
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

bool BodyFrameCfgWidget::addTableItems(QTableWidget *tableWidget, int rowIndex, QTableWidgetItem *firstItem, ...)
{
    int colIndex = 0;
    QTableWidgetItem* currentItem = firstItem;
    va_list args;
    va_start(args, firstItem);
    while(currentItem){
        tableWidget->setItem(rowIndex, colIndex++, currentItem);
        currentItem = va_arg(args, QTableWidgetItem*);
    }
    va_end(args);
    return true;
}

bool BodyFrameCfgWidget::addModuleToTableWidget(const Module& module)
{
    int rowIndex = ui->moduleTableWidget->rowCount();
    ui->moduleTableWidget->insertRow(rowIndex);
    QTableWidgetItem *moduleNumberItem = new QTableWidgetItem(QString::number(module.getModuleNumber()));
    QTableWidgetItem *initialWaitTimeItem = new QTableWidgetItem(QString::number(module.getInitialWaitTime()));
    QTableWidgetItem *moduleNameItem = new QTableWidgetItem(QString::fromStdString(module.getModuleName()));
    moduleNumberItem->setTextAlignment(Qt::AlignCenter);
    initialWaitTimeItem->setTextAlignment(Qt::AlignCenter);
    moduleNameItem->setTextAlignment(Qt::AlignCenter);
//    ui->moduleTableWidget->setItem(rowIndex, 0, moduleNumberItem);
//    ui->moduleTableWidget->setItem(rowIndex, 1, initialWaitTimeItem);
//    ui->moduleTableWidget->setItem(rowIndex, 2, moduleNameItem);
    addTableItems(ui->moduleTableWidget, rowIndex, moduleNameItem, initialWaitTimeItem, moduleNameItem, nullptr);
    return true;
}

bool BodyFrameCfgWidget::addDataFrameToTableWidget(const DataFrame &dataFrame)
{
    int rowIndex = ui->dataFrameTableWidget->rowCount();
    ui->dataFrameTableWidget->insertRow(rowIndex);
    QTableWidgetItem* dataFrameIdentificationItem = nullptr;
    (dataFrameIdentificationItem = new QTableWidgetItem(QString::fromStdString(dataFrame.getFrameIdentification())))->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem* dataFrameTotalWindowItem = nullptr;
    (dataFrameTotalWindowItem = new QTableWidgetItem(QString::number(dataFrame.getTotalWindow())))->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem* dataFramePeriodItem = nullptr;
    (dataFramePeriodItem = new QTableWidgetItem(QString::number(dataFrame.getFramePeriod())))->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem* dataFrameIdleWayItem = nullptr;
    (dataFrameIdleWayItem = new QTableWidgetItem("不知道是什么东西"))->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem* dataFrameChildFramesItem = nullptr;
    (dataFrameChildFramesItem = new QTableWidgetItem("不知道啥*2"))->setTextAlignment(Qt::AlignCenter);
    addTableItems(ui->dataFrameTableWidget, rowIndex, dataFrameIdentificationItem, dataFrameTotalWindowItem, dataFramePeriodItem, dataFrameIdleWayItem,
                  dataFrameChildFramesItem, nullptr);
    return true;
}

bool BodyFrameCfgWidget::exchangeDataFrameItemOrder(int row1, int row2)
{
    for (int column = 0; column < ui->dataFrameTableWidget->columnCount(); ++column) {
        QTableWidgetItem* item1 = ui->dataFrameTableWidget->takeItem(row1, column);
        QTableWidgetItem* item2 = ui->dataFrameTableWidget->takeItem(row2, column);

        // 将单元格项设置回表格中的另一个行
        ui->dataFrameTableWidget->setItem(row1, column, item2);
        ui->dataFrameTableWidget->setItem(row2, column, item1);
    }
    bodyFrameItem.changeDataFramesOrder(row1, row2);
    return true;
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

void BodyFrameCfgWidget::initWidget()
{
    ui->tabWidget->setCurrentWidget(ui->tabWidget);
    ui->id_lineEdit->setEnabled(false);
    ui->modifyModuleBtn->setEnabled(false);
    ui->deleteModuleBtn->setEnabled(false);
    ui->modifyDataFrameBtn->setEnabled(false);
    ui->deleteDataFrameBtn->setEnabled(false);
    ui->moveUpBtn->setEnabled(false);
    ui->moveDownBtn->setEnabled(false);
    this->setWindowTitle(tr("机架配置"));
    QStringList headerList;
    headerList << tr("模块号") << tr("名称") << tr("初始化等待时间");
    setStdTableHeader(ui->moduleTableWidget, headerList);
    headerList.clear();
    headerList << tr("帧标识") << tr("窗口数") << tr("帧周期") << tr("空闲方式") << tr("子帧");
    setStdTableHeader(ui->dataFrameTableWidget, headerList);
}







