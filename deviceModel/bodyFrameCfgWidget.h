#ifndef BODYFRAME_H
#define BODYFRAME_H

#include <QWidget>

#include <QMap>
#include <QTableWidget>
#include <QCloseEvent>
#include <QMessageBox>
#include <QValidator>
#include "hardwareModel.h"
#include "dataFrame.h"

/**
 *  机架类描述
 */

namespace Ui {
class BodyFrame;
}

class BodyFrameCfgWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BodyFrameCfgWidget(QWidget *parent = nullptr, QWidget *paraConfigWidget = nullptr);
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
    Ui::BodyFrame *ui;

    uint myBodyFrameID;

    //相关的机架属性

    //保存模块
    //   模块ID   模块信息
    QMap<uint, HardwareModel*> hardwareModelList;

    //保存帧
    //帧标识  帧信息
    QMap<QString, DataFrame*> dataFrameList;

    //参数配置界面
    QWidget* paraConfigWidget;

    bool ok = false;

    QIntValidator* validator;

    //安装事件过滤器
    void installEventFilter();

    //安装验证器
    void installValidator();


private:

    void setStdTableHeader(QTableWidget *widget, const QStringList &headerList);

signals:

    void saveFrameSignal();

    void updateFrameSignal();




    // QObject interface
public:

};

#endif // BODYFRAME_H
