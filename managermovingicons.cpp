#include "managermovingicons.h"
#include <windows.h>
#include <commctrl.h>
#include <QRect>
#include <QDebug>

ManagerMovingIcons::ManagerMovingIcons(QObject *parent)
    : QObject{parent}
{}


// Вспомогательная функция для поиска окна с иконками (SysListView32)
HWND ManagerMovingIcons::getDesktopListView() {
    HWND hwndListView = NULL;

    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        HWND p = FindWindowExW(hwnd, NULL, L"SHELLDLL_DefView", NULL);
        if (p != NULL) {
            *(HWND*)lParam = FindWindowExW(p, NULL, L"SysListView32", NULL);
        }
        return TRUE;
    }, (LPARAM)&hwndListView);

    return hwndListView;
}

// Главная функция: расталкивает иконки вокруг виджета
void ManagerMovingIcons::repositionDesktopIcons(int x, int y, int width, int height) {
    QRect widgetRect(x, y, width, height); // cобираем прямоугольник из переданных координат

    HWND hwndListView = getDesktopListView();
    if (!hwndListView) return;

    DWORD processId;
    GetWindowThreadProcessId(hwndListView, &processId); // запоминаем ID процесса проводника (explorer.exe)

    HANDLE hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, processId); // открываем процесс с правами чтения и записи
    if (!hProcess) {
        qWarning() << "Нет прав доступа к explorer.exe";
        return;
    }

    int itemCount = SendMessage(hwndListView, LVM_GETITEMCOUNT, 0, 0); // запоминаем количество иконок на рабочем столе

    // выделяем память в самом процессе проводника для хранения координат
    POINT* remotePoint = (POINT*)VirtualAllocEx(hProcess, NULL, sizeof(POINT), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remotePoint) {
        CloseHandle(hProcess);
        return;
    }

    POINT localPoint;
    for (int i = 0; i < itemCount; ++i) { // перебираем иконки
        SendMessage(hwndListView, LVM_GETITEMPOSITION, i, (LPARAM)remotePoint); // запоминаем координаты иконки в память проводника
        ReadProcessMemory(hProcess, remotePoint, &localPoint, sizeof(POINT), NULL); // читаем из памяти проводника координаты иконки в нашу программу

        if (widgetRect.contains(localPoint.x, localPoint.y)) {
            int newX = localPoint.x;
            int newY = widgetRect.bottom() + 20; // Выталкиваем иконку вниз
            SendMessage(hwndListView, LVM_SETITEMPOSITION, i, MAKELPARAM(newX, newY));
        }
    }

    VirtualFreeEx(hProcess, remotePoint, 0, MEM_RELEASE);
    CloseHandle(hProcess);
}



std::pair<int, int> ManagerMovingIcons::getDesktopIconsSpacing() {
    std::pair<int, int> dis (100, 100);
    HWND hwndListView = getDesktopListView();
    if (!hwndListView) return dis;

    LRESULT spacing = SendMessage(hwndListView, LVM_GETITEMSPACING, FALSE, 0); // запрашиваем у SysListView32 размер иконок

    dis.first = LOWORD(spacing); // щирина ячейки
    dis.second = HIWORD(spacing); // высота ячейки

    qDebug() << "Текущий размер ячейки иконки на рабочем столе:" << dis.first << "x" << dis.second;

    return dis;
}


int ManagerMovingIcons::getNewX(int qmlWindX, int qmlWindWidth, int localPointX) {

}

int ManagerMovingIcons::getNewY(int qmlWindY, int qmlWindHeight, int localPointY) {

}
