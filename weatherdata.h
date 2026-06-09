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
    Q_INVOKABLE void saveLocationFromQml(double latitude, double lontitude, const QString &addr);
    void writeLocationToEnvFile();

    void getWeatherData();
    void handleReply();
    double lat = 0;
    double lon = 0;
    QString address = "";

signals:
    void savedLocation();
};

#endif // WEATHERDATA_H
