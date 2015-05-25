#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot/qcustomplot.h"
#include "vibe.h"
#include "signalplot.h"

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

private:
    Ui::MainWindow *ui;
    QString demoName;
    Vibe* vibe;
    SignalPlot* signalplot;

    void mouseDoubleClickEvent(QMouseEvent *e);

};

#endif // MAINWINDOW_H
