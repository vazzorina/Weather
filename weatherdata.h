#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <QObject>
#include <QSettings>
#include <QNetworkAccessManager>
#include "managerweatherdata.h"

class WeatherData : public QObject
{
    Q_OBJECT
public:
    explicit WeatherData(QObject* parent = nullptr);

    ManagerWeatherData *mngWD = nullptr;
    WeatherModel *weatherModel = nullptr;

    QSettings *env;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;

    void writeApiKeyToEnvFile(QString apikey);
    QString readApiKeyFromEnvFile();
    double readLatFromEnvFile();
    double readLonFromEnvFile();
    QString readAddressFromEnvFile();
    Q_INVOKABLE void saveLocationFromQml(double latitude, double lontitude, const QString &addr);
    void writeLocationToEnvFile();

    void getWeatherData();
    void handleReply();
    double lat;
    double lon;
    QString address;

signals:
    void savedLocation();
};

#endif // WEATHERDATA_H
