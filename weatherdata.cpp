#include "weatherdata.h"

WeatherData::WeatherData(QObject *parent) : QObject{parent}
{
    env = new QSettings(".env", QSettings::IniFormat);
}

void WeatherData::writeApiKeyToEnvFile(QString apikey) {
    env->setValue("API_KEY", apikey);
}

QString WeatherData::readApiKeyFromEnvFile() {
    return env->value("API_KEY").toString();
}
