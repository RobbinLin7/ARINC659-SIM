#ifndef MONITORWIDGET_H
#define MONITORWIDGET_H

#include <QWidget>
#include "qcustomplot.h"
#include "deviceModel/busgraphicsitem.h"

namespace Ui {
class MonitorWidget;
}

class MonitorWidget : public QWidget
{
    Q_OBJECT

public:
    enum Type{SEND, RECEIVE};
    explicit MonitorWidget(const BusGraphicsItem* ax, const BusGraphicsItem* ay, const BusGraphicsItem* bx, const BusGraphicsItem* by, QWidget *parent = nullptr);
    ~MonitorWidget();

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

signals:
    void sendData(int val);
public slots:
    void receiveData(int val);
};

#endif // MONITORWIDGET_H
