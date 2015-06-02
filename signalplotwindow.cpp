#include "signalplotwindow.h"
#include "ui_signalplot.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>


SignalPlotWindow::SignalPlotWindow(Vibe *_signalPort, QWidget *parent) :
    QMainWindow(parent), signalPort(_signalPort),
    ui(new Ui::SignalPlotWindow)
{
    demoName = "Real Time EEG Signal Plot";
    ui->setupUi(this);
    setupDemo();
}

void SignalPlotWindow::setupDemo()
{

    setWindowTitle("BCIvibe: "+demoName);


    // Connection VRPNsignals to GUI plots
    signalPlot = new SignalPlot(ui->signalPlot);
    connect(signalPort, SIGNAL(gotSignal(double*)), signalPlot, SLOT(signalPlotSlot(double*)));

    ui->signalPlot->replot();
}

SignalPlotWindow::~SignalPlotWindow()
{
}

void SignalPlotWindow::mouseDoubleClickEvent(QMouseEvent *e)
{
    QWidget::mouseDoubleClickEvent(e);
    if((windowFlags() | Qt::FramelessWindowHint) == windowFlags()) {
        setWindowFlags( windowFlags() & ~Qt::FramelessWindowHint );
        show();
    } else {
        // qDebug() << windowFlags() <<Qt::FramelessWindowHint;
        setWindowFlags( windowFlags() | Qt::FramelessWindowHint );
        show();
    }
    qDebug()<<"Doubleclicked!";
}

void SignalPlotWindow::closeEvent(QCloseEvent *event)
{
    signalPort->terminate(); // VRPN is a blocking API!!
    event->accept();
}
