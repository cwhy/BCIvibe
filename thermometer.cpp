#include "ui_mainwindow.h"
#include "thermometer.h"

Thermometer::Thermometer(QCustomPlot *_uiThermometer)
{
    vibe = new Vibe("therm@localhost", 1);
    vibe->start();
    ymax = 0;
    ymin = 7;
    uiThermometer = _uiThermometer;
    // uiThermometer->plotLayout()->clear();
    therm = new QCPBars(uiThermometer->xAxis, uiThermometer->yAxis);
    uiThermometer->addPlottable(therm);
    QPen pen;
    pen.setWidthF(1.2);
    therm->setName("Workload Indicator");
    pen.setColor(QColor(150, 222, 0));
    therm->setPen(pen);
    therm->setBrush(QColor(150, 222, 0, 70));
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

    uiThermometer->yAxis->setRange(ymin, ymax);
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
        int i = 0;
        double value = chData.channel[i];
        QVector<double> values;
        values << value;
        uiThermometer->replot();
        lastKey = key;
        therm->setData(ticks, values);

        lastKey = key;
    }
    uiThermometer->replot();
}

void Thermometer::stopVibe(){
    vibe->quit();
}
