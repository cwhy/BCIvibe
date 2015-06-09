#include "ui_signalplot.h"
#include "metricplot.h"

MetricPlot::MetricPlot(QCustomPlot *_uiMetricPlot):
    uiMetricPlot(_uiMetricPlot)
{
    isPaused = false;
    zeroKey = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    yRange = QCPRange(0, 2);
    yRangeInit = QCPRange(0.5, 1.5);
    timeRange = 60*6;
    signalRate = 0.5;
    smoothWindows << 4 << 30;
    smoothDefault = smoothWindows.last();
    levels << Level(0,    2*60, QColor(0,255,0, 70), "lvl1")
           << Level(2*60, 4*60, QColor(255,155,0, 100), "lvl2")
           << Level(4*60, 6*60, QColor(222,41,16, 200), "lvl3");
    QList<QColor> colorList = QList<QColor>()
                            << QColor(Qt::lightGray) << QColor(Qt::darkRed)
                            << QColor("green") << QColor("darkCyan");
    auto itrColor = colorList.begin();
    colors = new QMap<short, QColor>;
    for(short win: smoothWindows){
        colors->insert(win, *(itrColor));
        itrColor++;
    }

    uiMetricPlot->plotLayout()->clear();
    setUpAxis();
    setUpPlots();
    setUpLevels();
}

void MetricPlot::setUpPlots()
{

    lines = new QMap<short, QCPGraph*>;
    leadDots = new QMap<short, QCPGraph*>;
    for(short win: smoothWindows){
        QCPGraph* _line;
        _line = uiMetricPlot->addGraph(axis->axis(QCPAxis::atBottom), axis->axis(QCPAxis::atLeft));
        QPen linePen = QPen(colors->value(win));
        linePen.setWidthF(2);
        _line->setPen(linePen);
        _line->setName(QString("%1 second window").arg(int(signalRate*win)));
        lines->insert(win, _line);

        QCPGraph* _leadDot;
        _leadDot = uiMetricPlot->addGraph(axis->axis(QCPAxis::atBottom), axis->axis(QCPAxis::atLeft));
        _leadDot->setPen(QPen(colors->value(win)));
        _leadDot->setLineStyle(QCPGraph::lsNone);
        _leadDot->setScatterStyle(QCPScatterStyle::ssDisc);
        leadDots->insert(win, _leadDot);
    }
    (*leadDots)[smoothDefault]->setPen(QPen(Qt::black));

    QCPLegend *_legend = new QCPLegend;
    axis->insetLayout()->addElement(_legend, Qt::AlignTop|Qt::AlignLeft);
    _legend->setLayer("legend");
    uiMetricPlot->setAutoAddPlottableToLegend(false);
    for(short win: smoothWindows){
        _legend->addItem(new QCPPlottableLegendItem(_legend, lines->value(win)));
    }
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


void MetricPlot::metricPlotSlot(double metric)
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    static QList<double> metrics;
    key -= pauseFix;
    QMap<short, double> mSmoothed;
    if (!isPaused){
        if (key-lastKey > 0.02) // at most add point every 10 ms
        {
            metrics << metric;
            int mLen = metrics.length();
            if (mLen > timeRange*signalRate){
                metrics.removeFirst();
                mLen = timeRange*signalRate;
            }
            for(short win: smoothWindows){
                mSmoothed[win] = 0;
                for (int i=qMax(0, mLen-win); i<mLen; i++){
                    mSmoothed[win] += metrics[i];
                }
                mSmoothed[win] /= qMin(int(win), mLen);
                lines->value(win)->addData(key-zeroKey, mSmoothed.value(win));
                leadDots->value(win)->clearData();
                leadDots->value(win)->addData(key-zeroKey, mSmoothed.value(win));
            }
            rescaleYAxis(mSmoothed.value(smoothDefault), 0.05);
            // axis->axis(QCPAxis::atBottom)->setRange(key+0.5, timeRange, Qt::AlignRight);


            uiMetricPlot->replot();
            lastKey = key;
        }
    }
}


void Level::updateBackground(QList<double>& metrics)
{

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
    axis->axis(QCPAxis::atBottom)->setRange(0, timeRange);
    for(auto graph: axis->graphs()){
        graph->clearData();
    }
    // uiMetricPlot->clearItems();

    // Set up special background colors
    if (zeroKey==0){
        for (auto l: levels){
            uiMetricPlot->addItem(l.background);
        }
    }
    uiMetricPlot->replot();
    zeroKey = key - pauseFix;
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

void Level::setUpBackgroud(QCPAxisRect*axis, QString layerName)
{
    background->setLayer(layerName);
    background->topLeft->setTypeX(QCPItemPosition::ptPlotCoords);
    background->topLeft->setTypeY(QCPItemPosition::ptAxisRectRatio);
    background->bottomRight->setTypeX(QCPItemPosition::ptPlotCoords);
    background->bottomRight->setTypeY(QCPItemPosition::ptAxisRectRatio);

    background->topLeft->setAxes(axis->axis(QCPAxis::atBottom), axis->axis(QCPAxis::atLeft));
    background->bottomRight->setAxes(axis->axis(QCPAxis::atBottom), axis->axis(QCPAxis::atLeft));
    background->topLeft->setAxisRect(axis);
    background->bottomRight->setAxisRect(axis);
    background->setClipToAxisRect(true);

    // qDebug() << yRange.upper << yRange.lower;
    background->topLeft->setCoords(tStart, yMax);
    background->bottomRight->setCoords(tEnd, yMin);

    background->setBrush(QBrush(color));
    background->setPen(Qt::NoPen);
}

Level::Level(float _tS, float _tE, QColor _c, QString _n):
    tStart(_tS), tEnd(_tE), color(_c), name(_n)
{
}

void MetricPlot::setUpLevels(){
    uiMetricPlot->layer("levels");
    uiMetricPlot->addLayer("levels", uiMetricPlot->layer("grid"), QCustomPlot::limBelow);
    for (auto l: levels){
        l.yMax = yRange.upper;
        l.yMin = yRange.lower;
        l.background = new QCPItemRect(uiMetricPlot);
        l.setUpBackgroud(axis, "levels");
    }
}
