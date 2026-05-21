#include "settingstraymenu.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>


SettingsTrayMenu::SettingsTrayMenu(QObject *parent) : QObject{parent}
{
    systemTrayIcon = new QSystemTrayIcon();
    systemTrayIcon->setIcon(QIcon(":qt/qml/Weather/images/tray-icon-32x32.png"));
    systemTrayIcon->setToolTip("Weather");

    menuTrayIcon = new QMenu();
    systemTrayIcon->setContextMenu(menuTrayIcon);

    actionSettings = new QAction();
    actionSettings->setText("Настройки");

    actionSearchCity = new QAction();
    actionSearchCity->setText("Выбрать местоположение");

    menuTrayIcon->addAction(actionSearchCity);
    menuTrayIcon->addAction(actionSettings);
}

void SettingsTrayMenu::showMenu() {
    systemTrayIcon->show();
}
