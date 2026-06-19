#ifndef SETTINGSTRAYMENU_H
#define SETTINGSTRAYMENU_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>

#include "weatherdata.h"
#include "translationmanager.h"

class SettingsTrayMenu : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsTrayMenu(WeatherData *weather, TranslationManager *translate, QWidget *parent = nullptr);
    QSystemTrayIcon *systemTrayIcon;
    QMenu *menuTrayIcon;
    QLineEdit *apiLineEdit;

    QAction *actionSettings;
    QAction *actionSearchCity;
    QAction *actionExit;

    QWidget *window;
    QLabel *titleLabel;
    QString infoText;
    QLabel *infoLabel;
    QLabel *languageLabel;
    QTabWidget *tabWidget;
    QLabel *latLabel;
    QLabel *lonLabel;
    QLabel *addressLabel;
    QPushButton *saveButton;
    QPushButton *cancelButton;

    WeatherData *weatherData = nullptr;
    QLineEdit *latEdit;
    QLineEdit *lonEdit;
    QLineEdit *addressEdit;
    QStringList languageList {"ru", "en"};
    QComboBox *languageComboBox;


    TranslationManager *translator = nullptr;

protected:
    void changeEvent(QEvent *event) override;

private:
    void saveAPI();
    void retranslateUi();
};

#endif // SETTINGSTRAYMENU_H
