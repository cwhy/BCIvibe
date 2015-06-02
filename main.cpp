#include <QApplication>
#include "signalplotwindow.h"
#include "metricplotwindow.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QMessageBox::critical(this, "", "You're using Qt < 5, the realtime data demo needs functions that are available with Qt 4.7 to work properly");
#endif
    QApplication a(argc, argv);

    QFont font;
    font.setFamily(QStringLiteral("Arial"));
    font.setPointSize(11);
    font.setWeight(50);
    QApplication::setFont(font);

    // Set up VRPN connections
    Vibe* signalPort = new Vibe("Sinus@localhost", 8);
    signalPort->start();

    Vibe* metricPort = new Vibe("therm@localhost", 1);
    metricPort->start();
    SignalPlotWindow sw = SignalPlotWindow(signalPort);
    MetricPlotWindow mw = MetricPlotWindow(metricPort);
    mw.show();
    sw.show();

    return a.exec();
}
