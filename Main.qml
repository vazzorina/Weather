import QtQuick
import QtQuick.Window 2.2

Window {
    property int screenX: Screen.desktopAvailableWidth
    property int screenY: Screen.desktopAvailableHeight

    id: wind
    width: 250
    height: 400
    x: screenX - width
    y: 0

    visible: true
    color: "#fff"
    flags: Qt.FramelessWindowHint | Qt.Tool //убирает рамки | не отображает в панели задач

    DragHandler {
        onActiveChanged: {
            if (active) {
                wind.startSystemMove()
            } else {
                if (wind.x + wind.width > screenX) wind.x = screenX - wind.width
                if (wind.x < 0) wind.x = 0
                if (wind.y + wind.height > screenY) wind.y = screenY - wind.height
                if (wind.y < 0) wind.y = 0
                managerMovingIcons.repositionDesktopIcons(wind.x, wind.y, wind.width, wind.height)
            }
        }
    }
}
