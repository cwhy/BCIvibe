#ifndef VIBE_H
#define VIBE_H

#include <QObject>
#include <QThread>
#include <vrpn_Analog.h>


class Vibe : public QThread
{
    Q_OBJECT

protected:
    void run();

public:
    Vibe(QString VibeHostName, int _bufferSize);
    void VRPN_CALLBACK getChData(vrpn_ANALOGCB analog);
    int bufferSize;

signals:
    void gotSignal(double* chData);
    void gotMetric(double metric);

public slots:
private:
    vrpn_Analog_Remote* VRPNAnalog;
    bool onePerChannel;
};

#endif // VIBE_H
