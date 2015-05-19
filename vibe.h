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
    Vibe();
    vrpn_Analog_Remote* VRPNAnalog;
    void VRPN_CALLBACK handleAnalog(void* user_data, vrpn_ANALOGCB analog);

signals:
    void gotAnalog(vrpn_ANALOGCB);

public slots:
};

#endif // VIBE_H
