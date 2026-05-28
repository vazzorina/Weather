#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <QObject>
#include <QSettings>

class WeatherData : public QObject
{
    Q_OBJECT
public:
    explicit WeatherData(QObject* parent = nullptr);

    QSettings *env;

    void writeApiKeyToEnvFile(QString apikey);
    QString readApiKeyFromEnvFile();
};

#endif // WEATHERDATA_H
