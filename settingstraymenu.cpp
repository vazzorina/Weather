#include "settingstraymenu.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextBrowser>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTabWidget> // Добавлено вместо QListWidget и QStackedWidget
#include <QQuickWidget>
#include <QQmlContext>

SettingsTrayMenu::SettingsTrayMenu(QObject *parent) : QObject{parent}
{
    // --- ТРЕЙ (без изменений) ---
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

    // --- ГЛАВНОЕ ОКНО ---
    window = new QDialog();
    window->setVisible(false);
    window->setWindowTitle(tr("Настройки Weather"));
    window->resize(500, 450); // Удобный размер для вкладок

    // Главный слой — снова вертикальный
    QVBoxLayout *mainLayout = new QVBoxLayout(window);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(15);

    // СОЗДАЕМ ТАБ-ВИДЖЕТ (Панель закладок)
    QTabWidget *tabWidget = new QTabWidget(window);
    mainLayout->addWidget(tabWidget);

    // ==========================================
    // ЗАКЛАДКА 1: API-Ключ
    // ==========================================
    QWidget *apiPage = new QWidget();
    QVBoxLayout *apiLayout = new QVBoxLayout(apiPage);
    apiLayout->setContentsMargins(10, 15, 10, 10); // Отступы внутри вкладки
    apiLayout->setSpacing(15);

    // Верхний блок: Ввод API
    QWidget *topCard = new QWidget(apiPage);
    QVBoxLayout *topCardLayout = new QVBoxLayout(topCard);
    topCardLayout->setSpacing(5);
    QLabel *titleLabel = new QLabel(tr("API-ключ сервиса WeatherAPI"), topCard);
    apiLineEdit = new QLineEdit(topCard);
    apiLineEdit->setPlaceholderText(tr("Введите API-ключ..."));
    QString apikey = weatherData->readApiKeyFromEnvFile();
    if (!apikey.isEmpty()) apiLineEdit->setText(apikey);

    topCardLayout->addWidget(titleLabel);
    topCardLayout->addWidget(apiLineEdit);
    apiLayout->addWidget(topCard);

    // Средний блок: Инструкция
    QWidget *instructionCard = new QWidget(apiPage);
    QVBoxLayout *instructionLayout = new QVBoxLayout(instructionCard);
    QLabel *infoLabel = new QLabel(instructionCard);
    infoLabel->setWordWrap(true);
    infoLabel->setOpenExternalLinks(true);
    QString infoText = tr(
        "<span style='color: #52616B;'>Чтобы получить API-ключ, необходимо:</span>"
        "<ol style='margin-top: 5px; margin-bottom: 0px; padding-left: 20px; color: #52616B;'>"
        "<li>Перейти по ссылке: <a href='https://www.weatherapi.com/signup.aspx' style='color: #0078D4;'>https://www.weatherapi.com/signup.aspx</a></li>"
        "<li>Зарегистрироваться и войти в аккаунт</li>"
        "<li>Перейти во вкладку API или перейти по ссылке: <a href='https://www.weatherapi.com/my/' style='color: #0078D4;'>https://www.weatherapi.com/my/</a></li>"
        "<li>Найти сверху страницы автоматически сгенерированный API key</li>"
        "<li>Нажать на кнопку Copy и вставить API-ключ в поле выше</li>"
        "<li>Нажать кнопку Сохранить и закрыть</li>"
        "</ol>"
        );
    infoLabel->setText(infoText);
    instructionLayout->addWidget(infoLabel);
    apiLayout->addWidget(instructionCard);
    apiLayout->addStretch(); // Прижимает элементы к верху вкладки

    // Добавляем страницу в качестве первой закладки
    tabWidget->addTab(apiPage, tr("API-ключ"));


    // ==========================================
    // ЗАКЛАДКА 2: Местоположение
    // ==========================================
    // 1. Создаем страницу виджета и ОДИН главный вертикальный слой для неё
    QWidget *locationPage = new QWidget();
    QVBoxLayout *mainLocationLayout = new QVBoxLayout(locationPage);
    mainLocationLayout->setContentsMargins(10, 10, 10, 10); // Отступы для всей страницы (можно настроить по вкусу)
    mainLocationLayout->setSpacing(8);                     // Расстояние между картой и полями ввода

    // 2. Создаем контейнер для QML карты
    QQuickWidget *quickWidget = new QQuickWidget(locationPage);

    // Настраиваем растяжение карты: она должна занимать максимум свободного места
    quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    quickWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Инициализируем класс-мост для связи с QML
    quickWidget->rootContext()->setContextProperty("weatherData", weatherData);
    quickWidget->rootContext()->setContextProperty("latCPP", weatherData->readLatFromEnvFile());
    quickWidget->rootContext()->setContextProperty("lonCPP", weatherData->readLonFromEnvFile());
    // Загружаем QML файл карты
    quickWidget->setSource(QUrl(QStringLiteral("qrc:/qt/qml/Weather/MapWidget.qml")));

    // Добавляем карту наверх нашей страницы
    mainLocationLayout->addWidget(quickWidget);


    // 3. БЛОК ПОЛЕЙ ВВОДA (Строго под картой на этой же странице)

    // --- Строка Долготы ---
    QHBoxLayout *latLayout = new QHBoxLayout();
    QLabel *latLabel = new QLabel("Долгота: ");
    latEdit = new QLineEdit();
    latEdit->setReadOnly(true);        // Только чтение
    latLayout->addWidget(latLabel, 0); // Текст занимает место по минимуму
    latLayout->addWidget(latEdit, 1);  // Поле ввода забирает всё оставшееся пространство
    mainLocationLayout->addLayout(latLayout);

    // --- Строка Широты ---
    QHBoxLayout *lonLayout = new QHBoxLayout();
    QLabel *lonLabel = new QLabel("Широта: ");
    lonEdit = new QLineEdit();
    lonEdit->setReadOnly(true);        // Только чтение
    lonLayout->addWidget(lonLabel, 0); // Текст по минимуму
    lonLayout->addWidget(lonEdit, 1);  // Поле ввода на всю ширину
    mainLocationLayout->addLayout(lonLayout);

    // --- Строка Адреса ---
    QHBoxLayout *addressLayout = new QHBoxLayout();
    QLabel *addressLabel = new QLabel("Адрес: ");
    addressEdit = new QLineEdit();
    addressEdit->setReadOnly(true);            // Только чтение
    addressLayout->addWidget(addressLabel, 0); // Текст по минимуму
    addressLayout->addWidget(addressEdit, 1);  // Поле ввода на всю ширину
    mainLocationLayout->addLayout(addressLayout);

    addressEdit->setText(weatherData->readAddressFromEnvFile());
    latEdit->setText(QString::number(weatherData->readLatFromEnvFile()));
    lonEdit->setText(QString::number(weatherData->readLonFromEnvFile()));

    // 4. Добавляем собранную страницу со всемдержимым внутрь вкладки
    tabWidget->addTab(locationPage, tr("Местоположение"));


    // ==========================================
    // НИЖНИЙ БЛОК: Кнопки (Под закладками)
    // ==========================================
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);
    QPushButton *saveButton = new QPushButton(tr("Сохранить и закрыть"), window);
    QPushButton *cancelButton = new QPushButton(tr("Отмена"), window);
    saveButton->setCursor(Qt::PointingHandCursor);

    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);

    // Кнопки добавляются в главный вертикальный слой, поэтому они всегда будут внизу окна
    mainLayout->addLayout(buttonLayout);


    // ==========================================
    // СИГНАЛЫ И СЛОТЫ
    // ==========================================

    // Клик по "Настройки" в трее -> открываем окно на первой вкладке (индекс 0)
    connect(actionSettings, &QAction::triggered, window, [=]() {
        tabWidget->setCurrentIndex(0);
        window->setVisible(true);
    });

    // Клик по "Выбрать местоположение" в трее -> открываем окно сразу на второй вкладке (индекс 1)
    connect(actionSearchCity, &QAction::triggered, window, [=]() {
        tabWidget->setCurrentIndex(1);
        window->setVisible(true);
    });

    // Кнопки Сохранить / Отмена
    connect(saveButton, &QPushButton::clicked, this, &SettingsTrayMenu::saveAPI);
    connect(cancelButton, &QPushButton::clicked, window, [=]() { window->close(); });

    connect(weatherData, &WeatherData::savedLocation, this, [=]() {
        addressEdit->setText(weatherData->readAddressFromEnvFile());
        latEdit->setText(QString::number(weatherData->readLatFromEnvFile()));
        lonEdit->setText(QString::number(weatherData->readLonFromEnvFile()));
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
        weatherData->getWeatherData();
        window->close();
    }
}



