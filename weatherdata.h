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
    explicit WeatherData(ManagerWeatherData *managerWeather, WeatherModel *model, QObject* parent = nullptr);

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
    void readXYWindow();
    Q_INVOKABLE void writeXWindow(int newX);
    Q_INVOKABLE void writeYWindow(int newY);
    void checkingEnvFile();

    void getWeatherData();
    void handleReply();
    double lat;
    double lon;
    QString address;
    int x, y;
    QString locale;

signals:
    void savedLocation();
};

#endif // WEATHERDATA_H
