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
    double readLatFromEnvFile();
    double readLonFromEnvFile();
    QString readAddressFromEnvFile();
    Q_INVOKABLE void writeLocationToEnvFile(double lat, double lon, const QString &addr = "");

    void getWeatherData();
    void handleReply();

signals:
    void savedLocation();
};

#endif // WEATHERDATA_H
