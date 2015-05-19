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
    vibe = new Vibe();
    vibe->start();
    setupDemo();
}

void MainWindow::setupDemo()
{
    signalRange = 20;
    channelNames << "Channel 1" << "Channel 2" << "Channel 3" << "Channel 4"
                 << "Channel 1" << "Channel 2" << "Channel 3" << "Channel 4"
                 << "Channel 1" << "Channel 2" << "Channel 3" << "Channel 4"
                 << "Channel 1" << "Channel 2" << "Channel 3" << "Channel 4";
    N_ch = channelNames.length();
    setupSignalPlots(ui->customPlot);
    qRegisterMetaType<vrpn_ANALOGCB>("vrpn_ANALOGCB");
    connect(vibe, SIGNAL(gotAnalog(vrpn_ANALOGCB)), this, SLOT(signalPlotSlot(vrpn_ANALOGCB)));

    setWindowTitle("BCIvibe: "+demoName);
    statusBar()->clearMessage();
    ui->customPlot->replot();
}

void MainWindow::setupSignalPlots(QCustomPlot *customPlot)
{
    colours << QColor("cyan") << QColor("magenta") << QColor("red") <<
               QColor("darkRed") << QColor("darkCyan") << QColor("darkMagenta") <<
               QColor("green") << QColor("darkGreen") << QColor("yellow") <<
               QColor("blue");
    customPlot->plotLayout()->clear();
    for (int i=0;i<N_ch;i++)
    {
        QCPAxisRect *tmpRect = new QCPAxisRect(customPlot);
        QCPGraph *tmpL;
        QCPGraph *tmpD;
        QColor tmpC;
        customPlot->plotLayout()->addElement(i, 0, tmpRect);
        //customPlot->plotLayout()->setRowStretchFactor(0, 1);
        tmpRect->axis(QCPAxis::atBottom)->setVisible(false);
        tmpRect->axis(QCPAxis::atLeft)->setRange(0, 1);
        tmpRect->setAutoMargins(QCP::msLeft|QCP::msBottom|QCP::msTop);
        tmpRect->setMargins(QMargins(0, 0, 0, 0));
        // tmpRect->setupFullAxesBox();
        // connect(tmpRect->axis(QCPAxis::atBottom), SIGNAL(rangeChanged(QCPRange)),
        //         tmpRect->axis(QCPAxis::atTop), SLOT(setRange(QCPRange)));
        // connect(tmpRect->axis(QCPAxis::atLeft), SIGNAL(rangeChanged(QCPRange)),
        //         tmpRect->axis(QCPAxis::atRight), SLOT(setRange(QCPRange)));

        foreach(QCPAxis *axis, tmpRect->axes())
        {
            axis->setLayer("axes");
            axis->grid()->setLayer("grid");
        }
        tmpC = colours[i % 10];

        tmpL = customPlot->addGraph(tmpRect->axis(QCPAxis::atBottom), tmpRect->axis(QCPAxis::atLeft));
        tmpL->setPen(QPen(tmpC.darker(200)));
        tmpD = customPlot->addGraph(tmpRect->axis(QCPAxis::atBottom), tmpRect->axis(QCPAxis::atLeft));

        tmpD->setPen(QPen(Qt::black));
        tmpD->setLineStyle(QCPGraph::lsNone);
        tmpD->setScatterStyle(QCPScatterStyle::ssDisc);
        axes << tmpRect;
        lines << tmpL;
        leadDots << tmpD;
    }
    axes.last()->axis(QCPAxis::atBottom)->setTickLabelType(QCPAxis::ltDateTime);
    axes.last()->axis(QCPAxis::atBottom)->setDateTimeFormat("hh:mm:ss");
    axes.last()->axis(QCPAxis::atBottom)->setAutoTickStep(false);
    axes.last()->axis(QCPAxis::atBottom)->setTickStep(2);
    axes.last()->axis(QCPAxis::atBottom)->setVisible(true);
}

void MainWindow::signalPlotSlot(vrpn_ANALOGCB chData)
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.02) // at most add point every 10 ms
    {
        for (int i=0;i<N_ch;i++)
        {
            double value = chData.channel[i];
            lines[i]->addData(key, value);
            lines[i]->removeDataBefore(key-signalRange);
            lines[i]->rescaleValueAxis(true);
            leadDots[i]->clearData();
            leadDots[i]->addData(key, value);
        }
        for (int i=0;i<N_ch;i++)
        {
            axes[i]->axis(QCPAxis::atBottom)->setRange(key+0.5, signalRange, Qt::AlignRight);
        }
        ui->customPlot->replot();
        lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of signalRange):

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
        ui->statusBar->showMessage(
                    QString("%1 FPS, Total Data points:")
                    .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
                    , 0);
        lastFpsKey = key;
        frameCount = 0;
    }
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
    vibe->exit();
    delete vibe;
    delete ui;
}


