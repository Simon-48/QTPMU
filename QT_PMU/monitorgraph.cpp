#include "monitorgraph.h"
#include "ui_monitorgraph.h"
#include <mainwindow.h>

#include <iostream>
#include <list>

MonitorGraph::MonitorGraph(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MonitorGraph)
{
    ui->setupUi(this);

    setupPlot();

    num = 0;

    // Show all graphs initially
    volt_0 = true;
    volt_1 = true;
    volt_2 = true;

    current_0 = false;
    current_1 = false;
    current_2 = false;




    // Check mark graphs to be monitored
    ui->voltage_0->toggle();
    ui->voltage_1->toggle();
    ui->voltage_2->toggle();

//    ui->current_0->toggle();
//    ui->current_1->toggle();
//    ui->current_2->toggle();



    ui->customPlot->legend->setVisible(true);
}

MonitorGraph::~MonitorGraph()
{
    delete ui;
}

void MonitorGraph::update_datetime()
{
    QString current_time = MainWindow::get_time();
    QString current_date = MainWindow::get_date();

    ui->label_date->setText(current_date);
    ui->label_time->setText(current_time);
}

void MonitorGraph::on_return_home_clicked()
{
    //hide();

    //MainWindow *w = new MainWindow(this);
    //w->show();

    this->hide();

    // Show the MainWindow (i.e. the parent window)
    QWidget *parent = this->parentWidget();
    parent->show();

}

void MonitorGraph::realtimeDataSlot()
{
    static int key = 0;

    update_datetime();

    // Store json packet

    json resultDict = MainWindow::get_channel_data();

    // Convert nlohmann json to QJsonObject
    std::string s = resultDict.dump();
    QString str = QString::fromUtf8(s.c_str());
    QByteArray br = str.toUtf8();
    QJsonDocument doc = QJsonDocument::fromJson(br);
    QJsonObject data = doc.object();


//    int Channels = data.value("Channels").toInt();
//    int bits = data.value("bits").toInt();
//    int Frame = data.value("Frame").toInt();
//    int Fs = data.value("Fs").toInt();
//    int n = data.value("n").toInt();

    QJsonValue channel_0_payload = data.value("Channel_0");
    QJsonObject channel_0_obj = channel_0_payload.toObject();
    QJsonArray channel_0 = channel_0_obj.value("Payload").toArray();

    QJsonValue channel_1_payload = data.value("Channel_1");
    QJsonObject channel_1_obj = channel_1_payload.toObject();
    QJsonArray channel_1 = channel_1_obj.value("Payload").toArray();

    QJsonValue channel_2_payload = data.value("Channel_2");
    QJsonObject channel_2_obj = channel_2_payload.toObject();
    QJsonArray channel_2 = channel_2_obj.value("Payload").toArray();

    QJsonValue channel_3_payload = data.value("Channel_3");
    QJsonObject channel_3_obj = channel_3_payload.toObject();
    QJsonArray channel_3 = channel_3_obj.value("Payload").toArray();

    QJsonValue channel_4_payload = data.value("Channel_4");
    QJsonObject channel_4_obj = channel_4_payload.toObject();
    QJsonArray channel_4 = channel_4_obj.value("Payload").toArray();

    QJsonValue channel_5_payload = data.value("Channel_5");
    QJsonObject channel_5_obj = channel_5_payload.toObject();
    QJsonArray channel_5 = channel_5_obj.value("Payload").toArray();



//    qDebug() << Channels << bits << Frame << Fs << n << endl;

      for(int i=0; i<128; i++){
          if(volt_0){
            ui->customPlot->graph(0)->addData(key, channel_0.at(i).toInt()); }
          if(volt_1){
            ui->customPlot->graph(1)->addData(key, channel_1.at(i).toInt()); }
          if(volt_2){
            ui->customPlot->graph(2)->addData(key, channel_2.at(i).toInt()); }

          if(current_0){
            ui->customPlot->graph(3)->addData(key, channel_3.at(i).toInt()); }
          if(current_1){
            ui->customPlot->graph(4)->addData(key, channel_4.at(i).toInt()); }
          if(current_2){
            ui->customPlot->graph(5)->addData(key, channel_5.at(i).toInt()); }



            key++;

            // make key axis range scroll with the data (at a constant range size of 8):
            ui->customPlot->xAxis->setRange(key, 512, Qt::AlignRight);
           // int volt_range = 80000;
           // ui->customPlot->yAxis->setRange(-1*volt_range, volt_range);

            ui->customPlot->replot();
      }


}

