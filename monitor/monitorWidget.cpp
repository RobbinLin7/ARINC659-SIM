#include "monitorWidget.h"
#include "ui_monitorWidget.h"

MonitorWidget::MonitorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MonitorWidget)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("数据监视器"));
    setAxData();
    setAyData();
    setBxData();
    setByData();
}

MonitorWidget::~MonitorWidget()
{
    delete ui;
}

void MonitorWidget::realtimeDataSlot()
{
    static QTime timeStart = QTime::currentTime();
    // calculate two new data points:
    double key = timeStart.msecsTo(QTime::currentTime())/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
        if(qSin(key)+std::rand()/(double)RAND_MAX*1*qSin(key/0.3843) > 0)
        {
            ui->ax_d0_chart->graph(0)->addData(key, 1);
        }
        else
        {
            ui->ax_d0_chart->graph(0)->addData(key, 0);
        }
      //ui->ax_d0_chart->graph(0)->addData(key, qSin(key)+std::rand()/(double)RAND_MAX*1*qSin(key/0.3843));

      //ui->ax_d0_chart->graph(1)->addData(key, qCos(key)+std::rand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
      // rescale value (vertical) axis to fit the current data:

      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->ax_d0_chart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->ax_d0_chart->replot();

    // calculate frames per second:
//    static double lastFpsKey;
//    static int frameCount;
//    ++frameCount;

}

void MonitorWidget::realtimeDataSlot2()
{
    static QTime timeStart = QTime::currentTime();
    // calculate two new data points:
    double key = timeStart.msecsTo(QTime::currentTime())/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
      //ui->ay_d0_chart->graph(0)->addData(key, qSin(key)+std::rand()/(double)RAND_MAX*1*qSin(key/0.3843));
        if(qCos(key)+std::rand()/(double)RAND_MAX*0.5*qSin(key/0.6364) > 0)
        {
            ui->ay_d0_chart->graph(1)->addData(key, -1);
        }
        else
        {
            ui->ay_d0_chart->graph(1)->addData(key, 1);
        }
      //ui->ay_d0_chart->graph(1)->addData(key, qCos(key)+std::rand()/(double)RAND_MAX*0.5*qSin(key/0.6364));
      // rescale value (vertical) axis to fit the current data:

      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->ay_d0_chart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->ay_d0_chart->replot();

    // calculate frames per second:
//    static double lastFpsKey;
//    static int frameCount;
//    ++frameCount;
}

void MonitorWidget::realtimeDataSlot3()
{
    static QTime timeStart = QTime::currentTime();
    // calculate two new data points:
    double key = timeStart.msecsTo(QTime::currentTime())/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
        if(qSin(key)+std::rand()/(double)RAND_MAX*1*qSin(key/0.2843) > 0)
        {
            ui->bx_d0_chart->graph(0)->addData(key, 0);
        }
        else
        {
            ui->bx_d0_chart->graph(0)->addData(key, 1);
        }
      //ui->bx_d0_chart->graph(0)->addData(key, qSin(key)+std::rand()/(double)RAND_MAX*1*qSin(key/0.2843));
      //ui->bx_d0_chart->graph(1)->addData(key, qCos(key)+std::rand()/(double)RAND_MAX*0.5*qSin(key/0.5364));
      // rescale value (vertical) axis to fit the current data:

      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->bx_d0_chart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->bx_d0_chart->replot();

    // calculate frames per second:
//    static double lastFpsKey;
//    static int frameCount;
//    ++frameCount;
}

void MonitorWidget::realtimeDataSlot4()
{
    static QTime timeStart = QTime::currentTime();
    // calculate two new data points:
    double key = timeStart.msecsTo(QTime::currentTime())/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
        if(qSin(key)+std::rand()/(double)RAND_MAX*1*qSin(key/0.4843) > 0)
        {
            ui->by_d0_chart->graph(0)->addData(key, 1);
        }
        else
        {
            ui->by_d0_chart->graph(0)->addData(key, 0);
        }
      //ui->by_d0_chart->graph(0)->addData(key, qSin(key)+std::rand()/(double)RAND_MAX*1*qSin(key/0.4843));
      //ui->by_d0_chart->graph(1)->addData(key, qCos(key)+std::rand()/(double)RAND_MAX*0.5*qSin(key/0.3364));
      // rescale value (vertical) axis to fit the current data:

      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->by_d0_chart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->by_d0_chart->replot();

    // calculate frames per second:
//    static double lastFpsKey;
//    static int frameCount;
    //    ++frameCount;
}

void MonitorWidget::on_actionWatchStart_triggered()
{
    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer.start(0); // Interval 0 means to refresh as fast as possible

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer2, SIGNAL(timeout()), this, SLOT(realtimeDataSlot2()));
    dataTimer2.start(0); // Interval 0 means to refresh as fast as possible

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer3, SIGNAL(timeout()), this, SLOT(realtimeDataSlot3()));
    dataTimer3.start(0); // Interval 0 means to refresh as fast as possible

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer4, SIGNAL(timeout()), this, SLOT(realtimeDataSlot4()));
    dataTimer4.start(0); // Interval 0 means to refresh as fast as possible
}

void MonitorWidget::on_actionWatchEnd_triggered()
{
    dataTimer.stop();
    dataTimer2.stop();
    dataTimer3.stop();
    dataTimer4.stop();
}

