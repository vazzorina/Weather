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

    lat = readLatFromEnvFile();
    lon = readLonFromEnvFile();
    address = readAddressFromEnvFile();
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
void WeatherData::saveLocationFromQml(double latitude, double lontitude, const QString &addr) {
    lat = latitude;
    lon = lontitude;
    address = addr;
    emit savedLocation();
}

void WeatherData::writeLocationToEnvFile() {
    env->setValue("LAT", lat);
    env->setValue("LON", lon);
    env->setValue("ADDRESS", address);

}

void WeatherData::getWeatherData() {
    if (reply) {
        qDebug() << "Прошлый запрос еще не завершился. Отменяем его...";

        weatherModel->clear();

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
        weatherModel->clear(); // чистим модель от предыдущих данных

        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonObject rootObj = jsonDoc.object();

        // получаем данные о погоде в текущий момент
        QJsonObject currentObj = rootObj["current"].toObject(); // массив с данными о текущем состоянии погоды
        int currentTemp = static_cast<int>(currentObj["temp_c"].toDouble()); // текущая температура
        QString currentCondition = currentObj["condition"].toObject()["text"].toString(); // описание текущей погоды
        QString mainIconCode = "qrc:qt/qml/Weather/images/" + QString::number(currentObj["condition"].toObject()["code"].toInt()) + ".png"; // сразу прописываем путь до нужно икокнки погоды
        QString nameLocation = rootObj["location"].toObject()["name"].toString(); // название местоположения

        // передаем текущую погоду в qml интерфейс
        mngWD->setCityName(nameLocation);
        mngWD->setCondition(currentCondition);
        mngWD->setTempNow(QString::number(static_cast<int>(currentTemp)));
        mngWD->setIconPath(mainIconCode);


        // получаем данные о будующей погоде для текущего дня
        QJsonObject forecastObj = rootObj["forecast"].toObject();
        QJsonArray forecastDayArray = forecastObj["forecastday"].toArray();

        if (!forecastDayArray.isEmpty()) {
            QJsonObject todayObj = forecastDayArray.at(0).toObject();
            QJsonArray hoursArray = todayObj["hour"].toArray(); // получаем массив с 24-х часовым прогнозом погоды

            for (int i = 0; i < hoursArray.size(); ++i) {
                QJsonObject hourObj = hoursArray.at(i).toObject();

                // получаем время и выводим в формате (00:00 - часы и минуты)
                QString time = hourObj["time"].toString().right(5);
                int temp = static_cast<int>(hourObj["temp_c"].toDouble()); // получаем температуру для i-го часа
                QString conditionText = hourObj["condition"].toObject()["text"].toString(); // получаем текстовое описание погоды
                QString iconPath = "qrc:qt/qml/Weather/images/" + QString::number(hourObj["condition"].toObject()["code"].toInt()) + ".png";

                // создаем элемент модели и передаем в модель для qml
                WeatherItem w_item {time, temp, iconPath};
                weatherModel->addWeatherItem(w_item);
            }

            qDebug() << "Заполнение модели закончено";

        }
    } else {
        qDebug() << "Ошибка запроса:" << reply->errorString();
    }
    reply->deleteLater();
    reply = nullptr;
}
