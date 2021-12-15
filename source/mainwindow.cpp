#include "mainwindow.h"
#include "ui_mainwindow.h"

//QList<double> dataList;//存储业务数据
int maxSize = 5000;

//QTimer updateTimer;
int timeId;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),//默认初始化？
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setMouseTracking(true);

    inistMemoryDisplay();
    inistClock();
    inistDiskDisplay();

    hideWindow();

    connectAll();
    ui->desktop->setStyleSheet(
                        "QPushButton{border-image: url(:/Computer.png);}"
                        "QPushButton:hover{border-image: url(:/ComputerOn.png);}"
                        "QPushButton:pressed{border-image: url(:/ComputerOn.png);}"
                       );
}
void MainWindow::hideWindow(){
    ui->memory->hide();
    ui->file->hide();
    ui->disk->hide();
}
//放入内存
void MainWindow::pushFileInMemory(file f){
    qDebug()<<"mainwindow slot:push"<<QString::fromStdString(f.fileName)<<"in to memory";
    qDebug()<<"type:"<<QString::fromStdString(f.fileType);
    qDebug()<<"size:"<<QString::number(f.fileSize);
    qDebug()<<"blocksize:"<<QString::number(f.blocknumber);
    memory_management0.AllocBlock(f);
}
//初始化内存折线图
void MainWindow::inistMemoryDisplay(){
    memory_management0.InitBlock();
    //新建一个chart
    m_chart = new QChart;
    QChartView *chartView = new QChartView(m_chart);

//    v.setRubberBand(QChartView::HorizontalRubberBand);
//  chartView->setRubberBand(QChartView::RectangleRubberBand);
//    chartView->setRubberBand();
    //seires是存放线性数据
    m_series = new QLineSeries;
    m_chart->addSeries(m_series);

    m_chart->setZValue(1);
    for(int i=0;i<maxSize;++i){
       m_series->append(i,0);
    }
    //m_series->setUseOpenGL(true);//openGl 加速
    //qDebug()<<m_series->useOpenGL();

    //x轴设置
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0,maxSize);
    axisX->setLabelFormat("%g");
    axisX->setTitleText("axisX");

     //y轴设置
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0,1);
    axisY->setTitleText("axisY");

    //绑定x，y轴到chart上
    m_chart->setAxisX(axisX,m_series);
    m_chart->setAxisY(axisY,m_series);
    m_chart->legend()->hide();
    m_chart->setTitle("内存占用率");

    QVBoxLayout *layout = ui->verticalLayout;
    layout->addWidget(chartView);

//    connect(&updateTimer,SIGNAL(timeout()),this,SLOT(updatedataSlot()));
//    updateTimer.start(0);
    timeId = startTimer(30);
}

//折线图chart获取数据使用
double MainWindow::MemoryGetData(double time){

    //double s = qCos( time * M_PI * 2 ) ;
    double temp=memory_management0.percent();
    //qDebug()<<"memory percent"<<temp;
    return temp;
}
void MainWindow::update(){
    while(1){
        qDebug()<<"???";
        count+=0.1f;
        Sleep(1000);
    }
}

