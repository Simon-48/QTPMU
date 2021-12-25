#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>
#include <string.h>
#include <cmath>
#include<math.h>
#include <map>
#include <ctime>
#include <climits>




// prePhase = np.zeros([channels, 1])  # phase angle
double prePhase[6][1];

// preOmega = np.zeros([channels, 1])  # last angular speed
double preOmega[6][1];


using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->home->setIconSize(QSize(10, 10));

    // Setup GPS Timer
    mainTimer = new QTimer(this);
    connect(mainTimer, SIGNAL(timeout()), this, SLOT(update_datetime()));
    mainTimer->start(0); // Interval 0 means to refresh as fast as possible

//    json data;


}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::get_date()
{
    QDate current_date(QDate::currentDate());
    return current_date.toString();
}

QString MainWindow::get_time()
{
    QTime current_time(QTime::currentTime());
    return current_time.toString();
}


void MainWindow::update_datetime()
{
    QString current_time = get_time();
    QString current_date = get_date();

    ui->label_date->setText(current_date);
    ui->label_time->setText(current_time);
}


void MainWindow::on_monitor_btn_clicked()
{
   hide();

  // monitor = new MonitorGraph(this);
   monitor->show();

}

json MainWindow::get_channel_data(){
    int channels = 6;

    // frequencies = [39.0, 87.0, 35.0, 65.0, 62.0, 32.0]
    double frequencies[6] = {50.0, 50.0, 50.0, 50.0, 50.0, 50.0};
    // cout << sizeof(frequencies)/sizeof(frequencies[0]) << endl;

    // phaseAngles = np.array([0, 1, 2, 0, 1, 2]) * np.pi * 2 / 3
    double phaseAngles[6] =  {0, 1, 2, 0, 1, 2};
    for(int i=0; i<channels; i++){
        phaseAngles[i] = phaseAngles[i] * M_PI * 2/3;
    }

    // frequencies = np.array(frequencies, ndmin=2).T
    double frequencies_t[channels][1];
    for(int i=0; i<channels; i++){
        frequencies_t[i][0] = frequencies[i];
    }

    // phaseAngles = np.array(phaseAngles, ndmin=2).T
    double phaseAngles_t[channels][1];
    for(int i=0; i<channels; i++){
        phaseAngles_t[i][0] = phaseAngles[i];
    }

    double interval = 0.0265;
    int Fs = 12800;
    int n = int(Fs * interval);
    int ADCRange = pow(2, 15) - 1;
    int bits = 16;

    // resultDict = dict() and others

    json resultDict;
    resultDict["Fs"] = Fs;
    resultDict["n"] = n;
    resultDict["Channels"] = channels;
    resultDict["bits"] = bits;

    // intervalDelta = timedelta(seconds=interval)
    // tSeries = np.expand_dims(np.linspace(0, interval, n+1, endpoint=True), axis=0)  # time

    double start_time = 0.0;
    double tSeries[129];
    tSeries[0] = start_time;
    for(int i=1; i<129; i++){
        tSeries[i] = tSeries[i-1]+0.000078;
    }

//    prePhase = np.zeros([channels, 1])  # phase angle
//    double prePhase[channels][1];
//    for(int i=0; i<channels; i++){
//        prePhase[i][0] = 0.0;
//    }

//    preOmega = np.zeros([channels, 1])  # last angular speed
//    double preOmega[channels][1];
//    for(int i=0; i<channels; i++){
//        preOmega[i][0] = 0.0;
//    }



    int frame = 0;
    resultDict["Frame"] = frame;

    resultDict["Date"] = get_date().toStdString();
    resultDict["Time"] = get_time().toStdString();

//    QString timestamp =
//    resultDict["Timestamp"] = QTime::currentTime();

    // newOmega = 2 * np.pi * frequencies  # angular speed
    double newOmega[channels][1];
    for(int i=0; i<channels; i++){
        newOmega[i][0] = 2 * M_PI * frequencies_t[i][0];
    }


    // phaseSeries = np.dot(newOmega, tSeries) + np.dot((newOmega - preOmega), np.power(tSeries, 2)) / 2.0 / interval + prePhase
    double phaseSeries[6][129];
    for(int i=0; i<6; i++){
        for(int j=0; j<129; j++){
            phaseSeries[i][j] = (newOmega[i][0]*tSeries[j]) +  ((newOmega[i][0]-preOmega[i][0]) * pow(tSeries[j], 2))  / 2 / interval + prePhase[i][0];
            //  + ( (newOmega[i][0] - preOmega[i][0]) * pow(tSeries[j], 2) ) / 2 / interval / prePhase[i][0]
        }
    }


    // prePhase = np.expand_dims(phaseSeries[:, -1], axis=1)

    for(int i=0; i<channels; i++){
        prePhase[i][0] = phaseSeries[i][128];
    }

    for(int i=0; i<channels; i++){
        preOmega[i][0] = newOmega[i][0];
    }

    // payload = (np.cos(phaseSeries[:,0:-1] + phaseAngles) * ADCRange).astype(np.int16).byteswap()
    int phaseSeries_complete[6][128];
    //uint16_t payload[6][128];

    for(int i=0; i<channels; i++){
            //cout << "Printing Phase cosines" <<endl;
        for(int j=0; j<128; j++){
            if(i > 2){
                // Decreasing current amplitude by half
                phaseSeries_complete[i][j] = (int) (cos(phaseSeries[i][j] + phaseAngles_t[i][0]) * ADCRange/2 ) ;
            }else {
                phaseSeries_complete[i][j] = (int) (cos(phaseSeries[i][j] + phaseAngles_t[i][0]) * ADCRange) ;
            }
//            phaseSeries_complete[i][j] = (int) (cos(phaseSeries[i][j] + phaseAngles_t[i][0]) * ADCRange) ;
            //payload[i][j] = swap_endian<uint16_t>(phaseSeries_complete[i][j]);

            //cout << phaseSeries_complete[i][j] << ", ";
        }
        //cout << endl;
    }

    // Save payload to dict
    for(int i=0; i<channels; i++){
        string channel_i = "Channel_" + to_string(i);
        json channel_payload;
        resultDict[channel_i]["Payload"] = phaseSeries_complete[i];
    }

    // update variables for next iteration
    frame++;
    for (int i=0; i<6; i++){
        frequencies[i] = 50.0;
    }


    return resultDict;

}
