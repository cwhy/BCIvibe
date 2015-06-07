#ifndef METRICPLOT_H
#define METRICPLOT_H

#include <QObject>
#include "qcustomplot/qcustomplot.h"

class MetricPlot : public QObject
{
    Q_OBJECT

public:
    explicit MetricPlot(QCustomPlot *_uiMetricPlot);
    void pauseToggle();
    void reInitialize();

private slots:
    void metricPlotSlot(double metric);

private:
    bool isPaused;
    double pauseFix;
    QCustomPlot *uiMetricPlot;
    QCPAxisRect* timeAxis;
    QCPAxisRect* axis;
    QCPGraph* line;
    QCPGraph* leadDot;
    double zeroKey;
    double lastKey;
    QCPRange yRange;
    QCPRange yRangeInit;
    double timeRange;

    QList<double> metrics;
    void setUpAxis();
    void setUpBackgroud(QString name, float TStart, float TEnd, const QColor color);
    void rescaleYAxis(double value, double yPadding);
};

#endif // METRICPLOT_H
