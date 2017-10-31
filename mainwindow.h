#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>//提供访问串口的功能
#include <QtSerialPort/QSerialPortInfo>//提供系统中存在的串口的信息

#include <QtWidgets/QApplication>//加上这两行 就能用了QChart
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QCategoryAxis>
QT_CHARTS_USE_NAMESPACE//加上这两行 就能用了QChart
#include <QGridLayout>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QBarSet>
#include <QMessageBox>
#include <QDesktopServices>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QString RceiveStr;
    QByteArray Rceive;

    void initSelectItem();
    //QTimer *timer;

QBarSeries *Bar=0;
QBarCategoryAxis *mAxis;
QValueAxis *mAyis;
QBarSet *set0;
QStringList *list;
QChart *mychart;
QChartView ChartView;
QGridLayout layout;

QBarSeries *temBar=0;
QBarCategoryAxis *temmAxis;
QValueAxis *temmAyis;
QBarSet *temset0;
QStringList *temlist;
QChart *temmychart;
QChartView temChartView;
QGridLayout temlayout;

QBarSeries *humBar=0;
QBarCategoryAxis *hummAxis;
QValueAxis *hummAyis;
QBarSet *humset0;
QStringList *humlist;
QChart *hummychart;
QChartView humChartView;
QGridLayout humlayout;

QBarSeries *airBar=0;
QBarCategoryAxis *airmAxis;
QValueAxis *airmAyis;
QBarSet *airset0;
QStringList *airlist;
QChart *airmychart;
QChartView airChartView;
QGridLayout airlayout;

QMessageBox *msgBox;

    void initqcharts();
 void initMenu();
 bool makeAction(QAction*& action, QWidget* parent, QString text, int key);//配置Qaction选项 用于菜单栏里的action
 bool makeAction(QAction*& action, QWidget* parent, QString tip, QString icon);//配置Qaction选项 用于工具栏里的action

private slots:
    //void freshenButtonClick();
    //void StartfreshenButtonClick();
    void searchSerialPort();
    void openButton();
    void Read_Data();
    void clearButtonClick();

    void leftButtonClick();
    void rightButtonClick();
    void forwarduttonClick();
    void backButtonClick();
    void stopButtonClick();

    void selectButtonClick();
    void exitButtonClick();

    void aboutSlots();
    void OpenSourceDialog();
    void useDialog();
};

#endif // MAINWINDOW_H