void MonitorGraph::setupPlot()
{
    const int PEN_WIDTH = 1;
    ui->customPlot->addGraph(); // yellow line
    ui->customPlot->graph(0)->setPen(QPen(QColor(255, 210, 41), PEN_WIDTH));
    ui->customPlot->graph(0)->setName("Voltage 0");


    ui->customPlot->addGraph(); // black line
    ui->customPlot->graph(1)->setPen(QPen(QColor(355, 110, 42), PEN_WIDTH));
    ui->customPlot->graph(1)->setName("Voltage 1");


    ui->customPlot->addGraph(); // purple line
    ui->customPlot->graph(2)->setPen(QPen(QColor(120, 10, 93), PEN_WIDTH));
    ui->customPlot->graph(2)->setName("Voltage 2");


    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(3)->setPen(QPen(QColor(200, 15, 50), PEN_WIDTH));
    ui->customPlot->graph(3)->setName("Current 0");


    ui->customPlot->addGraph(); // orange line
    ui->customPlot->graph(4)->setPen(QPen(QColor(255, 110, 41), PEN_WIDTH));
    ui->customPlot->graph(4)->setName("Current 1");


    ui->customPlot->addGraph(); // green line
    ui->customPlot->graph(5)->setPen(QPen(QColor(120, 215, 0), PEN_WIDTH));
    ui->customPlot->graph(5)->setName("Current 2");


    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->customPlot->xAxis->setTicker(timeTicker);
    ui->customPlot->axisRect()->setupFullAxesBox();
//    ui->customPlot->yAxis->setRange(-3, 3);

    int volt_range = 80000;
    ui->customPlot->yAxis->setRange(-1*volt_range, volt_range);

    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    dataTimer = new QTimer(this);
    connect(dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer->start(0); // Interval 0 means to refresh as fast as possible

}

void MonitorGraph::on_clearButton_clicked()
{
    ui->customPlot->graph(0)->data()->clear();
    ui->customPlot->graph(1)->data()->clear();
    ui->customPlot->graph(2)->data()->clear();
    ui->customPlot->graph(3)->data()->clear();
    ui->customPlot->graph(4)->data()->clear();
    ui->customPlot->graph(5)->data()->clear();
    ui->customPlot->replot();
    ui->customPlot->update();
}

void MonitorGraph::on_voltage_0_stateChanged(int arg1)
{
    if(arg1==2){
        volt_0 = true;
    }
    else{
        volt_0 = false;
    }
}

void MonitorGraph::on_voltage_1_stateChanged(int arg1)
{
    if(arg1==2){
        volt_1 = true;
    }
    else{
        volt_1 = false;
    }
}


void MonitorGraph::on_voltage_2_stateChanged(int arg1)
{
    if(arg1==2){
        volt_2 = true;
    }
    else{
        volt_2 = false;
    }
}

void MonitorGraph::on_current_0_stateChanged(int arg1)
{
    if(arg1==2){
        current_0 = true;
    }
    else{
        current_0 = false;
    }
}

void MonitorGraph::on_current_1_stateChanged(int arg1)
{
    if(arg1==2){
        current_1 = true;
    }
    else{
        current_1 = false;
    }
}


void MonitorGraph::on_current_2_stateChanged(int arg1)
{
    if(arg1==2){
        current_2 = true;
    }
    else{
        current_2 = false;
    }
}
