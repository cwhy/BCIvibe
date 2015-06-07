#include "ui_signalplot.h"
#include "metricplot.h"

MetricPlot::MetricPlot(QCustomPlot *_uiMetricPlot):uiMetricPlot(_uiMetricPlot)
{
    isPaused = false;
    zeroKey = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    yRange = QCPRange(0, 2);
    yRangeInit = QCPRange(0.5, 1.5);
    timeRange = 60*6;
    uiMetricPlot->plotLayout()->clear();
    setUpAxis();
    setUpPlots();
}

void MetricPlot::setUpPlots()
{
    line = uiMetricPlot->addGraph(axis->axis(QCPAxis::atBottom), axis->axis(QCPAxis::atLeft));
    QPen* linePen = new QPen(QColor(Qt::gray).lighter(100));
    linePen->setWidthF(2);
    line->setPen(*linePen);
    line->setName("0.5 second window");

    smoothLine = uiMetricPlot->addGraph(axis->axis(QCPAxis::atBottom), axis->axis(QCPAxis::atLeft));
    QPen* smoothLinePen = new QPen(QColor(Qt::red).darker(140));
    smoothLinePen->setWidthF(2);
    smoothLine->setPen(*smoothLinePen);
    smoothLine->setName("15 seconds window");

    leadDot = uiMetricPlot->addGraph(axis->axis(QCPAxis::atBottom), axis->axis(QCPAxis::atLeft));
    leadDot->setPen(QPen(Qt::black));
    leadDot->setLineStyle(QCPGraph::lsNone);
    leadDot->setScatterStyle(QCPScatterStyle::ssDisc);

    QCPLegend *_legend = new QCPLegend;
    axis->insetLayout()->addElement(_legend, Qt::AlignTop|Qt::AlignLeft);
    _legend->setLayer("legend");
    uiMetricPlot->setAutoAddPlottableToLegend(false);
    _legend->addItem(new QCPPlottableLegendItem(_legend, line));
    _legend->addItem(new QCPPlottableLegendItem(_legend, smoothLine));
    _legend->setBorderPen(Qt::NoPen);
    _legend->setBrush(QBrush(Qt::transparent));
}

void MetricPlot::setUpAxis()
{
    QCPAxisRect *_rect = new QCPAxisRect(uiMetricPlot);
    uiMetricPlot->plotLayout()->addElement(0, 0, _rect);
    _rect->setAntialiased(false);
    _rect->axis(QCPAxis::atLeft)->setSubTickPen(Qt::NoPen);
    //  _rect->axis(QCPAxis::atLeft)->setLabel("Workload");
    _rect->axis(QCPAxis::atLeft)->setTickLength(0, 3);
    _rect->axis(QCPAxis::atLeft)->setRange(yRangeInit);
    _rect->axis(QCPAxis::atBottom)->setTickLabelType(QCPAxis::ltDateTime);
    _rect->axis(QCPAxis::atBottom)->setDateTimeFormat("mm:ss");
    _rect->axis(QCPAxis::atBottom)->setAutoTickStep(true);
    // tmpRect->axis(QCPAxis::atBottom)->setTickStep(int(timeRange/10));
    _rect->axis(QCPAxis::atBottom)->setTickLength(0, 3);
    _rect->axis(QCPAxis::atBottom)->setVisible(true);
    _rect->axis(QCPAxis::atBottom)->setSubTickPen(Qt::NoPen);
    _rect->axis(QCPAxis::atBottom)->setRange(zeroKey, timeRange);
    // tmpRect->axis(QCPAxis::atBottom)->setTickLabelRotation(45);

    axis = _rect;
    uiMetricPlot->replot();
}

void MetricPlot::setUpBackgroud(QString layerName, float TStart, float TEnd, const QColor color)
{
    QCPItemRect *_background = new QCPItemRect(uiMetricPlot);
    uiMetricPlot->addItem(_background);
    _background->setLayer(layerName);
    _background->topLeft->setTypeX(QCPItemPosition::ptPlotCoords);
    _background->topLeft->setTypeY(QCPItemPosition::ptAxisRectRatio);
    _background->bottomRight->setTypeX(QCPItemPosition::ptPlotCoords);
    _background->bottomRight->setTypeY(QCPItemPosition::ptAxisRectRatio);

    _background->topLeft->setAxes(axis->axis(QCPAxis::atBottom), axis->axis(QCPAxis::atLeft));
    _background->bottomRight->setAxes(axis->axis(QCPAxis::atBottom), axis->axis(QCPAxis::atLeft));
    _background->topLeft->setAxisRect(axis);
    _background->bottomRight->setAxisRect(axis);
    _background->setClipToAxisRect(true);

    // qDebug() << yRange.upper << yRange.lower;
    _background->topLeft->setCoords(TStart,yRange.upper);
    _background->bottomRight->setCoords(TEnd,yRange.lower);

    _background->setBrush(QBrush(color));
    _background->setPen(Qt::NoPen);
}

void MetricPlot::metricPlotSlot(double metric)
{
    const short smooth = 30;
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    key -= pauseFix;
    double value;
    if (!isPaused){
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
                line->addData(key-zeroKey, metric);
                smoothLine->addData(key-zeroKey, value);
                rescaleYAxis(value, 0.05);
                leadDot->clearData();
                leadDot->addData(key-zeroKey, value);
                // axis->axis(QCPAxis::atBottom)->setRange(key+0.5, timeRange, Qt::AlignRight);
                uiMetricPlot->replot();
                lastKey = key;
            }
        }
    }
}

void MetricPlot::pauseToggle(){
    static double pauseTime = 0;
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    if(!isPaused){
        pauseTime = key;
        isPaused = true;
    } else {
        pauseFix += key - pauseTime;
        isPaused = false;
    }
}

void MetricPlot::reInitialize(){
    isPaused = true;
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    zeroKey = key - pauseFix;
    axis->axis(QCPAxis::atBottom)->setRange(0, timeRange);
    for(auto graph: axis->graphs()){
        graph->clearData();
    }
    // uiMetricPlot->clearItems();

    // Set up special background colors
    uiMetricPlot->layer("level");
    if (!uiMetricPlot->layer("level")){
        uiMetricPlot->addLayer("level", uiMetricPlot->layer("grid"),QCustomPlot::limBelow);
        setUpBackgroud("level", 0, 2*60, QColor(0,255,0, 70));
        setUpBackgroud("level", 2*60, 4*60, QColor(255,155,0, 100));
        setUpBackgroud("level", 4*60, 6*60, QColor(222,41,16, 200));
    }
    uiMetricPlot->replot();
    isPaused = false;
}

void MetricPlot::rescaleYAxis(double value, double yPadding){
    QCPAxis* yAxis = axis->axis(QCPAxis::atLeft);
    if(value + yPadding > yRangeInit.upper &&
            value + yPadding > yAxis->range().upper){

        yAxis->setRangeUpper(value + yPadding);

    } else if (value - yPadding < yRangeInit.lower &&
               value - yPadding < yAxis->range().lower){

        yAxis->setRangeLower(value - yPadding);
    }
}
