#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    demoName = "Real Time Data Demo";
    ui->setupUi(this);
    setupDemo();
}

void MainWindow::setupDemo()
{

    setWindowTitle("BCIvibe: "+demoName);

    // Set up VRPN connections
    signalPort = new Vibe("Sinus@localhost", 8);
    signalPort->start();

    metricPort = new Vibe("therm@localhost", 1);
    metricPort->start();

    // Connection VRPNsignals to GUI plots
    signalPlot = new SignalPlot(ui->signalPlot);
    thermPlot = new Thermometer(ui->thermPlot);
    metricPlot = new MetricPlot(ui->metricPlot);
    connect(signalPort, SIGNAL(gotSignal(double*)), signalPlot, SLOT(signalPlotSlot(double*)));
    connect(metricPort, SIGNAL(gotMetric(double)), thermPlot, SLOT(thermometerSlot(double)));
    connect(metricPort, SIGNAL(gotMetric(double)), metricPlot, SLOT(metricPlotSlot(double)));

    ui->signalPlot->replot();
    ui->thermPlot->replot();
    ui->metricPlot->replot();
}


void MainWindow::mouseDoubleClickEvent(QMouseEvent *e)
{
    QWidget::mouseDoubleClickEvent(e);
    if(isFullScreen()) {
        this->setWindowState(Qt::WindowMaximized);
    } else {
        this->setWindowState(Qt::WindowFullScreen);
        qDebug()<<"Doubleclicked!";
    }
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    signalPort->terminate(); // VRPN is a blocking API!!
    metricPort->terminate(); // VRPN is a blocking API!!
    event->accept();
}