void MainWindow::timerEvent(QTimerEvent *event){
    if(event->timerId()==timeId){//定时器到时间,//模拟数据填充
        static QTime dataTime(QTime::currentTime());
        long int eltime = dataTime.elapsed();
        static int lastpointtime = 0;
        int size = (eltime - lastpointtime);//数据个数
        //qDebug()<<"size-->"<<count;
        if(isVisible()){
            QVector<QPointF> oldPoints = m_series->pointsVector();//Returns the points in the series as a vector
            QVector<QPointF> points;

            for(int i=size;i<oldPoints.count();++i){
                points.append(QPointF(i-size ,oldPoints.at(i).y()));//替换数据用
            }
            qint64 sizePoints = points.count();
            for(int k=0;k<size;++k){
                points.append(QPointF(k+sizePoints,MemoryGetData((((double)lastpointtime+k+1)/1000))));
            }
            m_series->replace(points);
            lastpointtime = eltime;
       }
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTime(){
       QDateTime dt=QDateTime::currentDateTime();
       QString p;
       p=dt.toString("hh:mm:ss  ");
       this->ui->clock->setText(p);
}
void MainWindow::inistClock(){

}
void MainWindow::pushFileInDisk(int length){
    qDebug()<<"Mainwindow pushFileInDisk"<<QString::number(length)<<"current:"<<QString::number(currentDisk);
    QPointF closest(INT_MAX, INT_MAX);
    qreal firstPosx,firstPosy;
    firstPosx=currentDisk%64;
    firstPosy=currentDisk/64+1;
    length=(length-1)/4+1;
    int counter=length;

    for(int i=0;i<=length;i++){
        closest=QPointF((currentDisk+i)%64+1, (currentDisk+i)/64+1);
        m_scatter_on->remove(closest);
        m_scatter_noton->append(closest);
    }
    currentDisk=currentDisk+length;
}

void MainWindow::deletFileFromDisk(int length){
     qDebug()<<"Mainwindow deletFile"<<QString::number(length)<<"current:"<<QString::number(currentDisk);
    QPointF closest(INT_MAX, INT_MAX);
    qreal firstPosx,firstPosy;
    firstPosx=currentDisk%64;
    firstPosy=currentDisk/64+1;
    length=(length-1)/4+1;
    int counter=length;
    for (firstPosy; firstPosy >0; firstPosy --) {
      for(firstPosx;firstPosx>0;firstPosx--){
          closest=QPointF(firstPosx, firstPosy);
          m_scatter_on->append(closest);
          m_scatter_noton->remove(closest);
          counter--;
          if(counter==0){
              break;
          }
      }
        firstPosx=64;
        if(counter==0){
          break;
        }
    }
    currentDisk=currentDisk-length;
}
//初始化磁盘展示
void MainWindow::inistDiskDisplay(){

    currentDisk=0;
    //新建chartview展示框
    PointChart=new QChart;
    QChartView *chartView = new QChartView(PointChart);

    //初始化待用表
    m_scatter_on = new QScatterSeries();
    m_scatter_on->setName("待用");
    for (qreal x(1); x <= 64; x += 1) {
        for (qreal y(1); y <= 16; y += 1)
            *m_scatter_on << QPointF(x, y);
    }
    //初始化已用
    m_scatter_noton = new QScatterSeries();
    m_scatter_noton->setName("已用");

    m_scatter_on->setMarkerSize(8.0);
    m_scatter_noton->setMarkerSize(8.0);
    //设置待用表的形状为矩形
    m_scatter_on->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    //将数据录入Chart
    PointChart->addSeries(m_scatter_noton);
    PointChart->addSeries(m_scatter_on);
    //创建坐标轴
    PointChart->createDefaultAxes();
    PointChart->axes(Qt::Horizontal).first()->setRange(0, 64.5);
    PointChart->axes(Qt::Vertical).first()->setRange(0, 16.5);

    //连接到display框上
    ui->diskDisplay->addWidget(chartView);

   // connect(m_scatter_on, &QScatterSeries::clicked, this, &ChartView::handleClickedPoint);
}

void MainWindow::connectAll(){
    //时间显示
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateTime()));
    timer->start(1000);
    updateTime();
    //设置为内存页面
    connect(ui->btn_memory,&QPushButton::clicked,[=](){
        ui->memory->show();
        ui->memory->raise();
    });
    //内存页面缩小
    connect(ui->btn_memory_min,&QPushButton::clicked,[=](){
        ui->memory->hide();
    });
    //设置为磁盘页面
    connect(ui->btn_disk,&QPushButton::clicked,[=](){
        ui->disk->show();
        ui->disk->raise();
    });
    //磁盘页面缩小
    connect(ui->btn_disk_min,&QPushButton::clicked,[=](){
        ui->disk->hide();
    });
    //设置为目录页面
    connect(ui->btn_file,&QPushButton::clicked,[=](){
        ui->file->show();
        ui->file->raise();
    });
    //目录页面缩小
    connect(ui->btn_file_min,&QPushButton::clicked,[=](){
        ui->file->hide();
    });
    connect(ui->desktop,&QPushButton::clicked,[=](){
        ui->file->show();
         ui->file->raise();
    });
    connect(ui->file,&fileWidget::fileOnUsing,this,&MainWindow::pushFileInMemory);

    connect(ui->file,&fileWidget::creatFileToDisk,this,&MainWindow::pushFileInDisk);
    connect(ui->file,&fileWidget::deletFileToDisk,this,&MainWindow::deletFileFromDisk);

}
