#ifndef THERMOMETER_H
#define THERMOMETER_H
#include "qcustomplot/qcustomplot.h"
#include "vibe.h"

#include <QObject>

class Thermometer : public QObject
{
    Q_OBJECT

public:
    explicit Thermometer(QCustomPlot *uiThermometer);
    void stopVibe();


private slots:
    void thermometerSlot(vrpn_ANALOGCB);

private:
    Vibe* vibe;
    QCustomPlot* uiThermometer;
    QCPBars* therm;
    double lastKey;
    QCPRange yRange;

    QVector<double> ticks;
    QVector<QString> tickLabels;
    QCPColorGradient *colorMap;
};

#endif // THERMOMETER_H
