#ifndef VIBE_H
#define VIBE_H

#include <vrpn_Analog.h>
#include <QObject>
#include <QThread>


class Vibe : public QThread
{
    Q_OBJECT

protected:
    void run();

public:
    Vibe(QString VibeHostName, int _bufferSize);
    void VRPN_CALLBACK handleAnalog(vrpn_ANALOGCB analog);
    int bufferSize;
    void stop();

signals:
    void gotAnalog(vrpn_ANALOGCB);

public slots:
private:
    vrpn_Analog_Remote* VRPNAnalog;
    QMutex mutex;
    bool toStop;
};

#endif // VIBE_H
