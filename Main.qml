import QtQuick
import QtQuick.Window 2.2

Window {
    id: wind
    width: 250
    height: 400
    x: Screen.desktopAvailableWidth - width
    y: 0

    visible: true
    title: qsTr("Hello World")
    color: "#fff"
    flags: Qt.FramelessWindowHint |     // убирает рамки
        Qt.WindowStaysOnBottomHint | // размещает окно под всеми окнами
        Qt.Tool                      //не отображает в панели задач

    property int previousX
    property int previousY

    DragHandler {
        onActiveChanged: if (active) wind.startSystemMove()
    }
}
