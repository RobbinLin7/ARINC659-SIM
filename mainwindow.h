#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "deviceModel/bodyFrameCfgWidget.h"
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
    void on_actionNewBodyFrameItem_triggered();

//    //void showBodyFrameCfgSlot();

    void on_actionOpenMonitor_triggered();

//    void on_actionOpenCMDTable_triggered();

//    void on_actionStartSim_triggered();

//    void on_actionBurnToFPGA_triggered();

    void on_actionNewProject_triggered();

    void on_actionOpenProject_triggered();

    void addNewProjectSlot(QString name, QString info);

    void saveBodyFrameItemSlot(const BodyFrameItem&);

    void updateBodyFrameSlot(const BodyFrameItem&);

    void changeStyleSheetSlot(QString);

    void cfgBodyFrameItemSlot(uint);

    void deleteBodyFrameItemSlot(uint);

    void on_actionChangeStyleSheet_triggered();

    void onProjectItemPressed(QTreeWidgetItem *item, int column);

    void onProjectItemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow *ui;

    DeviceModelScene *scene;

    uint bodyFrameNum;

    QMap<uint, std::shared_ptr<BodyFrameCfgWidget>> currentBodyFrameList;

    QMap<uint, std::shared_ptr<BodyFrameGraphicsItem>> bodyFrameGraphicsItems;

    QVBoxLayout *layout;

    std::shared_ptr<BodyFrameCfgWidget> bodyFrame;

    void initMainWindow();

    void addLogToDockWidget(const QString log);

    void disableAllActionNeedAProject();

    void enableAllActionNeedAProject();

    BodyFrameItem getBodyFrameItem();

    MyXml myXml;

    QAction *test = nullptr;

    QMap<QString, Proj659*> projectMap;

    std::shared_ptr<Proj659> currentProject = nullptr;

};
#endif // MAINWINDOW_H
