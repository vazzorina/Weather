#ifndef SETTINGSTRAYMENU_H
#define SETTINGSTRAYMENU_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QLineEdit>
#include <QPushButton>

class SettingsTrayMenu : public QObject
{
    Q_OBJECT
public:
    explicit SettingsTrayMenu(QObject *parent = nullptr);
    QSystemTrayIcon *systemTrayIcon;
    QMenu *menuTrayIcon;
    QLineEdit *apiLineEdit;

    QAction *actionSettings;
    QAction *actionSearchCity;
    QAction *actionExit;

    QWidget *window;


private:
    void saveAPI();
};

#endif // SETTINGSTRAYMENU_H
