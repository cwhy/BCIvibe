#include "vibe.h"
#include <QDebug>

void VRPN_CALLBACK vrpn_analog_callback_stub(void* user_data, vrpn_ANALOGCB analog)
{
    reinterpret_cast<Vibe *>(user_data)->getChData(analog);
}

void VRPN_CALLBACK Vibe::getChData(vrpn_ANALOGCB analog){
    const int N_ch = analog.num_channel;

    if(N_ch == 1){
        emit gotMetric(double(analog.channel[0]));
    } else {
        if(onePerChannel){
            const int trueN_ch = N_ch/bufferSize;
            // qDebug() << N_ch << " VRPN Channels, " << trueN_ch <<" true channels.";
            QVarLengthArray<double>* chData = new QVarLengthArray<double>(trueN_ch);
            double *data = chData->data();

            for(int i=0; i<trueN_ch; i++){
                data[i] = double(analog.channel[i*bufferSize]);
            }

            emit gotSignal(data);

        } else {
            QVarLengthArray<double>* chData = new QVarLengthArray<double>(N_ch);
            double *data = chData->data();
            for(int i=0; i<N_ch; i++){
                data[i] = double(analog.channel[i]);
            }
            emit gotSignal(data);
        }
    }
}

Vibe::Vibe(QString VibeHostName, int _bufferSize)
{
    bufferSize = _bufferSize;
    onePerChannel = true;
    /* Binding of the VRPN Analog to a callback */
    VRPNAnalog = new vrpn_Analog_Remote( VibeHostName.toStdString().c_str() );
    VRPNAnalog->register_change_handler( this, vrpn_analog_callback_stub );
    // qRegisterMetaType<vrpn_ANALOGCB>("vrpn_ANALOGCB");
    qDebug() << " :.)";
}

void Vibe::run()
{
    while(true){
        VRPNAnalog->mainloop();
    }
}
