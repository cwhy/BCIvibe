#ifndef SIGNALPLOTWINDOW_H
#define SIGNALPLOTWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot/qcustomplot.h"
#include "vibe.h"
#include "signalplot.h"

namespace Ui {
class SignalPlotWindow;
}

class SignalPlotWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SignalPlotWindow( Vibe *_signalPort, QWidget *parent = 0);

    ~SignalPlotWindow();

    void setupDemo();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::SignalPlotWindow *ui;
    QString demoName;
    SignalPlot* signalPlot;

    Vibe* signalPort;
    void mouseDoubleClickEvent(QMouseEvent *e);

};

#endif // SIGNALPLOTWINDOW_H
