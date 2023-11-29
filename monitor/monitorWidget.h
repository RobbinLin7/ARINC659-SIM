#ifndef MONITORWIDGET_H
#define MONITORWIDGET_H

#include <QWidget>
#include "qcustomplot.h"

namespace Ui {
class MonitorWidget;
}

class MonitorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MonitorWidget(QWidget *parent = nullptr);
    ~MonitorWidget();

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

    void initD0_D1(QCustomPlot*);

private:
    void setAxData();

    void setAyData();

    void setBxData();

    void setByData();
};

#endif // MONITORWIDGET_H
