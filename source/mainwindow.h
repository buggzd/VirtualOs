#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui_mainwindow.h"
#include <QtCharts>
#include "QTimer"
#include "QTime"
#include "QList"
#include "qmath.h"
#include "QPointF"
#include "QDebug"
#include "windows.h"
#include <thread>
#include <QMainWindow>
#include <iostream>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include "memory_management.h"
using namespace std;

namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void update();
    std::thread *th1;
    double count;
    //初始化内存显示
    void inistMemoryDisplay();
    //初始胡时间显示
    void inistClock();
    //初始化硬盘显示
    void inistDiskDisplay();
//隐藏
    void hideWindow();
    ~MainWindow();
protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
private:

    //连接信号槽
    void connectAll();
    //折线图要用到
    QChart *m_chart;
    QLineSeries *m_series;
    //点图用到
    QScatterSeries *m_scatter_on;
    QScatterSeries *m_scatter_noton;
    QChart *PointChart;

    int currentDisk;

    Ui::MainWindow *ui;
    double MemoryGetData(double time);
protected:

private slots:
    void updateTime();
    void pushFileInMemory(file f);
    //磁盘传参
    void pushFileInDisk(int length);
    void deletFileFromDisk(int length);
};

#endif // MAINWINDOW_H
