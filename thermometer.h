#ifndef THERMOMETER_H
#define THERMOMETER_H
#include "qcustomplot/qcustomplot.h"

#include <QObject>

class Thermometer : public QObject
{
    Q_OBJECT

public:
    explicit Thermometer(QCustomPlot *uiThermometer);

private slots:
    void thermometerSlot(double metric);

private:
    QCustomPlot* uiThermometer;
    QCPBars* therm;
    double lastKey;
    QCPRange yRange;

    QVector<double> ticks;
    QVector<QString> tickLabels;
    QCPColorGradient *colorMap;
};

#endif // THERMOMETER_H
