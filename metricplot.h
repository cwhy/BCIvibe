#ifndef METRICPLOT_H
#define METRICPLOT_H

#include <QObject>
#include "qcustomplot/qcustomplot.h"

class MetricPlot : public QObject
{
    Q_OBJECT

public:
    explicit MetricPlot(QCustomPlot *_uiMetricPlot);

private slots:
    void metricPlotSlot(double metric);

private:
    QCustomPlot *uiMetricPlot;
    QCPAxisRect* timeAxis;
    QCPAxisRect* axis;
    QCPGraph* line;
    QCPGraph* leadDot;
    double firstKey;
    double lastKey;
    QCPRange yRange;
    double timeRange;

    QList<double> metrics;
};

#endif // METRICPLOT_H
