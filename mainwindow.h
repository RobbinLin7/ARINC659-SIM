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
#include "tools/commandfile.h"
#include "deviceModel/commandfilewidget.h"
#include "deviceModel/innerbodyframescene.h"

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
    BodyFrame getItem(){
        BodyFrame item;
        return item;
    }
    void forTest();
    void createNewInnerBodyFrameScene(uint bodyFrameId);


private slots:
    void on_actionNewBodyFrameItem_triggered();

//    //void showBodyFrameCfgSlot();

    void on_actionOpenMonitor_triggered();

//    void on_actionOpenCMDTable_triggered();

//    void on_actionStartSim_triggered();

    void on_actionBurnToFPGA_triggered();

    void on_actionNewProject_triggered();

    void on_actionOpenProject_triggered();

    void on_actionSaveProject_triggered();

    void addNewProjectSlot(QString name, QString info);

    void saveBodyFrameItemSlot(const BodyFrame&);

    void updateBodyFrameSlot(const BodyFrame&);

    void changeStyleSheetSlot(QString);

    void cfgBodyFrameItemSlot(uint);

    void deleteBodyFrameItemSlot(uint);

    void on_actionChangeStyleSheet_triggered();

    void on_actionCreateCMDTable_triggered();

    void on_actionCompileCMDTable_triggered();

    void on_actionStartSim_triggered();

    void on_actionAbortSim_triggered();

    void onProjectItemPressed(QTreeWidgetItem *item, int column);

    void onProjectItemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow *ui;

    std::shared_ptr<DeviceModelScene> scene;

    uint bodyFrameNum;

    QMap<uint, std::shared_ptr<BodyFrameCfgWidget>> currentBodyFrameList;

    //QMap<uint, std::shared_ptr<BodyFrameGraphicsItem>> bodyFrameGraphicsItems;

    //QMap<uint, std::shared_ptr<InnerBodyFrameScene>> bodyFrameScenes;

    QVBoxLayout *layout;

    std::shared_ptr<QWidget> currentWidget = nullptr;

    std::shared_ptr<BodyFrameCfgWidget> bodyFrameCfgWidget = nullptr;

    std::shared_ptr<CommandFileWidget> commandFileWidget = nullptr;

    void initMainWindow();

    void addLogToDockWidget(const QString log);

    void disableAllActionNeedAProject();

    void enableAllActionNeedAProject();

    void addCommandFileToFileTree();

    QTreeWidgetItem* createProjectTree(QString);

    void createNewScene();

    //void createNewInnerBodyFrameScene(BodyFrame& bodyFrameItem);

    QString createBatchFile();

    QAction *test = nullptr;

    QMap<QString, Proj659*> projectMap;

    std::shared_ptr<Proj659> currentProject = nullptr;

    std::shared_ptr<InnerBodyFrameScene> innerBodyFrameScene = nullptr;

    bool checkWhetherNeedToShowSaveWarningDialog();

    int showSaveWarningDialog();

    // QWidget interface
protected:
    //void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
