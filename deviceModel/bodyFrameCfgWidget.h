#ifndef BODYFRAMECFGWIDGET_H
#define BODYFRAMECFGWIDGET_H

#include <QWidget>
#include <QMap>
#include <QTableWidget>
#include <QCloseEvent>
#include <QMessageBox>
#include <QValidator>
#include <QDebug>
#include "modulecfgwidget.h"
#include "dataFrameCfgWidget.h"
#include "bodyFrameGraphicsItem.h"


/**
 *  机架类描述
 */

namespace Ui {
class BodyFrameCfgWidget;
}

class BodyFrameCfgWidget : public QWidget
{
    Q_OBJECT

public:
    enum OpenMode{New, Modified};
    explicit BodyFrameCfgWidget(uint frameId, QWidget *parent = nullptr);
    explicit BodyFrameCfgWidget(const BodyFrameItem &bodyFrameItem, QWidget *parent = nullptr);
    BodyFrameCfgWidget(const BodyFrameCfgWidget&);
    //explicit BodyFrameCfgWidget(BodyFrameItem& bodyFrameItem, QWidget *parent = nullptr);
    //BodyFrameCfgWidget(const BodyFrameCfgWidget&);
    ~BodyFrameCfgWidget();
    void setBodyFrameID(const uint &id);
    void connectOkButtonToUpdateSignal();

    const BodyFrameItem &getBodyFrameItem() const;

private slots:


protected:
    void closeEvent(QCloseEvent *e);
    //void mouseReleaseEvent(QObject* sender, QMouseEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
private:
    Ui::BodyFrameCfgWidget *ui;

    OpenMode openMode;

    BodyFrameItem bodyFrameItem;



    int dummy = 0;

    uint myBodyFrameID;

    //相关的机架属性

    //保存模块
    //   模块ID   模块信息
    QMap<uint, ModuleCfgWidget*> hardwareModelList;

    //保存帧
    //帧标识  帧信息
    QMap<QString, DataFrameCfgWidget*> dataFrameList;

    //参数配置界面
    QWidget* paraConfigWidget;

    bool ok = false;

    QIntValidator* idValidator = nullptr;

    QIntValidator* arbitrationStepDurationValidator = nullptr;

    QIntValidator* timeCalibrationFactorValidator = nullptr;

    QIntValidator* messageIntervalValidator = nullptr;

    QRegExpValidator* majorAndSubVersionNumerValidator = nullptr;

    //安装事件过滤器
    void installEventFilter();

    //安装验证器
    void installValidator();

    void updateBodyFrameItem();

    void dynamicSetLineEdit(QLineEdit* lineEdit);

    void initWidget(const BodyFrameItem& bodyFrameItem);

    void initWidget();

private:
    std::shared_ptr<Module> module = nullptr;
    void setStdTableHeader(QTableWidget *widget, const QStringList &headerList);
    bool check(QWidget *widget);

    bool addTableItems(QTableWidget* tableWidget, int rowIndex, QTableWidgetItem *item, ...);

    bool exchangeDataFrameItemOrder(int row1, int row2);
signals:

    void saveBodyFrameItemSignal(BodyFrameItem&);

    void updateBodyFrameItemSignal(const BodyFrameItem&);

private slots:
    void addModuleSlot();
    void addDataFrameSlot(const DataFrame&);
    void modifyModuleSlot(const Module&);
    void modifyDataFrameSlot(const DataFrame&);
    void on_modifyModuleBtn_clicked(bool);
    void on_deleteModuleBtn_clicked(bool);
    void on_addModuleBtn_clicked();
    void on_addDataFrameBtn_clicked();
    void on_deleteDataFrameBtn_clicked();
    void on_modifyDataFrameBtn_clicked();
    void on_okButton_clicked(bool);
    void on_cancelButton_clicked(bool);
    void on_moveUpBtn_clicked();
    void on_moveDownBtn_clicked();

};

#endif // BODYFRAME_H
