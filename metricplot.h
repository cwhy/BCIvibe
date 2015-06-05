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
    double gameStartKey;
    double zeroKey;
    double lastKey;
    QCPRange yRange;
    QCPRange yRangeInit;
    double timeRange;

    QList<double> metrics;
    void setUpAxis();
    void setUpBackgroud(QString name, float TStart, float TEnd, const QColor color);
};

#endif // METRICPLOT_H
