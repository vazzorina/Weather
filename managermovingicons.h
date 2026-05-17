#ifndef MANAGERMOVINGICONS_H
#define MANAGERMOVINGICONS_H

#include <QObject>
#include <windows.h>
#include <commctrl.h>
#include <QRect>
#include <QDebug>


class ManagerMovingIcons : public QObject
{
    Q_OBJECT
public:
    explicit ManagerMovingIcons(QObject *parent = nullptr);
    Q_INVOKABLE void repositionDesktopIcons(int x, int y, int width, int height);
private:
    HWND getDesktopListView();
    int getNewX(int qmlWindX, int qmlWindWidth, int localPointX);
    int getNewY(int qmlWindY, int qmlWindHeight, int localPointY);
    std::pair<int, int> getDesktopIconsSpacing();
};

#endif // MANAGERMOVINGICONS_H
