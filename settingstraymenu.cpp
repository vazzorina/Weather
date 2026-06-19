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
#include <QComboBox>
#include <QEvent>

SettingsTrayMenu::SettingsTrayMenu(WeatherData *weather, TranslationManager *translate, QWidget *parent)
    : QWidget{parent}, weatherData{weather}, translator{translate}
{
    // --- ТРЕЙ (без изменений) ---
    systemTrayIcon = new QSystemTrayIcon();
    systemTrayIcon->setIcon(QIcon(":qt/qml/Weather/images/tray-icon.png"));
    systemTrayIcon->setToolTip("Weather");

    menuTrayIcon = new QMenu();
    systemTrayIcon->setContextMenu(menuTrayIcon);

    actionSettings = new QAction();


    actionSearchCity = new QAction();


    actionExit = new QAction();


    menuTrayIcon->addAction(actionSearchCity);
    menuTrayIcon->addAction(actionSettings);
    menuTrayIcon->addSeparator();
    menuTrayIcon->addAction(actionExit);

    // --- ГЛАВНОЕ ОКНО ---
    window = new QDialog();
    window->setVisible(false);

    window->resize(500, 450); // Удобный размер для вкладок

    // Главный слой — снова вертикальный
    QVBoxLayout *mainLayout = new QVBoxLayout(window);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(15);

    // СОЗДАЕМ ТАБ-ВИДЖЕТ (Панель закладок)
    tabWidget = new QTabWidget(window);
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
    titleLabel = new QLabel(topCard);
    apiLineEdit = new QLineEdit(topCard);

    QString apikey = weatherData->readApiKeyFromEnvFile();
    if (!apikey.isEmpty()) apiLineEdit->setText(apikey);

    topCardLayout->addWidget(titleLabel);
    topCardLayout->addWidget(apiLineEdit);
    apiLayout->addWidget(topCard);

    // Средний блок: Инструкция
    QWidget *instructionCard = new QWidget(apiPage);
    QVBoxLayout *instructionLayout = new QVBoxLayout(instructionCard);
    infoLabel = new QLabel(instructionCard);
    infoLabel->setWordWrap(true);
    infoLabel->setOpenExternalLinks(true);

    instructionLayout->addWidget(infoLabel);
    apiLayout->addWidget(instructionCard);
    apiLayout->addStretch(); // Прижимает элементы к верху вкладки

    QHBoxLayout *languageLayout = new QHBoxLayout(apiPage);
    languageLabel = new QLabel();

    languageComboBox = new QComboBox();
    languageComboBox->addItems(languageList);

    languageLayout->addWidget(languageLabel);
    languageLayout->addWidget(languageComboBox);
    languageLayout->setSpacing(5);
    languageLayout->setContentsMargins(10, 10, 10, 10);
    languageLayout->addStretch();
    apiLayout->addLayout(languageLayout);


    // Добавляем страницу в качестве первой закладки
    tabWidget->addTab(apiPage, "");


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
    latLabel = new QLabel();
    latEdit = new QLineEdit();
    latEdit->setReadOnly(true);        // Только чтение
    latLayout->addWidget(latLabel, 0); // Текст занимает место по минимуму
    latLayout->addWidget(latEdit, 1);  // Поле ввода забирает всё оставшееся пространство
    mainLocationLayout->addLayout(latLayout);

    // --- Строка Широты ---
    QHBoxLayout *lonLayout = new QHBoxLayout();
    lonLabel = new QLabel();
    lonEdit = new QLineEdit();
    lonEdit->setReadOnly(true);        // Только чтение
    lonLayout->addWidget(lonLabel, 0); // Текст по минимуму
    lonLayout->addWidget(lonEdit, 1);  // Поле ввода на всю ширину
    mainLocationLayout->addLayout(lonLayout);

    // --- Строка Адреса ---
    QHBoxLayout *addressLayout = new QHBoxLayout();
    addressLabel = new QLabel();
    addressEdit = new QLineEdit();
    addressEdit->setReadOnly(true);            // Только чтение
    addressLayout->addWidget(addressLabel, 0); // Текст по минимуму
    addressLayout->addWidget(addressEdit, 1);  // Поле ввода на всю ширину
    mainLocationLayout->addLayout(addressLayout);

    addressEdit->setText(weatherData->address);
    latEdit->setText(QString::number(weatherData->lat));
    lonEdit->setText(QString::number(weatherData->lon));

    // 4. Добавляем собранную страницу со всемдержимым внутрь вкладки
    tabWidget->addTab(locationPage, "");


    // ==========================================
    // НИЖНИЙ БЛОК: Кнопки (Под закладками)
    // ==========================================
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);
    saveButton = new QPushButton(window);
    cancelButton = new QPushButton(window);
    saveButton->setFocusPolicy(Qt::NoFocus);
    cancelButton->setFocusPolicy(Qt::NoFocus);

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
        languageComboBox->setCurrentText(translator->readLangFromEnvFile());
        apiLineEdit->setText(weatherData->readApiKeyFromEnvFile());
        window->setVisible(true);
    });

    // Клик по "Выбрать местоположение" в трее -> открываем окно сразу на второй вкладке (индекс 1)
    connect(actionSearchCity, &QAction::triggered, window, [=]() {
        tabWidget->setCurrentIndex(1);
        languageComboBox->setCurrentText(translator->readLangFromEnvFile());
        apiLineEdit->setText(weatherData->readApiKeyFromEnvFile());
        window->setVisible(true);
    });

    // Кнопки Сохранить / Отмена
    connect(saveButton, &QPushButton::clicked, this, &SettingsTrayMenu::saveAPI);
    connect(cancelButton, &QPushButton::clicked, window, [=]() { window->close(); });

    connect(weatherData, &WeatherData::savedLocation, this, [=]() {
        addressEdit->setText(weatherData->address);
        latEdit->setText(QString::number(weatherData->lat));
        lonEdit->setText(QString::number(weatherData->lon));
    });

    systemTrayIcon->show();
    retranslateUi();
}

