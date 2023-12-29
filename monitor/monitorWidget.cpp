#include "monitorWidget.h"
#include "ui_monitorWidget.h"
#include <fstream>

MonitorWidget::MonitorWidget(const LRMGraphicsItem& lrmGraphicsItem, const DataFrames& dataFrames,
                             const QMap<uint, std::shared_ptr<LRMGraphicsItem>>* moduleGraphicItems, QWidget *parent):
    QWidget(parent),
    ui(new Ui::MonitorWidget),
    moduleGraphicItems(moduleGraphicItems),
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
}

MonitorWidget::~MonitorWidget()
{
    delete ui;
}

void MonitorWidget::realtimeDataSlot()
{
    int value;
    int totalPeriod = 0;
    std::for_each(dataFrames.begin(), dataFrames.end(), [&](const DataFrame& dataFrame){
        totalPeriod += dataFrame.getFramePeriod();
    });
    totalPeriod = totalPeriod * 10000000 / 333;
    int sumPeriod = 0;
    for(auto& dataFrame: dataFrames){
        auto period = dataFrame.getFramePeriod() * 10000000 / 333 / 2;
        sumPeriod += period;
        if(tickCnt % totalPeriod == sumPeriod){
            //新周期的开始，更新所有的窗口的finished参数
            std::for_each(dataFrame.getFrameWindows().begin(), dataFrame.getFrameWindows().end(), [&](const FrameWindow& frameWindow){
                frameWindow.setFinished(false);
            });
            continue;
        }
        //DataFrame dataFrame;
        uint32_t sum  = 0;
        bool flag = false;
        for(auto& window: dataFrame.getFrameWindows()){

            if(tickCnt % period < sum + window.getNumOfTimeSlot()){
                //1000应该是一个和window有关的参数，这里只是为了调试先写死
                flag = true;
                switch(window.getWindowType()){
                case FrameWindow::DATA_SEND:
                    if(window.getMainLRM() != lrmGraphicsItem.getModule().getModuleNumber()){
                        continue; //只有发送lrm才发送，其余接收
                    }               
                    ui->label->setText("数据传输窗口");
                    if(window.getFinished() == true){
                        value = 0;
                    }
                    else{
                        if(ifs.is_open() == false){
                            ifs.open(window.getDataSourceFile());
//                            ifs.seekg(0, std::ios::end);
//                            fileLen = ifs.tellg();
//                            ifs.seekg(0, std::ios::beg);
                            ifs.read(buffer, 512);
                            ptr_byte = 0;
                            ptr_bit = 0;
                        }
                        if(ptr_byte == ifs.gcount()){
                            ifs.read(buffer, 512);
                            if(ifs.gcount() == 0 || ifs.eof() == true){
                                //此时表示文件读完了
                                window.setFinished(true);
                                ifs.close();
                                break;
                            }
                            ptr_byte = 0;
                            ptr_bit = 0;
                        }
                        if(((buffer[ptr_byte] >> (7 - ptr_bit)) & 1) > 0){
                            value = 1;
                        }
                        else{
                            value = 0;
                        }
                        ptr_bit = (ptr_bit + 1) % 8;
                        if(ptr_bit == 0){
                            ptr_byte += 1;
                        }
//                        if(ptr_byte == 0 && ptr_bit == 0){
//                            window.setFinished(true);
//                        }
                        break;
                    }
                case FrameWindow::VERSION_SEND:
                    ui->label->setText("版本校验窗口");
                    value = 1;
                    break;
                case FrameWindow::LONG_SYNC:
                    ui->label->setText("长同步窗口");
                    value = 1;
                    break;
                case FrameWindow::FRAME_SWITCH:
                    ui->label->setText("帧切换窗口");
                    break;
                case FrameWindow::CALL_SUBFRAME:
                    ui->label->setText("子帧调用窗口");
                    break;
                case FrameWindow::INT_SEND:
                    ui->label->setText("中断窗口");
                    value = 1;
                    break;
                case FrameWindow::FRAME_JUMP:
                    ui->label->setText("帧跳转窗口");
                    break;
                case FrameWindow::FREE:
                    ui->label->setText("FREE窗口");
                    value = 0;
                    break;
                case FrameWindow::SHORT_SYNC:
                    ui->label->setText("短同步窗口");
                    break;
                default:
                    break;
                }
                break;
            }
            sum += window.getNumOfTimeSlot();
        }
        if(flag == false){
            ui->label->setText("空闲");
        }
    }

    ++tickCnt;
    static QTime timeStart = QTime::currentTime();
    // calculate two new data points:
    double key = timeStart.msecsTo(QTime::currentTime()) / 1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;

    if (key - lastPointKey > 0.002) // at most add point every 2 ms
    {
        ui->ax_d0_chart->graph(0)->addData(tickCnt, value);
        ui->ax_d1_chart->graph(0)->addData(tickCnt, value);
        ui->ax_clk_chart->graph(0)->addData(tickCnt, 1);

        ui->ay_d0_chart->graph(0)->addData(tickCnt, value);
        ui->ay_d1_chart->graph(0)->addData(tickCnt, value);
        ui->ay_clk_chart->graph(0)->addData(tickCnt, 1);

        ui->bx_d0_chart->graph(0)->addData(tickCnt, value);
        ui->bx_d1_chart->graph(0)->addData(tickCnt, value);
        ui->bx_clk_chart->graph(0)->addData(tickCnt, 1);

        ui->by_d0_chart->graph(0)->addData(tickCnt, value);
        ui->by_d1_chart->graph(0)->addData(tickCnt, value);
        ui->by_clk_chart->graph(0)->addData(tickCnt, 1);

        size_t size = 800;
        ui->ax_d0_chart->xAxis->setRange(tickCnt, size, Qt::AlignRight);
        ui->ax_d0_chart->replot();

        ui->ax_d1_chart->xAxis->setRange(tickCnt, size, Qt::AlignRight);
        ui->ax_d1_chart->replot();

        ui->ax_clk_chart->xAxis->setRange(tickCnt, size, Qt::AlignRight);
        ui->ax_clk_chart->replot();

        ui->ay_d0_chart->xAxis->setRange(tickCnt, size, Qt::AlignRight);
        ui->ay_d0_chart->replot();

        ui->ay_d1_chart->xAxis->setRange(tickCnt, size, Qt::AlignRight);
        ui->ay_d1_chart->replot();

        ui->ay_clk_chart->xAxis->setRange(tickCnt, size, Qt::AlignRight);
        ui->ay_clk_chart->replot();

        ui->bx_d0_chart->xAxis->setRange(tickCnt, size, Qt::AlignRight);
        ui->bx_d0_chart->replot();

        ui->bx_d1_chart->xAxis->setRange(tickCnt, size, Qt::AlignRight);
        ui->bx_d1_chart->replot();

        ui->bx_clk_chart->xAxis->setRange(tickCnt, size, Qt::AlignRight);
        ui->bx_clk_chart->replot();

        ui->by_d0_chart->xAxis->setRange(tickCnt, size, Qt::AlignRight);
        ui->by_d0_chart->replot();

        ui->by_d1_chart->xAxis->setRange(tickCnt, size, Qt::AlignRight);
        ui->by_d1_chart->replot();

        ui->by_clk_chart->xAxis->setRange(tickCnt, size, Qt::AlignRight);
        ui->by_clk_chart->replot();

        lastPointKey = key;
        emit sendData(value);
    }
    // make key axis range scroll with the data (at a constant range size of 8):

}

