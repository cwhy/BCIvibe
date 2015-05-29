#ifndef CHANNELLOCATION_H
#define CHANNELLOCATION_H

#include <QVector3D>
#include <QString>

class ChannelLocation : public QVector3D
{
public:
    ChannelLocation::ChannelLocation(float x, float y, float z, QString _name):
        QVector3D(x, y, z), name(_name){}
    QString name;
};

#endif // CHANNELLOCATION_H
