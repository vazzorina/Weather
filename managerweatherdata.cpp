#include "managerweatherdata.h"
#include <QTimer>
#include <QDate>
#include <QLocale>

ManagerWeatherData::ManagerWeatherData(QObject *parent)
    : QObject{parent}
{
    updateDate();
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ManagerWeatherData::updateDate);
    timer->start(60000);
}

void ManagerWeatherData::setCityName(const QString newCityName) {
    if (m_cityName == newCityName) return;
    m_cityName = newCityName;
    emit cityNameChanged();
}

void ManagerWeatherData::setTempNow(const QString newTempNow) {
    if (m_tempNow == newTempNow) return;
    m_tempNow = newTempNow;
    emit tempNowChanged();
}

void ManagerWeatherData::setCondition(const QString newCondition) {
    if (m_condition == newCondition) return;
    m_condition = newCondition;
    emit conditionChanged();
}

void ManagerWeatherData::setIconPath(const QString newIconPath) {
    if (m_iconPath == newIconPath) return;
    m_iconPath = newIconPath;
    emit iconPathChanged();
}

void ManagerWeatherData::updateDate() {
    QDate current = QDate::currentDate();
    QLocale ru(QLocale::Russian);
    QString formatted = ru.toString(current, "ddd, d MMMM");

    if (!formatted.isEmpty()) {
        formatted[0] = formatted[0].toUpper();
    }

    // eсли наступил новый день, меняем переменную и уведомляем QML
    if (m_currentDate != formatted) {
        m_currentDate = formatted;
        emit currentDateChanged();
    }
}
