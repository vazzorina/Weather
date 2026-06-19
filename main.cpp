#include <QApplication>
#include <QQmlApplicationEngine>
#include <windows.h>
#include <QQuickWindow>
#include <QQmlContext>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <iostream>

#include "managermovingicons.h"
#include "settingstraymenu.h"
#include "managerweatherdata.h"
#include "translationmanager.h"

static QMutex logMutex;
void weatherMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // ,локируем мьютекс на время выполнения функции
    QMutexLocker locker(&logMutex);

    QString levelText;
    switch (type) {
    case QtDebugMsg:    levelText = "DEBUG"; break;
    case QtInfoMsg:     levelText = "INFO"; break;
    case QtWarningMsg:  levelText = "WARNING"; break;
    case QtCriticalMsg: levelText = "CRITICAL"; break;
    case QtFatalMsg:    levelText = "FATAL"; break;
    }

    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

    QString logMessage = QString("[%1] [%2] %3 (File: %4, Line: %5)")
                             .arg(currentTime)
                             .arg(levelText)
                             .arg(msg)
                             .arg(QString(context.file ? context.file : "unknown"))
                             .arg(context.line);

    // запись в файл Weather.log
    QFile outFile("Weather.log");
    // файл открывается в режиме Append, что не сотрет старые логи
    if (outFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream textStream(&outFile);
        textStream << logMessage << "\n";
        outFile.close();
    }

    std::cerr << logMessage.toLocal8Bit().constData() << std::endl;

    if (type == QtFatalMsg) {
        abort();
    }
}



int main(int argc, char *argv[])
{
    qInstallMessageHandler(weatherMessageHandler); // устновка перехватчика

    qInfo() << "Запуск приложения Weather";

    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    TranslationManager *translator = new TranslationManager(&app);
    translator->setLanguage();

    app.setWindowIcon(QIcon("qrc:/images/tray-icon.ico"));

    ManagerWeatherData *managerWeather = new ManagerWeatherData(&app);
    WeatherModel *weatherModel = new WeatherModel(&app);
    WeatherData *weatherData = new WeatherData(managerWeather, weatherModel, &app);
    SettingsTrayMenu *stm = new SettingsTrayMenu(weatherData, translator);

    QObject::connect(stm->actionExit, &QAction::triggered, &app, QApplication::exit);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    QObject::connect(translator, &TranslationManager::languageChanged, &engine, [&engine]() {
        engine.retranslate();
    });

    QObject::connect(translator, &TranslationManager::languageChanged, &app, [=]() {

        //weatherData->getWeatherData();
    });

    ManagerMovingIcons mmi;
    engine.rootContext()->setContextProperty("managerMovingIcons", &mmi);
    engine.rootContext()->setContextProperty("weatherData", weatherData);
    engine.rootContext()->setContextProperty("managerWeather", managerWeather);
    engine.rootContext()->setContextProperty("weatherModel", weatherModel);
    engine.loadFromModule("Weather", "Main");


    if (engine.rootObjects().isEmpty()) {
        qFatal() << "Интрефейса QML не существует";
        return -1;
    }

    QQuickWindow *window = qobject_cast<QQuickWindow*>(engine.rootObjects().first());
    if (window) {
        HWND hwnd = (HWND)window->winId();

        HWND hwndProgman = FindWindowW(L"Progman", NULL); // находим главное окно диспетчера рабочего стола
        SendMessageTimeoutW(hwndProgman, 0x052C, 0, 0, SMTO_NORMAL, 1000, NULL); //отправляем спец.сообщение, после которого Windows разделяет рабочий стол на слои

        HWND hwndWorkerW = NULL;

        EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL { //перебираем окна, чтобы найти окно c классом SHELLDLL_DefView, где отобржаются иконки
            HWND p = FindWindowExW(hwnd, NULL, L"SHELLDLL_DefView", NULL); //ищем дискриптор нужного окна
            if (p != NULL) { //если дискриптор найден
                *(HWND*)lParam = hwnd;
            }
            return TRUE;
        }, (LPARAM)&hwndWorkerW);

        if (hwndWorkerW) {
            SetParent(hwnd, hwndWorkerW); //устанавливаем для нашего окна в качетсве родителя дискриптор найденного окна
        }
    }

    QObject::connect(&app, &QApplication::aboutToQuit, []() {
        qInfo() << "Приложение корректно завершает работу";
    });

    return app.exec();
}