void SettingsTrayMenu::saveAPI() {
    QString adr = weatherData->readAddressFromEnvFile();
    double lat = weatherData->readLatFromEnvFile();
    double lon = weatherData->readLonFromEnvFile();

    if (apiLineEdit->text().isEmpty() or latEdit->text() == "0" or lonEdit->text() == "0" or addressEdit->text().isEmpty()) {
        QMessageBox::warning(window,
                            tr("Не введен API-ключ или не указано местоположение!"),
                            tr("Чтобы сохранить API-ключ, сначала введите его в соответствующее поле.\n"
                                "Чтобы указать местоположение, выберите точку на карте или введите адрес."),
                            QMessageBox::Ok);
        apiLineEdit->setFocus();
    }
    else {
        if (weatherData->lat != lat or latEdit->text() != QString::number(lat) or
            weatherData->lon != lon or lonEdit->text() != QString::number(lon) or
            weatherData->address != adr or addressEdit->text() != adr) {
            weatherData->writeLocationToEnvFile();
        }
        if (apiLineEdit->text() != weatherData->readApiKeyFromEnvFile()){
            weatherData->writeApiKeyToEnvFile(apiLineEdit->text());
        }
        translator->writeLangToEnvFile(languageComboBox->currentText());
        translator->setLanguage();
        weatherData->getWeatherData();

        window->close();
    }
}

void SettingsTrayMenu::changeEvent(QEvent *event)
{
    // Обязательно вызываем базовый класс
    QWidget::changeEvent(event);

    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();
    }
}

void SettingsTrayMenu::retranslateUi() {
    actionSettings->setText(tr("Настройки"));
    actionSearchCity->setText(tr("Выбрать местоположение"));
    actionExit->setText(tr("Закрыть виджет"));
    window->setWindowTitle(tr("Настройки Weather"));
    titleLabel->setText(tr("API-ключ сервиса WeatherAPI"));
    apiLineEdit->setPlaceholderText(tr("Введите API-ключ..."));
    infoText = tr(
        "<span style='color: #52616B;'>Чтобы получить API-ключ, необходимо:</span>"
        "<ol style='margin-top: 5px; margin-bottom: 0px; padding-left: 20px; color: #52616B;'>"
        "<li>Перейти по ссылке: <a href='https://www.weatherapi.com/signup.aspx' style='color: #0078D4;'>https://www.weatherapi.com/signup.aspx</a></li>"
        "<li>Зарегистрироваться и войти в аккаунт</li>"
        "<li>Перейти во вкладку API или перейти по ссылке: <a href='https://www.weatherapi.com/my/' style='color: #0078D4;'>https://www.weatherapi.com/my/</a></li>"
        "<li>Найти сверху страницы автоматически сгенерированный API key</li>"
        "<li>Нажать на кнопку Copy и вставить API-ключ в поле выше</li>"
        "<li>Нажать кнопку Сохранить и закрыть</li>"
        "</ol>");
    infoLabel->setText(infoText);
    languageLabel->setText(tr("Язык: "));
    tabWidget->setTabText(0, tr("API-ключ"));
    tabWidget->setTabText(1, tr("Местоположение"));
    latLabel->setText(tr("Долгота: "));
    lonLabel->setText(tr("Широта: "));
    addressLabel->setText(tr("Адрес: "));
    saveButton->setText(tr("Сохранить и закрыть"));
    cancelButton->setText(tr("Отмена"));


}

