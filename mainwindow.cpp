#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QBarSet>
#include <QTimer>
#include "AboutDialog.h"
#include <QDesktopServices>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort;

    initSelectItem();
    initqcharts();
    initMenu();
    //timer=new QTimer(this);
    //QObject::connect(this->timer,SIGNAL(timeout()),this,SLOT (freshenButtonClick()),Qt::UniqueConnection);

    ui->ReceivePlainTextEdit->setReadOnly(true);//设置接收窗口为只读

    QObject::connect(ui->searchButton,SIGNAL(clicked(bool)),this,SLOT (searchSerialPort()),Qt::UniqueConnection);
    QObject::connect(ui->openButton,SIGNAL(clicked(bool)),this,SLOT (openButton()),Qt::UniqueConnection);
    //QObject::connect(ui->freshenButton,SIGNAL(clicked(bool)),this,SLOT (StartfreshenButtonClick()),Qt::UniqueConnection);

    QObject::connect(ui->LeftButton,SIGNAL(clicked(bool)),this,SLOT (leftButtonClick()),Qt::UniqueConnection);
    QObject::connect(ui->rightButton,SIGNAL(clicked(bool)),this,SLOT (rightButtonClick()),Qt::UniqueConnection);
    QObject::connect(ui->forwardButton,SIGNAL(clicked(bool)),this,SLOT (forwarduttonClick()),Qt::UniqueConnection);
    QObject::connect(ui->stopButton,SIGNAL(clicked(bool)),this,SLOT (stopButtonClick()),Qt::UniqueConnection);
    QObject::connect(ui->backButton,SIGNAL(clicked(bool)),this,SLOT (backButtonClick()),Qt::UniqueConnection);
    QObject::connect(ui->selectButton,SIGNAL(clicked(bool)),this,SLOT (selectButtonClick()),Qt::UniqueConnection);
    QObject::connect(ui->exitButton,SIGNAL(clicked(bool)),this,SLOT (exitButtonClick()),Qt::UniqueConnection);
    QObject::connect(ui->clearButton,SIGNAL(clicked(bool)),this,SLOT (clearButtonClick()),Qt::UniqueConnection);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMenu()
{
    QMenu *aboutMenu=new QMenu("帮助",ui->menuBar);

    QAction *action=new QAction;

    makeAction(action,aboutMenu,"关于(A)", Qt::CTRL + Qt::Key_A);
    connect(action,SIGNAL(triggered()),this,SLOT(aboutSlots()));
    aboutMenu->addAction(action);

    makeAction(action,aboutMenu,"开源(O)", Qt::CTRL + Qt::Key_O);
    connect(action,SIGNAL(triggered()),this,SLOT(OpenSourceDialog()));
    aboutMenu->addAction(action);

    makeAction(action,aboutMenu,"说明(U)", Qt::CTRL + Qt::Key_U);
    connect(action,SIGNAL(triggered()),this,SLOT(useDialog()));
    aboutMenu->addAction(action);

    ui->menuBar->addMenu(aboutMenu);
}

void MainWindow::initSelectItem()
{
    ui->setBaudRate->addItem("1200",1200);
    ui->setBaudRate->addItem("2400",2400);
    ui->setBaudRate->addItem("4800",4800);
    ui->setBaudRate->addItem("9600",9600);
    ui->setBaudRate->addItem("14400",14400);
    ui->setBaudRate->addItem("19200",19200);
    ui->setBaudRate->addItem("38400",38400);
    ui->setBaudRate->addItem("43000",43000);
    ui->setBaudRate->addItem("57600",57600);
    ui->setBaudRate->addItem("76800",76800);
    ui->setBaudRate->addItem("115200",115200);
    ui->setBaudRate->setCurrentIndex(3);

    ui->setStopBits->addItem("1",1);
    ui->setStopBits->addItem("1.5",1.5);
    ui->setStopBits->addItem("2",2);

    ui->setParitycheck->addItem("无",0);
    ui->setParitycheck->addItem("奇校验",1);
    ui->setParitycheck->addItem("偶校验",2);

    ui->setDataBits->addItem("8",8);
    ui->setDataBits->addItem("7",7);
    ui->setDataBits->addItem("6",6);
    ui->setDataBits->addItem("5",5);
}

