#ifndef METRICPLOT_H
#define METRICPLOT_H

#include <QObject>
#include "qcustomplot/qcustomplot.h"

class Level {

public:
    explicit Level(float _tS, float _tE, QColor _c, QString _n);
    void setUpBackgroud(QCPAxisRect *axis, QString layerName);
    void setUpFinishLine(QCPAxisRect *axis);
    void updateBackground(QList<double>& metrics, double t);
    void resetBackground();
    QCPItemRect *background;
    QCPItemLine *finishLine;
    double yMax;
    double yMin;
    double signalRate;
private:
    const double tStart;
    const double tEnd;
    const QColor color;
    const QString name;
};

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

    QList<short> smoothWindows;
    QMap<short, QCPGraph*>* lines;
    QMap<short, QCPGraph*>* leadDots;
    QMap<short, QColor>* colors;
    short smoothDefault;

    double signalRate;
    double zeroKey;
    double lastKey;
    QCPRange yRange;
    QCPRange yRangeInit;
    double timeRange;


    QList<Level> levels;
    void setUpAxis();
    void setUpPlots();
    void setUpLevels();
    void rescaleYAxis(double value, double yPadding);
};

#endif // METRICPLOT_H
