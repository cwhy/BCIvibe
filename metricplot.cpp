#include "ui_signalplot.h"
#include "metricplot.h"

MetricPlot::MetricPlot(QCustomPlot *_uiMetricPlot)
{
    yRange = QCPRange(0, 1);
    timeRange = 600;
    uiMetricPlot = _uiMetricPlot;
    uiMetricPlot->plotLayout()->clear();
    QCPAxisRect *tmpRect = new QCPAxisRect(uiMetricPlot);
    uiMetricPlot->plotLayout()->addElement(0, 0, tmpRect);
    // tmpRect->axis(QCPAxis::atLeft)->setTickLabels(false);
    tmpRect->setAntialiased(false);
    tmpRect->axis(QCPAxis::atLeft)->setSubTickPen(Qt::NoPen);
    tmpRect->axis(QCPAxis::atLeft)->setAutoTickCount(2);
    tmpRect->axis(QCPAxis::atLeft)->setLabel("Workload");
    tmpRect->axis(QCPAxis::atLeft)->setTickLength(0, 3);
    tmpRect->axis(QCPAxis::atLeft)->setRange(yRange);
    tmpRect->axis(QCPAxis::atBottom)->setTickLabelType(QCPAxis::ltDateTime);
    tmpRect->axis(QCPAxis::atBottom)->setDateTimeFormat("mm:ss");
    tmpRect->axis(QCPAxis::atBottom)->setAutoTickStep(false);
    tmpRect->axis(QCPAxis::atBottom)->setTickStep(int(timeRange/10));
    tmpRect->axis(QCPAxis::atBottom)->setTickLength(0, 3);
    tmpRect->axis(QCPAxis::atBottom)->setVisible(true);
    tmpRect->axis(QCPAxis::atBottom)->setSubTickPen(Qt::NoPen);
    tmpRect->axis(QCPAxis::atBottom)->setTickLabelRotation(45);
    line = uiMetricPlot->addGraph(tmpRect->axis(QCPAxis::atBottom), tmpRect->axis(QCPAxis::atLeft));
    QPen* linePen = new QPen(Qt::red);
    linePen->setWidthF(2);
    line->setPen(*linePen);

    leadDot = uiMetricPlot->addGraph(tmpRect->axis(QCPAxis::atBottom), tmpRect->axis(QCPAxis::atLeft));
    leadDot->setPen(QPen(Qt::black));
    leadDot->setLineStyle(QCPGraph::lsNone);
    leadDot->setScatterStyle(QCPScatterStyle::ssDisc);
    axis = tmpRect;
}

void MetricPlot::metricPlotSlot(double metric)
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    double value;
    static bool xaxisFix = false;
    if (metrics.length() < 30){
        metrics << metric;
        value = metric;
    } else{
        metrics.removeFirst();
        metrics << metric;
        value = 0;
        for (auto p: metrics){
            value += p;
        }
        value /= 30;
        if (key-lastKey > 0.02) // at most add point every 10 ms
        {
            if (!xaxisFix){
                axis->axis(QCPAxis::atBottom)->setRange(0, timeRange);
                xaxisFix = true;
                firstKey = key;
            }
            line->addData(key-firstKey, value);
            // line->removeDataBefore(key-timeRange);
            // line->rescaleValueAxis(false);
            leadDot->clearData();
            leadDot->addData(key-firstKey, value);
            // axis->axis(QCPAxis::atBottom)->setRange(key+0.5, timeRange, Qt::AlignRight);
            uiMetricPlot->replot();
            lastKey = key;
        }
    }
}
