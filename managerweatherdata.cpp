#include "managerweatherdata.h"
#include <QTimer>
#include <QDate>
#include <QLocale>

ManagerWeatherData::ManagerWeatherData(QObject *parent)
    : QObject{parent}
{
    updateDate();
    updateCurrentHour();
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        updateDate();
        updateCurrentHour();
    });
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

void ManagerWeatherData::setX_window(const int newX) {
    if (m_x_window == newX) return;
    m_x_window = newX;
    emit x_windowChanged();
}

void ManagerWeatherData::setY_window(const int newY) {
    if (m_y_window == newY) return;
    m_y_window = newY;
    emit y_windowChanged();
}



void ManagerWeatherData::updateDate() {
    QDate current = QDate::currentDate();
    QLocale lo(QLocale::Russian);
    QString formatted = lo.toString(current, "ddd, d MMMM");

    if (!formatted.isEmpty()) {
        formatted[0] = formatted[0].toUpper();
    }

    // eсли наступил новый день, меняем переменную и уведомляем QML
    if (m_currentDate != formatted) {
        m_currentDate = formatted;
        emit currentDateChanged();
    }
}

void ManagerWeatherData::updateCurrentHour() {
    int current = QTime::currentTime().hour();

    QString formatted = current < 10 ? "0" + QString::number(current) : QString::number(current);
    formatted += ":00";

    // eсли наступил новый день, меняем переменную и уведомляем QML
    if (m_currentHour != formatted) {
        m_currentHour = formatted;
        emit currentHourChanged();
    }
}
