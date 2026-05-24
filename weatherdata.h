#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <QObject>

class WeatherData : public QObject
{
    Q_OBJECT
public:
    explicit WeatherData(QObject* parent = nullptr);
};

#endif // WEATHERDATA_H
