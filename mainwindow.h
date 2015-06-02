#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot/qcustomplot.h"
#include "vibe.h"
#include "signalplot.h"
#include "thermometer.h"
#include "metricplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( Vibe *_metricPort, Vibe *_signalPort, QWidget *parent = 0);

    ~MainWindow();

    void setupDemo();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QString demoName;
    SignalPlot* signalPlot;
    Thermometer* thermPlot;
    MetricPlot* metricPlot;

    Vibe* signalPort;
    Vibe* metricPort;
    void mouseDoubleClickEvent(QMouseEvent *e);

};

#endif // MAINWINDOW_H
