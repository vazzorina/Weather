#include "settingstraymenu.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextBrowser>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>


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

    actionExit = new QAction();
    actionExit->setText("Закрыть виджет");

    menuTrayIcon->addAction(actionSearchCity);
    menuTrayIcon->addAction(actionSettings);
    menuTrayIcon->addSeparator();
    menuTrayIcon->addAction(actionExit);

    window = new QDialog();
    window->setVisible(false);
    window->setWindowTitle(tr("Настройки Weather"));
    window->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *mainLayout = new QVBoxLayout(window);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(15);

    // --- ВЕРХНИЙ БЛОК: Поле ввода API-ключа ---
    QWidget *topCard = new QWidget(window);
    QVBoxLayout *topCardLayout = new QVBoxLayout(topCard);
    topCardLayout->setSpacing(5);

    QLabel *titleLabel = new QLabel(tr("API-ключ сервиса OpenWeather"), topCard);

    apiLineEdit = new QLineEdit(topCard);
    apiLineEdit->setPlaceholderText(tr("Введите API-ключ..."));

    topCardLayout->addWidget(titleLabel);
    topCardLayout->addWidget(apiLineEdit);
    mainLayout->addWidget(topCard);

    // --- СРЕДНИЙ БЛОК: Инструкция ---
    QWidget *instructionCard = new QWidget(window);
    QVBoxLayout *instructionLayout = new QVBoxLayout(instructionCard);

    QLabel *infoLabel = new QLabel(instructionCard);
    infoLabel->setWordWrap(true);
    infoLabel->setOpenExternalLinks(true); // Позволяет кликать по ссылкам

    // Текст инструкции с HTML-разметкой для ссылок и списков
    QString infoText = tr(
        "<span style='color: #52616B;'>Чтобы получить API-ключ, необходимо:</span>"
        "<ol style='margin-top: 5px; margin-bottom: 0px; padding-left: 20px; color: #52616B;'>"
        "<li>Перейти по ссылке: <a href='https://home.openweathermap.org/users/sign_in' style='color: #0078D4;'>https://home.openweathermap.org/users/sign_in</a></li>"
        "<li>Зарегистрироваться и войти в аккаунт</li>"
        "<li>Перейти во вкладку Аккаунт -&gt; My API keys или перейти по ссылке: <a href='https://home.openweathermap.org/api_keys' style='color: #0078D4;'>https://home.openweathermap.org/api_keys</a></li>"
        "<li>Нажать кнопку Generate, чтобы сгенерировать ключ</li>"
        "<li>Скопировать сгенерированный API-ключ и ввести в поле выше</li>"
        "<li>Нажать кнопку сохранить.</li>"
        "</ol>"
        );
    infoLabel->setText(infoText);
    instructionLayout->addWidget(infoLabel);
    mainLayout->addWidget(instructionCard);

    // --- НИЖНИЙ БЛОК: Кнопка "Сохранить" ---
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);
    QPushButton *saveButton = new QPushButton(tr("Сохранить и закрыть"), window);
    QPushButton *cancelButton = new QPushButton(tr("Отмена"), window);
    saveButton->setCursor(Qt::PointingHandCursor);

    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);


    connect(actionSettings, &QAction::triggered, window, [=]() {
        window->setVisible(true);
        QString apikey = "";
        apikey = weatherData->readApiKeyFromEnvFile();
        if (apikey != "") apiLineEdit->setText(apikey);
    });
    connect(saveButton, &QPushButton::clicked, this, &SettingsTrayMenu::saveAPI);
    connect(cancelButton, &QPushButton::clicked, window, [=]() {
        apiLineEdit->clear();
        window->close();
    });
    systemTrayIcon->show();
}

void SettingsTrayMenu::saveAPI() {
    if (apiLineEdit->text().isEmpty()) {
        QMessageBox::warning(window,
                            tr("Не введен API-ключ!"),
                            tr("Чтобы сохранить API-ключ, сначала введите его в соответствующее поле"),
                            QMessageBox::Ok);
        apiLineEdit->setFocus();
    }
    else {
        weatherData->writeApiKeyToEnvFile(apiLineEdit->text());
        window->close();
    }
}



