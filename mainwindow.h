﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "deviceModel/bodyFrame.h"
#include "deviceModel/deviceModelScene.h"
#include "monitor/monitorWidget.h"
#include "monitor/qcustomplot.h"
#include "simulation/simulinkwidget.h"
#include "tools/myxml.h"
#include "dialog/stylesheetdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QGraphicsScene;
class QSplitter;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void forTest();


private slots:
    void on_actionNewBodyFrame_triggered();

//    //void showBodyFrameCfgSlot();

//    void on_actionOpenMonitor_triggered();

//    void on_actionOpenCMDTable_triggered();

//    void on_actionStartSim_triggered();

//    void on_actionBurnToFPGA_triggered();

    void on_actionNewProject_triggered();

    void on_actionOpenProject_triggered();

    void addNewProjectSlot(QString name, QString info);

    void saveFrame();

    void changeStyleSheetSlot(QString);

    void on_actionChangeStyleSheet_triggered();

private:
    Ui::MainWindow *ui;

    DeviceModelScene *scene;

    uint bodyFrameNum;

    QMap<uint, BodyFrame*> myBodyFrameList;

    QVBoxLayout *layout;

    void initMainWindow();

    void addLogToDockWidget(const QString log);

    MyXml myXml;  

};
#endif // MAINWINDOW_H
