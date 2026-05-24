#include <QApplication>
#include <QQmlApplicationEngine>
#include <windows.h>
#include <QQuickWindow>
#include <QQmlContext>


#include "managermovingicons.h"
#include "settingstraymenu.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    SettingsTrayMenu *stm = new SettingsTrayMenu();

    QObject::connect(stm->actionExit, &QAction::triggered, &app, QApplication::exit);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    ManagerMovingIcons mmi;
    engine.rootContext()->setContextProperty("managerMovingIcons", &mmi);

    engine.loadFromModule("Weather", "Main");


    if (engine.rootObjects().isEmpty())
        return -1;


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

    return app.exec();
}
