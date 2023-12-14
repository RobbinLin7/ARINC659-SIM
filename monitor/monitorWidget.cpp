#include "monitorWidget.h"
#include "ui_monitorWidget.h"
#include <fstream>

MonitorWidget::MonitorWidget(const LRMGraphicsItem& lrmGraphicsItem, const DataFrames& dataFrames, QWidget *parent):
    QWidget(parent),
    ui(new Ui::MonitorWidget),
    lrmGraphicsItem(lrmGraphicsItem),
    ax(lrmGraphicsItem.getToAx().getBus()),
    ay(lrmGraphicsItem.getToAy().getBus()),
    bx(lrmGraphicsItem.getToBx().getBus()),
    by(lrmGraphicsItem.getToBy().getBus()),
    dataFrames(dataFrames)

{
    ui->setupUi(this);
    this->setWindowTitle(tr("数据监视器"));
    ui->label->setText(QString("%1").arg(QString::fromStdString(lrmGraphicsItem.getModule().getModuleName())));
    setAxData();
    setAyData();
    setBxData();
    setByData();
    if(dataFrames.size() > 0){
        FrameWindow window = *dataFrames.begin()->getFrameWindows().begin();
        std::ifstream ifs(window.getDataSourceFile());
        if(ifs){
            ifs.seekg(0,std::ios::end);
            fileLen = ifs.tellg();
            ifs.seekg(0, std::ios::beg);
            ifs.read(buffer, fileLen);
            ifs.close();
        }
    }
}

MonitorWidget::~MonitorWidget()
{
    delete ui;
}

