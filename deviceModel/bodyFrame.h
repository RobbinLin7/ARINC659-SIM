#ifndef BODYFRAME_H
#define BODYFRAME_H

#include <QWidget>

#include <QMap>
#include <QTableWidget>
#include <QCloseEvent>
#include <QMessageBox>
#include "hardwareModel.h"
#include "dataFrame.h"

/**
 *  机架类描述
 */

namespace Ui {
class BodyFrame;
}

class BodyFrame : public QWidget
{
    Q_OBJECT

public:
    explicit BodyFrame(QWidget *parent = nullptr, QWidget *paraConfigWidget = nullptr);
    ~BodyFrame();

    void setBodyFrameID(const uint &id);

private slots:
    void on_addModelBtn_clicked();

    void on_newFrameBtn_clicked();


protected:
    void closeEvent(QCloseEvent *e);

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


private:

    void setStdTableHeader(QTableWidget *widget, const QStringList &headerList);

signals:

    void saveFrame();

};

#endif // BODYFRAME_H
