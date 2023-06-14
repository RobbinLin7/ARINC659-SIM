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
            ui->axChart->graph(0)->addData(key, 1);
        }
        else
        {
            ui->axChart->graph(0)->addData(key, 0);
        }
      //ui->axChart->graph(0)->addData(key, qSin(key)+std::rand()/(double)RAND_MAX*1*qSin(key/0.3843));

      //ui->axChart->graph(1)->addData(key, qCos(key)+std::rand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
      // rescale value (vertical) axis to fit the current data:

      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->axChart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->axChart->replot();

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
      //ui->ayChart->graph(0)->addData(key, qSin(key)+std::rand()/(double)RAND_MAX*1*qSin(key/0.3843));
        if(qCos(key)+std::rand()/(double)RAND_MAX*0.5*qSin(key/0.6364) > 0)
        {
            ui->ayChart->graph(1)->addData(key, -1);
        }
        else
        {
            ui->ayChart->graph(1)->addData(key, 1);
        }
      //ui->ayChart->graph(1)->addData(key, qCos(key)+std::rand()/(double)RAND_MAX*0.5*qSin(key/0.6364));
      // rescale value (vertical) axis to fit the current data:

      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->ayChart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->ayChart->replot();

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
            ui->bxChart->graph(0)->addData(key, 0);
        }
        else
        {
            ui->bxChart->graph(0)->addData(key, 1);
        }
      //ui->bxChart->graph(0)->addData(key, qSin(key)+std::rand()/(double)RAND_MAX*1*qSin(key/0.2843));
      //ui->bxChart->graph(1)->addData(key, qCos(key)+std::rand()/(double)RAND_MAX*0.5*qSin(key/0.5364));
      // rescale value (vertical) axis to fit the current data:

      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->bxChart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->bxChart->replot();

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
            ui->byChart->graph(0)->addData(key, 1);
        }
        else
        {
            ui->byChart->graph(0)->addData(key, 0);
        }
      //ui->byChart->graph(0)->addData(key, qSin(key)+std::rand()/(double)RAND_MAX*1*qSin(key/0.4843));
      //ui->byChart->graph(1)->addData(key, qCos(key)+std::rand()/(double)RAND_MAX*0.5*qSin(key/0.3364));
      // rescale value (vertical) axis to fit the current data:

      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->byChart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->byChart->replot();

    // calculate frames per second:
//    static double lastFpsKey;
//    static int frameCount;
//    ++frameCount;
}

void MonitorWidget::setAxData()
{
    ui->axChart->addGraph(); // blue line
    ui->axChart->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->axChart->addGraph(); // red line
    ui->axChart->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->axChart->xAxis->setTicker(timeTicker);
    ui->axChart->axisRect()->setupFullAxesBox();
    ui->axChart->yAxis->setRange(-1.2, 1.2);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->axChart->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->axChart->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->axChart->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->axChart->yAxis2, SLOT(setRange(QCPRange)));


}

void MonitorWidget::setAyData()
{
    ui->ayChart->addGraph(); // blue line
    ui->ayChart->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->ayChart->addGraph(); // red line
    ui->ayChart->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->ayChart->xAxis->setTicker(timeTicker);
    ui->ayChart->axisRect()->setupFullAxesBox();
    ui->ayChart->yAxis->setRange(-1.2, 1.2);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->ayChart->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->ayChart->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->ayChart->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->ayChart->yAxis2, SLOT(setRange(QCPRange)));


}

void MonitorWidget::setBxData()
{
    ui->bxChart->addGraph(); // blue line
    ui->bxChart->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->bxChart->addGraph(); // red line
    ui->bxChart->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->bxChart->xAxis->setTicker(timeTicker);
    ui->bxChart->axisRect()->setupFullAxesBox();
    ui->bxChart->yAxis->setRange(-1.2, 1.2);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->bxChart->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->bxChart->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->bxChart->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->bxChart->yAxis2, SLOT(setRange(QCPRange)));


}

void MonitorWidget::setByData()
{
    ui->byChart->addGraph(); // blue line
    ui->byChart->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->byChart->addGraph(); // red line
    ui->byChart->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->byChart->xAxis->setTicker(timeTicker);
    ui->byChart->axisRect()->setupFullAxesBox();
    ui->byChart->yAxis->setRange(-1.2, 1.2);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->byChart->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->byChart->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->byChart->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->byChart->yAxis2, SLOT(setRange(QCPRange)));


}

void MonitorWidget::on_startBtn_clicked()
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

void MonitorWidget::on_stopBtn_clicked()
{
    dataTimer.stop();
    dataTimer2.stop();
    dataTimer3.stop();
    dataTimer4.stop();
}
