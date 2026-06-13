#ifndef SETTINGSTRAYMENU_H
#define SETTINGSTRAYMENU_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QLineEdit>
#include <QPushButton>

#include "weatherdata.h"

class SettingsTrayMenu : public QObject
{
    Q_OBJECT
public:
    explicit SettingsTrayMenu(WeatherData *weather, QObject *parent = nullptr);
    QSystemTrayIcon *systemTrayIcon;
    QMenu *menuTrayIcon;
    QLineEdit *apiLineEdit;

    QAction *actionSettings;
    QAction *actionSearchCity;
    QAction *actionExit;

    QWidget *window;

    WeatherData *weatherData = nullptr;
    QLineEdit *latEdit;
    QLineEdit *lonEdit;
    QLineEdit *addressEdit;

private:
    void saveAPI();
};

#endif // SETTINGSTRAYMENU_H
