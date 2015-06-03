#include "ui_signalplot.h"
#include "signalplot.h"

SignalPlot::SignalPlot(QCustomPlot *_uiSignalPlot)
{
    uiSignalPlot = _uiSignalPlot;

    timeRange = 20;
    QFile channelNameFile("channelToShow.txt");
    if (!channelNameFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        channelNames << "F3" << "F4" << "C3"
                     << "C4" << "P3" << "P4"
                     << "O1" << "O2";
        qDebug() << "channelToShow.txt is not found";
    } else {
        QTextStream textStream(&channelNameFile);
        while (!textStream.atEnd())
            channelNames << textStream.readLine();
    }
    N_ch = channelNames.length();

    colours << QColor("cyan") << QColor("magenta") << QColor("yellow") <<
               QColor("darkRed") << QColor("darkGreen") << QColor("darkMagenta") <<
               QColor("green") << QColor("darkCyan") << QColor("red") <<
               QColor("blue");
    setupPlot();

    lastKey = 0;
}

void SignalPlot::setupPlot()
{
    uiSignalPlot->plotLayout()->clear();
    QCPMarginGroup * marginGroup = new QCPMarginGroup(uiSignalPlot);
    for (int i=0;i<N_ch;i++)
    {
        QCPAxisRect *tmpRect = new QCPAxisRect(uiSignalPlot);
        QCPGraph *tmpL;  // line
        QCPGraph *tmpD;  // dot
        QColor tmpC;
        uiSignalPlot->plotLayout()->addElement(i, 0, tmpRect);
        uiSignalPlot->plotLayout()->setRowStretchFactor(i, 2);
        tmpRect->axis(QCPAxis::atBottom)->setVisible(false);
        // tmpRect->axis(QCPAxis::atLeft)->setVisible(true);
        // tmpRect->axis(QCPAxis::atLeft)->setRange(0, 1);
        // tmpRect->setAutoMargins(QCP::msLeft|QCP::msBottom|QCP::msTop);
        tmpRect->setMarginGroup(QCP::msLeft|QCP::msRight, marginGroup);
        tmpRect->setAutoMargins(QCP::msLeft);
        tmpRect->setMargins(QMargins(0, 0, 0, 0));
        // tmpRect->setupFullAxesBox();
        // connect(tmpRect->axis(QCPAxis::atBottom), SIGNAL(rangeChanged(QCPRange)),
        //         tmpRect->axis(QCPAxis::atTop), SLOT(setRange(QCPRange)));
        // connect(tmpRect->axis(QCPAxis::atLeft), SIGNAL(rangeChanged(QCPRange)),
        //         tmpRect->axis(QCPAxis::atRight), SLOT(setRange(QCPRange)));

        tmpRect->axis(QCPAxis::atLeft)->setTickLabels(false);
        tmpRect->axis(QCPAxis::atLeft)->setSubTickPen(Qt::NoPen);
        tmpRect->axis(QCPAxis::atLeft)->setAutoTickCount(2);
        tmpRect->axis(QCPAxis::atLeft)->setLabel(channelNames[i]);
        tmpRect->axis(QCPAxis::atLeft)->setTickLength(0, 3);
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
    QCPAxisRect *tmpRect = new QCPAxisRect(uiSignalPlot);
    uiSignalPlot->plotLayout()->addElement(N_ch, 0, tmpRect);
    uiSignalPlot->plotLayout()->setRowStretchFactor(N_ch, 1);
    tmpRect->axis(QCPAxis::atBottom)->setTickLabelType(QCPAxis::ltDateTime);
    tmpRect->axis(QCPAxis::atBottom)->setDateTimeFormat("hh:mm:ss");
    tmpRect->axis(QCPAxis::atBottom)->setAutoTickStep(false);
    tmpRect->axis(QCPAxis::atBottom)->setTickStep(2);
    tmpRect->axis(QCPAxis::atBottom)->setTickLength(0, 3);
    tmpRect->axis(QCPAxis::atBottom)->setVisible(true);
    tmpRect->axis(QCPAxis::atBottom)->setSubTickPen(Qt::NoPen);
    tmpRect->axis(QCPAxis::atLeft)->setVisible(false);
    tmpRect->axis(QCPAxis::atBottom)->grid()->setPen(Qt::NoPen);
    tmpRect->setMinimumSize(100, 30);
    tmpRect->setMarginGroup(QCP::msLeft|QCP::msRight, marginGroup);
    tmpRect->setAutoMargins(QCP::msLeft|QCP::msBottom);
    timeAxis = tmpRect;
}

void SignalPlot::signalPlotSlot(double* chData)
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    if (key-lastKey > 0.02) // at most add point every 10 ms
    {
        for (int i=0;i<N_ch;i++)
        {
            double value = chData[i];
            lines[i]->addData(key, value);
            lines[i]->removeDataBefore(key-timeRange);
            lines[i]->rescaleValueAxis(false);
            leadDots[i]->clearData();
            leadDots[i]->addData(key, value);
            axes[i]->axis(QCPAxis::atBottom)->setRange(key+0.5, timeRange, Qt::AlignRight);
        }
        // make key axis range scroll with the data (at a constant range size of signalRange):
        timeAxis->axis(QCPAxis::atBottom)->setRange(key+0.5, timeRange, Qt::AlignRight);
        uiSignalPlot->replot();
        lastKey = key;
    }

}
