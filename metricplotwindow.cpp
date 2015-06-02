#include "metricplotwindow.h"
#include "ui_metricplot.h"
#include <QDebug>
// #include <QDesktopWidget>
// #include <QScreen>
// #include <QMessageBox>
// #include <QMetaEnum>


MetricPlotWindow::MetricPlotWindow(Vibe *_metricPort, QWidget *parent) :
    QMainWindow(parent), metricPort(_metricPort),
    ui(new Ui::MetricPlotWindow)
{
    demoName = "Biometric for Workload";
    ui->setupUi(this);
    setupDemo();
}

void MetricPlotWindow::setupDemo()
{

    setWindowTitle("BCIvibe: "+demoName);

    // Connection VRPNsignals to GUI plots
    thermPlot = new Thermometer(ui->thermPlot);
    metricPlot = new MetricPlot(ui->metricPlot);
    connect(metricPort, SIGNAL(gotMetric(double)), thermPlot, SLOT(thermometerSlot(double)));
    connect(metricPort, SIGNAL(gotMetric(double)), metricPlot, SLOT(metricPlotSlot(double)));

    ui->thermPlot->replot();
    ui->metricPlot->replot();
}

void MetricPlotWindow::mouseDoubleClickEvent(QMouseEvent *e)
{
    QWidget::mouseDoubleClickEvent(e);
    if((windowFlags() | Qt::FramelessWindowHint) == windowFlags()) {
        qDebug() << windowFlags();
        setWindowFlags( windowFlags() & ~Qt::FramelessWindowHint );
        show();
    } else {
        qDebug() << windowFlags() <<Qt::FramelessWindowHint;
        setWindowFlags( windowFlags() | Qt::FramelessWindowHint );
        show();
    }
    qDebug()<<"Doubleclicked!";
}

MetricPlotWindow::~MetricPlotWindow()
{
}

void MetricPlotWindow::closeEvent(QCloseEvent *event)
{
    metricPort->terminate(); // VRPN is a blocking API!!
    event->accept();
}