void MonitorWidget::on_actionWatchStart_triggered()
{
    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    if(type == SEND){
        //如果是数据发送方，则自己产生数据
        dataTimer.setTimerType(Qt::PreciseTimer);
        connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
        dataTimer.start(1); // Interval 0 means to refresh as fast as possible
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

    QSharedPointer<QCPAxisTickerFixed> xTicker(new QCPAxisTickerFixed);
    QSharedPointer<QCPAxisTickerFixed> yTicker(new QCPAxisTickerFixed);
    yTicker->setTickStep(1);
    //timeTicker->setTimeFormat()
    //timeTicker->setTimeFormat("%h:%m:%s");
    //timeTicker->setTimeFormat("%s");
    chart->xAxis->setTicker(xTicker);
    xTicker->setTickStep(100);
    chart->xAxis->setLabel("x:单位(位时)");
    chart->yAxis->setLabel("y:单位(电平)");
    chart->xAxis->setObjectName("位置时间");
    //ui->ax_d0_chart->axisRect()->setupFullAxesBox();
    chart->yAxis->setRange(0, 1);
    chart->yAxis->setTicker(yTicker);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(chart->xAxis, SIGNAL(rangeChanged(QCPRange)), chart->xAxis2, SLOT(setRange(QCPRange)));
    connect(chart->yAxis, SIGNAL(rangeChanged(QCPRange)), chart->yAxis2, SLOT(setRange(QCPRange)));
}

void MonitorWidget::receiveData(int value)
{
    //static QTime timeStart = QTime::currentTime();
    // calculate two new data points:
    static int key = 0;
    //double key = timeStart.msecsTo(QTime::currentTime())/1000.0; // time elapsed since start of demo, in seconds
    auto func = [=](decltype (ax) bus, decltype (ui->ax_clk_chart) clk, decltype (ui->ax_d0_chart) d0, decltype (ui->ax_d1_chart) d1){
        if(bus->getError() == false){
            d0->graph(0)->addData(key, value);
            d1->graph(0)->addData(key, value);
            clk->graph(0)->addData(key, 1);
        }
        else{
            if(bus->getErrorType() == FaultInjectDialog::CLK_ERROR){
                d0->graph(0)->addData(key, 0);
                d1->graph(0)->addData(key, 0);
                clk->graph(0)->addData(key, 0);
            }
            else{
                clk->graph(0)->addData(key, 1);
                if(bus->getErrorType() == FaultInjectDialog::D0_ERROR){
                    d0->graph(0)->addData(key, 0);
                }
                else{
                    d0->graph(0)->addData(key, value);
                }
                if(ax->getErrorType() == FaultInjectDialog::D1_ERROR){
                    d1->graph(0)->addData(key, 0);
                }
                else{
                    d1->graph(0)->addData(key, value);
                }
            }
        }
        clk->xAxis->setRange(key, 800, Qt::AlignRight);
        clk->replot();
        d0->xAxis->setRange(key, 800, Qt::AlignRight);
        d0->replot();
        d1->xAxis->setRange(key, 800, Qt::AlignRight);
        d1->replot();
    };
    func(ax, ui->ax_clk_chart, ui->ax_d0_chart, ui->ax_d1_chart);
    func(ay, ui->ay_clk_chart, ui->ay_d0_chart, ui->ay_d1_chart);
    func(bx, ui->bx_clk_chart, ui->bx_d0_chart, ui->bx_d1_chart);
    func(by, ui->by_clk_chart, ui->by_d0_chart, ui->by_d1_chart);
    ++key;

}
