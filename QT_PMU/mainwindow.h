#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <monitorgraph.h>
#include <QDebug>

#include <QTimer>
#include <QTime>
#include <QDateTime>
#include <nlohmann/json.hpp>

using nlohmann::json;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    static QString get_date();
    static QString get_time();
    static json get_channel_data();


private slots:
    void update_datetime();
    void on_monitor_btn_clicked();
//    QString get_date();

private:
    Ui::MainWindow *ui;
    MonitorGraph *monitor = new MonitorGraph(this);

    QTimer *mainTimer;
};
#endif // MAINWINDOW_H
