#ifndef SIGNALPLOT_H
#define SIGNALPLOT_H
#include "qcustomplot/qcustomplot.h"
#include "vibe.h"
#include <QObject>


class SignalPlot : public QObject
{
    Q_OBJECT

public:
    explicit SignalPlot(QCustomPlot *uiSignalPlot);
    void stopVibe();

private slots:
    void signalPlotSlot(vrpn_ANALOGCB);

private:
    Vibe* vibe;
    QCustomPlot *uiSignalPlot;
    double signalRange;
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