void MainWindow::initqcharts()
{
    mychart=new QChart;
    Bar=new QBarSeries(this);//条状图
    mAxis=new QBarCategoryAxis;
    mychart->setTheme(QChart::ChartThemeDark);
    set0=new QBarSet(tr("加速度计"));
    set0->setColor(Qt::blue);
    set0->append(0);
    set0->append(0);
    set0->append(0);

    Bar->append(set0);
    list=new QStringList;
    *list<<"X轴"<<"Y轴"<<"Z轴";
    mAxis->append(*list);  //设置横轴坐标值，这里是用qstring
    Bar->setBarWidth(0.1);
    mychart->addSeries(Bar);
    mychart->createDefaultAxes();
    mychart->setAxisX(mAxis,Bar);
    mychart->axisY()->setRange(-300,300);
    mychart->axisY()->setGridLineVisible(true);
    mychart->setTitle("三轴加速度计");
    mychart->legend()->hide();//隐藏图标
    mychart->setAnimationOptions(QChart::SeriesAnimations);
    //mychart->setAnimationDuration(90);
    ChartView.setChart(mychart);
    ChartView.setRenderHint(QPainter::Qt4CompatiblePainting);
    ChartView.setParent(ui->widget);
    layout.addWidget(&ChartView);
    ui->widget->setLayout(&layout);

    temmychart=new QChart;
    temBar=new QBarSeries(this);//条状图
    temmAxis=new QBarCategoryAxis;
    temmychart->setTheme(QChart::ChartThemeDark);
    temset0=new QBarSet(tr("温度计"));
    temset0->setColor(Qt::red);
    temset0->append(20);

    temBar->append(temset0);
    temlist=new QStringList;
    *temlist<<"A:温度";
    temmAxis->append(*temlist);  //设置横轴坐标值，这里是用qstring
    temBar->setBarWidth(0.1);
    temmychart->addSeries(temBar);
    temmychart->createDefaultAxes();
    temmychart->setAxisX(temmAxis,temBar);
    temmychart->axisY()->setRange(-30,90);
    temmychart->axisY()->setGridLineVisible(true);
    temmychart->setTitle("温度");
    temmychart->legend()->hide();//隐藏图标
    temmychart->setAnimationOptions(QChart::SeriesAnimations);
   // temmychart->setAnimationDuration(90);
    temChartView.setChart(temmychart);
    temChartView.setRenderHint(QPainter::Qt4CompatiblePainting);
    temChartView.setParent(ui->widget_2);
    temlayout.addWidget(&temChartView);
    ui->widget_2->setLayout(&temlayout);

    hummychart=new QChart;
    humBar=new QBarSeries(this);//条状图
    hummAxis=new QBarCategoryAxis;
    hummychart->setTheme(QChart::ChartThemeDark);
    humset0=new QBarSet(tr("湿度计"));
    humset0->setColor(Qt::cyan);
    humset0->append(20);

    humBar->append(humset0);
    humlist=new QStringList;
    *humlist<<"A:湿度";
    hummAxis->append(*humlist);  //设置横轴坐标值，这里是用qstring
    humBar->setBarWidth(0.1);
    hummychart->addSeries(humBar);
    hummychart->createDefaultAxes();
    hummychart->setAxisX(hummAxis,humBar);
    hummychart->axisY()->setRange(0,100);
    hummychart->axisY()->setGridLineVisible(true);
    hummychart->setTitle("湿度");
    hummychart->legend()->hide();//隐藏图标
    hummychart->setAnimationOptions(QChart::SeriesAnimations);
    //hummychart->setAnimationDuration(90);
    humChartView.setChart(hummychart);
    humChartView.setRenderHint(QPainter::Qt4CompatiblePainting);
    humChartView.setParent(ui->widget_3);
    humlayout.addWidget(&humChartView);
    ui->widget_3->setLayout(&humlayout);

    airmychart=new QChart;
    airBar=new QBarSeries(this);//条状图
    airmAxis=new QBarCategoryAxis;
    airmychart->setTheme(QChart::ChartThemeDark);
    airset0=new QBarSet(tr("空气质量"));
    airset0->setColor(Qt::yellow);
    airset0->append(2000);

    airBar->append(airset0);
    airlist=new QStringList;
    *airlist<<"A:空气质量";
    airmAxis->append(*airlist);  //设置横轴坐标值，这里是用qstring
    airBar->setBarWidth(0.1);
    airmychart->addSeries(airBar);
    airmychart->createDefaultAxes();
    airmychart->setAxisX(airmAxis,airBar);
    airmychart->axisY()->setRange(500,3000);
    airmychart->axisY()->setGridLineVisible(true);
    airmychart->setTitle("空气质量");
    airmychart->legend()->hide();//隐藏图标
    airmychart->setAnimationOptions(QChart::SeriesAnimations);
    //airmychart->setAnimationDuration(90);
    airChartView.setChart(airmychart);
    airChartView.setRenderHint(QPainter::Qt4CompatiblePainting);
    airChartView.setParent(ui->widget_4);
    airlayout.addWidget(&airChartView);
    ui->widget_4->setLayout(&airlayout);

}

bool MainWindow::makeAction(QAction*& action, QWidget* parent, QString text, int key)
{
    bool ret = true;

    action = new QAction(text, parent);

    if( action != NULL )
    {
        action->setShortcut(QKeySequence(key));
    }
    else
    {
        ret = false;
    }

    return ret;
}
bool MainWindow::makeAction(QAction*& action, QWidget* parent, QString tip, QString icon)//配置Qaction选项 用于工具栏里的action
{
    bool ret = true;

    action = new QAction("", parent);

    if( action != NULL )
    {
        action->setToolTip(tip);
        action->setIcon(QIcon(icon));
    }
    else
    {
        ret = false;
    }

    return ret;
}

void MainWindow::useDialog()
{
    ///---实例化消息对话框对象

    msgBox = new QMessageBox("说明",       ///--这里是设置消息框标题
        "15电气一班唐伟康的毕业设计\n\n配套毕业设计硬件部分小车的上位机",                       ///--这里是设置消息框显示的内容
        QMessageBox::Information,                          ///--这里是在消息框显示的图标
        QMessageBox::Ok ,     ///---这里是显示消息框上的按钮情况
        0,  ///---这里与 键盘上的 escape 键结合。当用户按下该键，消息框将执行cancel按钮事件
        0);                                                     ///---这里是 定义第三个按钮， 该例子 只是 了显示2个按钮
    msgBox->exec();                                  ///---显示消息框
    delete msgBox;
}

void MainWindow::aboutSlots()
{
    AboutDialog(this).exec();
}

void MainWindow::OpenSourceDialog()
{
    QDesktopServices::openUrl(QUrl("https://github.com/balanceTWK"));
}
