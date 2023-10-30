#ifndef DATAFRAMECFGWIDGET_H
#define DATAFRAMECFGWIDGET_H

#include <QWidget>
#include <QValidator>
#include <QTableWidgetItem>
#include "bodyFrameGraphicsItem.h"
#include "validator/stringidvalidator.h"
#include "dialog/datatransferwindowcfgdialog.h"
#include "dialog/frameswitchwindowcfgdialog.h"
#include "dialog/freewindowcfgdialog.h"
#include "dialog/interruptionwindowcfgdialog.h"
#include "dialog/jumpwindowcfgdialog.h"
#include "dialog/longsyncwindowcfgdialog.h"
#include "dialog/stylesheetdialog.h"
#include "dialog/subframecallwindowdialog.h"
#include "dialog/versiondetectionwindowcfgdialog.h"

namespace Ui {
class DataFrameCfgWidget;
}

class DataFrameCfgWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DataFrameCfgWidget(const BodyFrame&, const std::map<std::string, DataFrame>& dataframes, QWidget *parent = nullptr);
    explicit DataFrameCfgWidget(const BodyFrame&, const DataFrame& dataFrame, const std::map<std::string, DataFrame>& dataframes, QWidget *parent = nullptr);
    struct TableItem{
        int index;
        QTableWidgetItem* item;
    };

    ~DataFrameCfgWidget();

private:
    Ui::DataFrameCfgWidget *ui;
    QValidator* framePeriodValidator = nullptr;
    //QValidator* frameIdentificationValidator = nullptr;
    std::map<std::string, DataFrame> dataframes;
    StringIdValidator frameIdentificationValidator;
    DataFrame dataFrame;
    const BodyFrame bodyFrame;
    int dummy = 0;
    void installValidator();
    bool check(QWidget*);
    void setForm();
    void addWindow(const FrameWindow&);
    bool addTableItems(QTableWidget* tableWidget, int rowIndex, TableItem *item, ...);
    bool addTableItems(QTableWidget* tableWidget, int rowIndex, QTableWidgetItem *item, ...);
    WindowCfgDialog* newWindowCfgDialog();
    void changeOrderOfTwoRows(int row1, int row2);

private slots:
    void checkLineEditText();
    void on_okPushButton_clicked(bool);
    void on_cancelPushButton_clicked(bool);
    void on_dataTransferPushButton_clicked(bool);
    void on_interruptionPushButton_clicked(bool);
    void on_longSyncPushButton_clicked(bool);
    void on_jumpPushButton_clicked(bool);
    void on_versionDetectionPushButton_clicked(bool);
    void on_subFrameCallPushButton_clicked(bool);
    void on_freePushButton_clicked(bool);
    void on_frameSwitchPushButton_clicked(bool);
    void on_shortSyncPushButton_clicked(bool);
    void on_editWindowPushButton_clicked(bool);
    void on_deleteWindowPushButton_clicked(bool);
    void on_moveUpWindowPushButton_clicked(bool);
    void on_moveDownWindowPushButton_clicked(bool);
    void addNewWindow(const FrameWindow& frameWindow);


signals:
    void saveDataFrameSignal(const DataFrame&);

    // QObject interface
public:
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // DATAFRAMECFGWIDGET_H
