#include "weatherdata.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

WeatherData::WeatherData(QObject *parent) : QObject{parent}
{
    env = new QSettings(".env", QSettings::IniFormat);
    manager = new QNetworkAccessManager();
    reply = nullptr;
}

void WeatherData::writeApiKeyToEnvFile(QString apikey) {
    env->setValue("API_KEY", apikey);
}

QString WeatherData::readApiKeyFromEnvFile() {
    return env->value("API_KEY").toString();
}

double WeatherData::readLonFromEnvFile() {
    return env->value("LON").toDouble();
}

double WeatherData::readLatFromEnvFile() {
    return env->value("LAT").toDouble();
}

QString WeatherData::readAddressFromEnvFile() {
    return env->value("ADDRESS").toString();
}

void WeatherData::writeLocationToEnvFile(double lat, double lon, const QString &addr) {
    env->setValue("LAT", lat);
    env->setValue("LON", lon);
    env->setValue("ADDRESS", addr);
    emit savedLocation();
}

void WeatherData::getWeatherData() {
    if (reply) {
        qDebug() << "Прошлый запрос еще не завершился. Отменяем его...";

        // отключаем сигналы, чтобы лямбда старого запроса не сработала при аборте
        reply->disconnect();

        reply->abort();      // прерываем сетевой поток
        reply->deleteLater(); // удаляем объект из памяти
        reply = nullptr;
    }

    QUrl url("https://api.weatherapi.com/v1/forecast.json");
    QString location = QString::number(readLatFromEnvFile()) + "," + QString::number(readLonFromEnvFile());
    QUrlQuery query;
    query.addQueryItem("key", readApiKeyFromEnvFile());
    query.addQueryItem("q", location);
    query.addQueryItem("days", "1");
    query.addQueryItem("lang", "ru");
    url.setQuery(query);

    QNetworkRequest request(url);
    qDebug() << "Отправка запроса на WeatherAPI:" << url.toString();

    reply = manager->get(request);

    QObject::connect(reply, &QNetworkReply::finished, this, &WeatherData::handleReply);
}


void WeatherData::handleReply() {
    if (!reply) return;

    if (reply->error() == QNetworkReply::OperationCanceledError) {
        qDebug() << "Запрос был успешно отменен.";
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonObject rootObj = jsonDoc.object();

        // получаем данные о погоде в текущий момент
        QJsonObject currentObj = rootObj["current"].toObject();
        double currentTemp = currentObj["temp_c"].toDouble();
        QString currentCondition = currentObj["condition"].toObject()["text"].toString();

        QJsonObject nameObj = rootObj["location"].toObject();
        QString nameLocation = nameObj["name"].toString();

        qDebug() << "=== СЕЙЧАС в " << nameLocation << " ===";
        qDebug() << "Текущая температура:" << currentTemp << "°C";
        qDebug() << "Текущее состояние:" << currentCondition << "\n";

        // получаем данные о будующей погоде для текущего дня
        QJsonObject forecastObj = rootObj["forecast"].toObject();
        QJsonArray forecastDayArray = forecastObj["forecastday"].toArray();

        if (!forecastDayArray.isEmpty()) {
            QJsonObject todayObj = forecastDayArray.at(0).toObject();
            QJsonArray hoursArray = todayObj["hour"].toArray(); // получаем массив с 24-х часовым прогнозом погоды

            qDebug() << "=== ПОЧАСОВОЙ ПРОГНОЗ НА СЕГОДНЯ ===";

            for (int i = 0; i < hoursArray.size(); ++i) {
                QJsonObject hourObj = hoursArray.at(i).toObject();

                // получаем время и выводим в формате (00:00 - часы и минуты)
                QString timeString = hourObj["time"].toString();
                QString formattedTime = timeString.right(5);

                double temp = hourObj["temp_c"].toDouble(); // получаем температуру для i-го часа

                QString conditionText = hourObj["condition"].toObject()["text"].toString(); // получаем текстовое описание погоды

                // выводим отформатированные данные о погоде
                qDebug() << QString("[%1] : %2°C — %3")
                                .arg(formattedTime)
                                .arg(temp, 0, 'f', 1)
                                .arg(conditionText);
            }
            qDebug() << "======================================";

        }
    } else {
        qDebug() << "Ошибка запроса:" << reply->errorString();
    }
    reply->deleteLater();
    reply = nullptr;
}
