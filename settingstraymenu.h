#ifndef SETTINGSTRAYMENU_H
#define SETTINGSTRAYMENU_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class SettingsTrayMenu : public QObject
{
    Q_OBJECT
public:
    explicit SettingsTrayMenu(QObject *parent = nullptr);
    QObject* rootObject = nullptr;
    void showMenu();

    QSystemTrayIcon *systemTrayIcon;
    QMenu *menuTrayIcon;

    QAction *actionSettings;
    QAction *actionSearchCity;
};

#endif // SETTINGSTRAYMENU_H
