#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot/qcustomplot.h"
#include "vibe.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setupDemo();
    void setupSignalPlots(QCustomPlot *customPlot);


private slots:
    void signalPlotSlot(vrpn_ANALOGCB);

private:
    Ui::MainWindow *ui;
    QString demoName;
    Vibe* vibe;

    double signalRange;
    QList<QCPAxisRect*> axes;
    QList<QCPGraph*> lines;
    QList<QCPGraph*> leadDots;
    QList<QString> channelNames;
    int N_ch;

    QList<QColor> colours;

    void mouseDoubleClickEvent(QMouseEvent *e);

};

#endif // MAINWINDOW_H
