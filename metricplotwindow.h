#ifndef METRICPLOTWINDOW_H
#define METRICPLOTWINDOW_H


#include <QMainWindow>
#include <QTimer>
#include "qcustomplot/qcustomplot.h"
#include "vibe.h"
#include "thermometer.h"
#include "metricplot.h"

namespace Ui {
class MetricPlotWindow;
}

class MetricPlotWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MetricPlotWindow( Vibe *_metricPort, QWidget *parent = 0);

    ~MetricPlotWindow();

    void setupDemo();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MetricPlotWindow *ui;
    QString demoName;
    Thermometer* thermPlot;
    MetricPlot* metricPlot;

    Vibe* metricPort;
    void mouseDoubleClickEvent(QMouseEvent *e);

};

#endif // METRICPLOTWINDOW_H
