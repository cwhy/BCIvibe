#include "ui_mainwindow.h"
#include "signalplot.h"

SignalPlot::SignalPlot(QCustomPlot *_uiSignalPlot)
{
    vibe = new Vibe("Sinus@localhost", 8);
    vibe->start();
    uiSignalPlot = _uiSignalPlot;

    signalRange = 20;
    channelNames << "Channel 1" << "Channel 2" << "Channel 3" << "Channel 4"
                 << "Channel 5";
    N_ch = channelNames.length();
    colours << QColor("cyan") << QColor("magenta") << QColor("red") <<
               QColor("darkRed") << QColor("darkCyan") << QColor("darkMagenta") <<
               QColor("green") << QColor("darkGreen") << QColor("yellow") <<
               QColor("blue");
    uiSignalPlot->plotLayout()->clear();
    for (int i=0;i<N_ch;i++)
    {
        QCPAxisRect *tmpRect = new QCPAxisRect(uiSignalPlot);
        QCPGraph *tmpL;
        QCPGraph *tmpD;
        QColor tmpC;
        uiSignalPlot->plotLayout()->addElement(i, 0, tmpRect);
        //uiSignalPlot->plotLayout()->setRowStretchFactor(0, 1);
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

        tmpL = uiSignalPlot->addGraph(tmpRect->axis(QCPAxis::atBottom), tmpRect->axis(QCPAxis::atLeft));
        tmpL->setPen(QPen(tmpC.darker(200)));
        tmpD = uiSignalPlot->addGraph(tmpRect->axis(QCPAxis::atBottom), tmpRect->axis(QCPAxis::atLeft));

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

    lastKey = 0;
    connect(vibe, SIGNAL(gotAnalog(vrpn_ANALOGCB)), this, SLOT(signalPlotSlot(vrpn_ANALOGCB)));
}

void SignalPlot::signalPlotSlot(vrpn_ANALOGCB chData)
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    if (key-lastKey > 0.02) // at most add point every 10 ms
    {
        for (int i=0;i<N_ch;i++)
        {
            double value = chData.channel[vibe->bufferSize*i];
            lines[i]->addData(key, value);
            lines[i]->removeDataBefore(key-signalRange);
            lines[i]->rescaleValueAxis(false);
            leadDots[i]->clearData();
            leadDots[i]->addData(key, value);
        }
        for (int i=0;i<N_ch;i++)
        {
    // make key axis range scroll with the data (at a constant range size of signalRange):
            axes[i]->axis(QCPAxis::atBottom)->setRange(key+0.5, signalRange, Qt::AlignRight);
        }
        uiSignalPlot->replot();
        lastKey = key;
    }

}

void SignalPlot::stopVibe(){
    vibe->quit();
}
