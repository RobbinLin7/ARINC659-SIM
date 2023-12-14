#ifndef MONITORWIDGET_H
#define MONITORWIDGET_H

#include <QWidget>
#include "qcustomplot.h"
#include "deviceModel/busgraphicsitem.h"
class MonitorWidget;
#include "deviceModel/lrmgrphicsitem.h"
#include "data/module.h"
#include "data/dataframe.h"
#include "data/dataframes.h"
#include <fstream>

namespace Ui {
class MonitorWidget;
}

class MonitorWidget : public QWidget
{
    Q_OBJECT

public:
    enum Type{SEND, RECEIVE};
    explicit MonitorWidget(const LRMGraphicsItem& lrmGraphicsItem, const DataFrames& dataFrames, QWidget *parent = nullptr);
    ~MonitorWidget();
    const int maxBufferSize = 512;
    void setType(Type newType);

private slots:
  void realtimeDataSlot();
  void realtimeDataSlot2();
  void realtimeDataSlot3();
  void realtimeDataSlot4();
  void on_actionWatchStart_triggered();
  void on_actionWatchEnd_triggered();
  void on_startBtn_clicked();
  void on_stopBtn_clicked();

private:
    Ui::MonitorWidget *ui;

    QTimer dataTimer;
    QTimer dataTimer2;
    QTimer dataTimer3;
    QTimer dataTimer4;



    const LRMGraphicsItem& lrmGraphicsItem;

    Type type = SEND;

    const BusGraphicsItem* ax = nullptr;
    const BusGraphicsItem* ay = nullptr;
    const BusGraphicsItem* bx = nullptr;
    const BusGraphicsItem* by = nullptr;

private:
    void setAxData();

    void setAyData();

    void setBxData();

    void setByData();

    void initD0_D1(QCustomPlot*);

    const DataFrames& dataFrames;

    unsigned tickCnt = 0;

    char buffer[512];

    unsigned ptr_byte = 0;

    unsigned ptr_bit = 0;

    unsigned fileLen;

    std::ifstream ifs;

signals:
    void sendData(int val);
public slots:
    void receiveData(int val);
};

#endif // MONITORWIDGET_H
