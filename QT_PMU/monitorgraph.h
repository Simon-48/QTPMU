#ifndef MONITORGRAPH_H
#define MONITORGRAPH_H

#include <QDialog>

namespace Ui {
class MonitorGraph;
}

class MonitorGraph : public QDialog
{
    Q_OBJECT

public:
    explicit MonitorGraph(QWidget *parent = nullptr);
    ~MonitorGraph();

    void setupPlot();

public slots:
    void update_datetime();

private slots:

    void on_return_home_clicked();

    void realtimeDataSlot();

    void on_voltage_1_stateChanged(int arg1);
    void on_voltage_0_stateChanged(int arg1);
    void on_voltage_2_stateChanged(int arg1);

    void on_current_1_stateChanged(int arg1);
    void on_current_0_stateChanged(int arg1);
    void on_current_2_stateChanged(int arg1);

    void on_clearButton_clicked();



//    void update_datetime();

private:
    Ui::MonitorGraph *ui;


    QTimer *dataTimer;
    int num;

    bool volt_0;
    bool volt_1;
    bool volt_2;

    bool current_0;
    bool current_1;
    bool current_2;






};

#endif // MONITORGRAPH_H