void MonitorWidget::realtimeDataSlot()
{
    int value;
    const DataFrame& dataFrame = *dataFrames.begin();
    auto period = 120;
    if(tickCnt % period == 0){
        //新周期的开始，更新所有的窗口的finished参数
        std::for_each(dataFrame.getFrameWindows().begin(), dataFrame.getFrameWindows().end(), [&](const FrameWindow& frameWindow){
            frameWindow.setFinished(false);
        });
    }
    //DataFrame dataFrame;
    for(auto& window: dataFrame.getFrameWindows()){
        if(tickCnt % period < 100){

            //1000应该是一个和window有关的参数，这里只是为了调试先写死
            switch(window.getWindowType()){
            case FrameWindow::DATA_SEND:
                if(window.getFinished() == true){
                    value = 0;
                }
                else{
                    if(((buffer[ptr_byte] >> (7 - ptr_bit)) & 1) > 0){
                        value = 1;
                    }
                    else{
                        value = 0;
                    }
                    ptr_bit = (ptr_bit + 1) % 8;
                    if(ptr_bit == 0){
                        ptr_byte = (ptr_byte + 1) % fileLen;
                    }
                    if(ptr_byte == 0 && ptr_bit == 0){
                        window.setFinished(true);
                    }
                    break;
                }
            default:
                break;
            }
        }
    }
    ++tickCnt;
    static QTime timeStart = QTime::currentTime();
    // calculate two new data points:
    double key = timeStart.msecsTo(QTime::currentTime())/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;

    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
        // add data to lines:
//          if(qSin(key)+std::rand()/(double)RAND_MAX*1*qSin(key/0.3843) > 0)
//          {
//              value = 1;
//          }
//          else
//          {
//              value = 0;
//          }
          ui->ax_d0_chart->graph(0)->addData(key, value);
          ui->ax_d1_chart->graph(0)->addData(key, value);

          ui->ay_d0_chart->graph(0)->addData(key, value);
          ui->ay_d1_chart->graph(0)->addData(key, value);

          ui->bx_d0_chart->graph(0)->addData(key, value);
          ui->bx_d1_chart->graph(0)->addData(key, value);

          ui->by_d0_chart->graph(0)->addData(key, value);
          ui->by_d1_chart->graph(0)->addData(key, value);


        //ui->ax_d0_chart->graph(0)->addData(key, qSin(key)+std::rand()/(double)RAND_MAX*1*qSin(key/0.3843));

        //ui->ax_d0_chart->graph(1)->addData(key, qCos(key)+std::rand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
        // rescale value (vertical) axis to fit the current data:

        lastPointKey = key;
        emit sendData(value);
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->ax_d0_chart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->ax_d0_chart->replot();

    ui->ax_d1_chart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->ax_d1_chart->replot();

    ui->ay_d0_chart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->ay_d0_chart->replot();

    ui->ay_d1_chart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->ay_d1_chart->replot();

    ui->bx_d0_chart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->bx_d0_chart->replot();

    ui->bx_d1_chart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->bx_d1_chart->replot();

    ui->by_d0_chart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->by_d0_chart->replot();

    ui->by_d1_chart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->by_d1_chart->replot();



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
    if(type == SEND){
        //如果是数据发送方，则自己产生数据
        connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
        dataTimer.start(500); // Interval 0 means to refresh as fast as possible
    }
    else if(type == RECEIVE){
        //如果是接收方，则接受发送方产生的数据

    }
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

void MonitorWidget::setType(Type newType)
{
    type = newType;
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
    //timeTicker->setTimeFormat()
    //timeTicker->setTimeFormat("%h:%m:%s");
    chart->xAxis->setTicker(timeTicker);
    //ui->ax_d0_chart->axisRect()->setupFullAxesBox();
    chart->yAxis->setRange(0, 1);
    chart->yAxis->setTicker(ticker);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(chart->xAxis, SIGNAL(rangeChanged(QCPRange)), chart->xAxis2, SLOT(setRange(QCPRange)));
    connect(chart->yAxis, SIGNAL(rangeChanged(QCPRange)), chart->yAxis2, SLOT(setRange(QCPRange)));
}

void MonitorWidget::receiveData(int value)
{
    static QTime timeStart = QTime::currentTime();
    // calculate two new data points:
    double key = timeStart.msecsTo(QTime::currentTime())/1000.0; // time elapsed since start of demo, in seconds
    if(ax->getError() == false){
        ui->ax_d0_chart->graph(0)->addData(key, value);
        ui->ax_d1_chart->graph(0)->addData(key, value);
    }
    else{
        if(ax->getErrorType() == FaultInjectDialog::D0_ERROR){
            ui->ax_d0_chart->graph(0)->addData(key, 0);
        }
        else{
            ui->ax_d0_chart->graph(0)->addData(key, value);
        }
        if(ax->getErrorType() == FaultInjectDialog::D1_ERROR){
            ui->ax_d1_chart->graph(0)->addData(key, 0);
        }
        else{
            ui->ax_d1_chart->graph(0)->addData(key, value);
        }
    }

    if(ay->getError() == false){
        ui->ay_d0_chart->graph(0)->addData(key, value);
        ui->ay_d1_chart->graph(0)->addData(key, value);
    }
    else{
        if(ay->getErrorType() == FaultInjectDialog::D0_ERROR){
            ui->ay_d0_chart->graph(0)->addData(key, 0);
            ui->ay_d1_chart->graph(0)->addData(key, value);
        }
        else{
            ui->ay_d0_chart->graph(0)->addData(key, value);
            ui->ay_d1_chart->graph(0)->addData(key, 0);
        }

    }
    if(bx->getError() == false){
        ui->bx_d0_chart->graph(0)->addData(key, value);
        ui->bx_d1_chart->graph(0)->addData(key, value);
    }
    else{
        if(bx->getErrorType() == FaultInjectDialog::D0_ERROR){
            ui->bx_d0_chart->graph(0)->addData(key, 0);
            ui->bx_d1_chart->graph(0)->addData(key, value);
        }
        else{
            ui->bx_d0_chart->graph(0)->addData(key, value);
            ui->bx_d1_chart->graph(0)->addData(key, 0);
        }
    }

    if(by->getError() == false){
        ui->by_d0_chart->graph(0)->addData(key, value);
        ui->by_d1_chart->graph(0)->addData(key, value);
    }
    else{
        if(by->getErrorType() == FaultInjectDialog::D0_ERROR){
            ui->by_d0_chart->graph(0)->addData(key, 0);
            ui->by_d1_chart->graph(0)->addData(key, value);
        }
        else{
            ui->by_d0_chart->graph(0)->addData(key, value);
            ui->by_d1_chart->graph(0)->addData(key, 0);
        }
    }

    ui->ax_d0_chart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->ax_d0_chart->replot();

    ui->ax_d1_chart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->ax_d1_chart->replot();

    ui->ay_d0_chart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->ay_d0_chart->replot();

    ui->ay_d1_chart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->ay_d1_chart->replot();

    ui->bx_d0_chart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->bx_d0_chart->replot();

    ui->bx_d1_chart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->bx_d1_chart->replot();

    ui->by_d0_chart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->by_d0_chart->replot();

    ui->by_d1_chart->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->by_d1_chart->replot();

}
