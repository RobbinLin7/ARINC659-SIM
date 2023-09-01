#ifndef BODYFRAMECFGWIDGET_H
#define BODYFRAMECFGWIDGET_H

#include <QWidget>

#include <QMap>
#include <QTableWidget>
#include <QCloseEvent>
#include <QMessageBox>
#include <QValidator>
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
    explicit BodyFrameCfgWidget(uint frameId, QWidget *parent = nullptr);
    ~BodyFrameCfgWidget();

    void setBodyFrameID(const uint &id);

    void connectOkButtonToUpdateSignal();

private slots:
    void on_addModelBtn_clicked();

    void on_newFrameBtn_clicked();

protected:
    void closeEvent(QCloseEvent *e);
    //void mouseReleaseEvent(QObject* sender, QMouseEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
private:
    Ui::BodyFrameCfgWidget *ui;

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


private:

    void setStdTableHeader(QTableWidget *widget, const QStringList &headerList);

signals:

    void saveFrameItemSignal(BodyFrameItem);

    void updateFrameSignal();

    // QObject interface
public:

};

#endif // BODYFRAME_H
