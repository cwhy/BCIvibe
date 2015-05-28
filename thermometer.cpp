#include "ui_mainwindow.h"
#include "thermometer.h"

Thermometer::Thermometer(QCustomPlot *_uiThermometer)
{
    vibe = new Vibe("therm@localhost", 1);
    vibe->start();
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
    uiThermometer->xAxis->grid()->setVisible(true);
    uiThermometer->xAxis->setRange(0, 2);

    uiThermometer->yAxis->setRange(yRange);
    uiThermometer->yAxis->setPadding(5); // a bit more space to the left border
    uiThermometer->yAxis->setLabel("Workload");
    uiThermometer->yAxis->grid()->setSubGridVisible(true);
    lastKey = 0;
    connect(vibe, SIGNAL(gotAnalog(vrpn_ANALOGCB)), this, SLOT(thermometerSlot(vrpn_ANALOGCB)));
}

void Thermometer::thermometerSlot(vrpn_ANALOGCB chData)
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    if (key-lastKey > 0.02) // at most add point every 10 ms
    {
        double value = chData.channel[0];
        QVector<double> values;
        values << value;
        therm->setData(ticks, values);
        therm->setBrush(QBrush(colorMap->color(value, yRange)));

        lastKey = key;
        uiThermometer->replot();
    }
}

void Thermometer::stopVibe(){
    vibe->stop();
    vibe->quit();
    delete vibe;
}
