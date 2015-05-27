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
    // setupSignalPlots(ui->signalPlot);
    signalPlot = new SignalPlot(ui->signalPlot);
    thermPlot = new Thermometer(ui->thermPlot);

    setWindowTitle("BCIvibe: "+demoName);
    statusBar()->clearMessage();
    ui->signalPlot->replot();
    ui->thermPlot->replot();
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
    signalPlot->stopVibe();
    thermPlot->stopVibe();
    delete signalPlot;
    delete thermPlot;
    delete ui;
}

    // calculate frames per second:
//    static double lastFpsKey;
//    static int frameCount;
//    ++frameCount;
//    if (key-lastFpsKey > 2) // average fps over 2 seconds
//    {
//        ui->statusBar->showMessage(
//                    QString("%1 FPS, Total Data points:")
//                    .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
//                    , 0);
//        lastFpsKey = key;
//        frameCount = 0;
//    }
