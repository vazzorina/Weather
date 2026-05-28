#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <QObject>
#include <QSettings>
#include <QNetworkAccessManager>

class WeatherData : public QObject
{
    Q_OBJECT
public:
    explicit WeatherData(QObject* parent = nullptr);

    QSettings *env;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;

    void writeApiKeyToEnvFile(QString apikey);
    QString readApiKeyFromEnvFile();

    void getWeatherData();
    void handleReply();
};

#endif // WEATHERDATA_H