void MonitorWidget::setAxData()
{
//    ui->ax_d0_chart->addGraph(); // blue line
//    ui->ax_d0_chart->graph(0)->setPen(QPen(QColor(40, 110, 255)));
//    ui->ax_d0_chart->addGraph(); // red line
//    ui->ax_d0_chart->graph(1)->setPen(QPen(QColor(255, 110, 40)));

//    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
//    QSharedPointer<QCPAxisTickerFixed> ticker(new QCPAxisTickerFixed);
//    ticker->setTickStep(1);
//    timeTicker->setTimeFormat("%h:%m:%s");
//    ui->ax_d0_chart->xAxis->setTicker(timeTicker);
//    //ui->ax_d0_chart->axisRect()->setupFullAxesBox();
//    ui->ax_d0_chart->yAxis->setRange(0, 1);
//    ui->ax_d0_chart->yAxis->setTicker(ticker);

//    // make left and bottom axes transfer their ranges to right and top axes:
//    connect(ui->ax_d0_chart->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->ax_d0_chart->xAxis2, SLOT(setRange(QCPRange)));
//    connect(ui->ax_d0_chart->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->ax_d0_chart->yAxis2, SLOT(setRange(QCPRange)));

//    ui->ax_d1_chart->yAxis->setRange(0, 1);
    //yAxis->setTickVectorLabels(QStringList() << "0" << "1");
    initD0_D1(ui->ax_d0_chart);
    initD0_D1(ui->ax_d1_chart);
    initD0_D1(ui->ax_clk_chart);

}

void MonitorWidget::setAyData()
{
//    ui->ay_d0_chart->addGraph(); // blue line
//    ui->ay_d0_chart->graph(0)->setPen(QPen(QColor(40, 110, 255)));
//    ui->ay_d0_chart->addGraph(); // red line
//    ui->ay_d0_chart->graph(1)->setPen(QPen(QColor(255, 110, 40)));

//    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
//    timeTicker->setTimeFormat("%h:%m:%s");
//    ui->ay_d0_chart->xAxis->setTicker(timeTicker);
//    ui->ay_d0_chart->axisRect()->setupFullAxesBox();
//    ui->ay_d0_chart->yAxis->setRange(0, 1);

//    // make left and bottom axes transfer their ranges to right and top axes:
//    connect(ui->ay_d0_chart->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->ay_d0_chart->xAxis2, SLOT(setRange(QCPRange)));
//    connect(ui->ay_d0_chart->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->ay_d0_chart->yAxis2, SLOT(setRange(QCPRange)));
    initD0_D1(ui->ay_d0_chart);
    initD0_D1(ui->ay_d1_chart);
    initD0_D1(ui->ay_clk_chart);
}

void MonitorWidget::setBxData()
{
//    ui->bx_d0_chart->addGraph(); // blue line
//    ui->bx_d0_chart->graph(0)->setPen(QPen(QColor(40, 110, 255)));
//    ui->bx_d0_chart->addGraph(); // red line
//    ui->bx_d0_chart->graph(1)->setPen(QPen(QColor(255, 110, 40)));

//    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
//    timeTicker->setTimeFormat("%h:%m:%s");
//    ui->bx_d0_chart->xAxis->setTicker(timeTicker);
//    ui->bx_d0_chart->axisRect()->setupFullAxesBox();
//    ui->bx_d0_chart->yAxis->setRange(0, 1);

//    // make left and bottom axes transfer their ranges to right and top axes:
//    connect(ui->bx_d0_chart->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->bx_d0_chart->xAxis2, SLOT(setRange(QCPRange)));
//    connect(ui->bx_d0_chart->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->bx_d0_chart->yAxis2, SLOT(setRange(QCPRange)));
    initD0_D1(ui->bx_d0_chart);
    initD0_D1(ui->bx_d1_chart);
    initD0_D1(ui->bx_clk_chart);
}

void MonitorWidget::setByData()
{
//    ui->by_d0_chart->addGraph(); // blue line
//    ui->by_d0_chart->graph(0)->setPen(QPen(QColor(40, 110, 255)));
//    ui->by_d0_chart->addGraph(); // red line
//    ui->by_d0_chart->graph(1)->setPen(QPen(QColor(255, 110, 40)));

//    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
//    timeTicker->setTimeFormat("%h:%m:%s");
//    ui->by_d0_chart->xAxis->setTicker(timeTicker);
//    ui->by_d0_chart->axisRect()->setupFullAxesBox();
//    ui->by_d0_chart->yAxis->setRange(0, 1);

//    // make left and bottom axes transfer their ranges to right and top axes:
//    connect(ui->by_d0_chart->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->by_d0_chart->xAxis2, SLOT(setRange(QCPRange)));
//    connect(ui->by_d0_chart->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->by_d0_chart->yAxis2, SLOT(setRange(QCPRange)));
    initD0_D1(ui->by_d0_chart);
    initD0_D1(ui->by_d1_chart);
    initD0_D1(ui->by_clk_chart);
}

void MonitorWidget::on_startBtn_clicked()
{


}

void MonitorWidget::on_stopBtn_clicked()
{
    dataTimer.stop();
    dataTimer2.stop();
    dataTimer3.stop();
    dataTimer4.stop();
}

void MonitorWidget::initD0_D1(QCustomPlot *chart)
{
    chart->addGraph(); // blue line
    chart->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    chart->addGraph(); // red line
    chart->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    QSharedPointer<QCPAxisTickerFixed> ticker(new QCPAxisTickerFixed);
    ticker->setTickStep(1);
    timeTicker->setTimeFormat("%h:%m:%s");
    chart->xAxis->setTicker(timeTicker);
    //ui->ax_d0_chart->axisRect()->setupFullAxesBox();
    chart->yAxis->setRange(0, 1);
    chart->yAxis->setTicker(ticker);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(chart->xAxis, SIGNAL(rangeChanged(QCPRange)), chart->xAxis2, SLOT(setRange(QCPRange)));
    connect(chart->yAxis, SIGNAL(rangeChanged(QCPRange)), chart->yAxis2, SLOT(setRange(QCPRange)));
}
