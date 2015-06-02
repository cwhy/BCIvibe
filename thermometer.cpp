#include "ui_mainwindow.h"
#include "thermometer.h"

Thermometer::Thermometer(QCustomPlot *_uiThermometer)
{
    yRange = QCPRange(0, 1);
    uiThermometer = _uiThermometer;
    // uiThermometer->plotLayout()->clear();
    therm = new QCPBars(uiThermometer->xAxis, uiThermometer->yAxis);
    colorMap = new QCPColorGradient(QCPColorGradient::gpJet);

    uiThermometer->addPlottable(therm);
    QPen pen;
    pen.setWidthF(1.2);
    pen.setColor(QColor(0, 0, 0, 0));
    therm->setPen(pen);

    therm->setName("Workload Indicator");
    therm->setBrush(Qt::white);
    ticks << 1;
    tickLabels << "Workload";
    uiThermometer->xAxis->setAutoTicks(false);
    uiThermometer->xAxis->setAutoTickLabels(false);
    uiThermometer->xAxis->setTickVector(ticks);
    uiThermometer->xAxis->setTickVectorLabels(tickLabels);
//   uiThermometer->xAxis->setTickLabelRotation(60);
    uiThermometer->xAxis->setSubTickCount(0);
    uiThermometer->xAxis->setTickLength(0, 4);
    uiThermometer->xAxis->grid()->setVisible(false);
    uiThermometer->xAxis->setRange(0, 2);

    uiThermometer->yAxis->setRange(yRange);
    uiThermometer->yAxis->setPadding(5); // a bit more space to the left border
    uiThermometer->yAxis->setLabel("Workload");
    uiThermometer->yAxis->grid()->setSubGridVisible(true);
    lastKey = 0;
}

void Thermometer::thermometerSlot(double metric)
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    if (key-lastKey > 0.02) // at most add point every 10 ms
    {
        QVector<double> values;
        values << metric;
        therm->setData(ticks, values);
        therm->setBrush(QBrush(colorMap->color(metric, yRange)));

        lastKey = key;
        uiThermometer->replot();
    }
}
