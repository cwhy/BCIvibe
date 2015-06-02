#ifndef SIGNALPLOT_H
#define SIGNALPLOT_H

#include <QObject>
#include "qcustomplot/qcustomplot.h"


class SignalPlot : public QObject
{
    Q_OBJECT

public:
    explicit SignalPlot(QCustomPlot *uiSignalPlot);

private slots:
    void signalPlotSlot(double* chData);

private:
    QCustomPlot *uiSignalPlot;
    double timeRange;
    QList<QCPAxisRect*> axes;
    QCPAxisRect* timeAxis;
    QList<QCPGraph*> lines;
    QList<QCPGraph*> leadDots;
    QList<QString> channelNames;
    int N_ch;
    double lastKey;
    void setupPlot();

    QList<QColor> colours;
};

#endif // SIGNALPLOT_H
