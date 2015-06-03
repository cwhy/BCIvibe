#include "ui_signalplot.h"
#include "metricplot.h"

MetricPlot::MetricPlot(QCustomPlot *_uiMetricPlot):uiMetricPlot(_uiMetricPlot)
{
    yRange = QCPRange(0, 1);
    timeRange = 600;
    uiMetricPlot->plotLayout()->clear();
    setUpAxis();

    line = uiMetricPlot->addGraph(axis->axis(QCPAxis::atBottom), axis->axis(QCPAxis::atLeft));
    QPen* linePen = new QPen(Qt::red);
    linePen->setWidthF(2);
    line->setPen(*linePen);

    leadDot = uiMetricPlot->addGraph(axis->axis(QCPAxis::atBottom), axis->axis(QCPAxis::atLeft));
    leadDot->setPen(QPen(Qt::black));
    leadDot->setLineStyle(QCPGraph::lsNone);
    leadDot->setScatterStyle(QCPScatterStyle::ssDisc);

    uiMetricPlot->replot();
}

void MetricPlot::setUpAxis()
{
    QCPAxisRect *tmpRect = new QCPAxisRect(uiMetricPlot);
    uiMetricPlot->plotLayout()->addElement(0, 0, tmpRect);
    // tmpRect->axis(QCPAxis::atLeft)->setTickLabels(false);
    tmpRect->setAntialiased(false);
    tmpRect->axis(QCPAxis::atLeft)->setSubTickPen(Qt::NoPen);
    // tmpRect->axis(QCPAxis::atLeft)->setAutoTickCount(2);
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
    tmpRect->axis(QCPAxis::atBottom)->setRange(0, timeRange);
    // tmpRect->axis(QCPAxis::atBottom)->setTickLabelRotation(45);
    axis = tmpRect;

    // Set up special background colors
    setUpBackgroud("lvl0", 0, 3*60, QColor("green").lighter(150));
    setUpBackgroud("lvl1", 3*60, 6*60, QColor("yellow").lighter(140));
    setUpBackgroud("lvl2", 6*60, 9*60, QColor("orange").lighter(130));

}

void MetricPlot::setUpBackgroud(QString name, float TStart, float TEnd, const QColor color)
{
    QCPItemRect *_background = new QCPItemRect(uiMetricPlot);
    uiMetricPlot->addItem(_background);
    uiMetricPlot->addLayer(name, uiMetricPlot->layer("grid"),QCustomPlot::limBelow);
    _background->setLayer(name);
    _background->topLeft->setTypeX(QCPItemPosition::ptPlotCoords);
    _background->topLeft->setTypeY(QCPItemPosition::ptAxisRectRatio);
    _background->bottomRight->setTypeX(QCPItemPosition::ptPlotCoords);
    _background->bottomRight->setTypeY(QCPItemPosition::ptAxisRectRatio);

    _background->topLeft->setAxes(axis->axis(QCPAxis::atBottom), axis->axis(QCPAxis::atLeft));
    _background->bottomRight->setAxes(axis->axis(QCPAxis::atBottom), axis->axis(QCPAxis::atLeft));
    _background->topLeft->setAxisRect(axis);
    _background->bottomRight->setAxisRect(axis);
    _background->setClipToAxisRect(true);

    _background->topLeft->setCoords(TStart,1);
    _background->bottomRight->setCoords(TEnd,0);

    _background->setBrush(QBrush(color));
    _background->setPen(Qt::NoPen);
}

void MetricPlot::metricPlotSlot(double metric)
{
    const short smooth = 10;
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    double value;
    static bool xaxisFix = false;
    if (metrics.length() < smooth){
        metrics << metric;
        value = metric;
    } else{
        metrics.removeFirst();
        metrics << metric;
        value = 0;
        for (auto p: metrics){
            value += p;
        }
        value /= smooth;
        if (key-lastKey > 0.02) // at most add point every 10 ms
        {
            if (!xaxisFix){
                xaxisFix = true;
                firstKey = key;
                // qDebug() << firstKey;
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
