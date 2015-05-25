#include "vibe.h"
#include <QDebug>

void VRPN_CALLBACK vrpn_analog_callback_stub(void* user_data, vrpn_ANALOGCB analog)
{
  reinterpret_cast<Vibe *>(user_data)->handleAnalog(analog);
}

void VRPN_CALLBACK Vibe::handleAnalog(vrpn_ANALOGCB analog){
    double perc;
    perc = analog.channel[0];
    //qDebug()<<perc<<" :.|";
    emit gotAnalog(analog);
}

Vibe::Vibe(QString VibeHostName)
{
    bufferSize = 4;
    /* Binding of the VRPN Analog to a callback */
    VRPNAnalog = new vrpn_Analog_Remote( VibeHostName.toStdString().c_str() );
    VRPNAnalog->register_change_handler( this, vrpn_analog_callback_stub );
    qDebug() << " :.|";
}

void Vibe::run(){
    while(true){
       VRPNAnalog->mainloop();
    }
}
