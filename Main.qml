import QtQuick
import QtQuick.Window 2.2
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window

Window {
    property int screenX: Screen.desktopAvailableWidth
    property int screenY: Screen.desktopAvailableHeight

    property string mainFontFamily: "Arcade Jeu"
    property string mainFontColor: "#3f2f0d"

    id: wind
    width: 460
    height: 400
    x: managerWeather.x_window
    y: managerWeather.y_window

    onXChanged: {
        weatherData.writeXWindow(wind.x)
    }

    onYChanged: {
        weatherData.writeYWindow(wind.y)
    }

    visible: true
    color: "transparent"
    flags: Qt.FramelessWindowHint | Qt.Tool //убирает рамки | не отображает в панели задач

    Rectangle {
        id: backgroundRect
        anchors.fill: parent
        color: "transparent"
        Image {
            source: "qrc:qt/qml/Weather/images/background.png"
            anchors.fill: parent
        }

        // Главный вертикальный слой для всего контента
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 10

            // 1. Заголовок города
            Text {
                text: managerWeather.cityName
                font.pixelSize: 12
                color: mainFontColor
                font.family: mainFontFamily
                Layout.alignment: Qt.AlignHCenter
            }

            // 2. Блок текущей погоды (Иконка + Температура и Описание)
            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 10
                Layout.alignment: Qt.AlignHCenter

                // Контейнер для большой иконки
                Rectangle {
                    width: 200
                    height: 200
                    color: "transparent"
                    Image {
                        source: "qrc:qt/qml/Weather/images/ramka.png"
                        fillMode: Image.PreserveAspectFit
                        Layout.alignment: Qt.AlignVCenter
                        anchors.centerIn: parent
                        Image {
                            source: managerWeather.iconPath
                            fillMode: Image.PreserveAspectFit
                            Layout.alignment: Qt.AlignVCenter
                            anchors.centerIn: parent
                        }
                    }
                }

                // Слой для температуры и текстового статуса
                ColumnLayout {
                    spacing: 10
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignTop
                    Layout.topMargin: 10
                    Text {
                        text: managerWeather.currentDate
                        font.pixelSize: 16
                        color: mainFontColor
                        font.family: mainFontFamily
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true // Позволяет тексту корректно переноситься по словам
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Text {
                        text: managerWeather.tempNow + "℃"
                        font.pixelSize: 48
                        color: mainFontColor
                        font.family: mainFontFamily
                        Layout.alignment: Qt.AlignHCenter
                        Layout.margins: 15
                    }

                    Text {
                        text: managerWeather.condition
                        font.pixelSize: 12
                        color: mainFontColor
                        font.family: mainFontFamily
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true // Позволяет тексту корректно переноситься по словам
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }

            ListView {
                id: weatherList
                Layout.preferredWidth: 420
                Layout.preferredHeight: 110
                orientation: ListView.Horizontal // Горизонтальная прокрутка
                clip: true
                spacing: 5
                model: weatherModel // Ваша модель данных

                snapMode: ListView.SnapOneItem

                highlightRangeMode: ListView.StrictlyEnforceRange

                delegate: Rectangle {
                    width: 80
                    height: 110
                    color: "transparent"
                    radius: 15

                    Image {
                        source: model.time === managerWeather.currentHour ? "qrc:qt/qml/Weather/images/current-hour-background.png" : "qrc:qt/qml/Weather/images/hour-background.png"
                        anchors.fill: parent
                    }

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 2

                        Text {
                            text: model.time
                            font.pixelSize: 12
                            font.family: mainFontFamily
                            color: mainFontColor
                            Layout.alignment: Qt.AlignHCenter
                        }

                        Image {
                            source: model.icon
                            Layout.preferredWidth: 60
                            Layout.preferredHeight: 60
                            fillMode: Image.Stretch
                            Layout.alignment: Qt.AlignHCenter
                        }

                        Text {
                            text: model.temp + "℃"
                            font.pixelSize: 14
                            font.bold: true
                            font.family: mainFontFamily
                            color: mainFontColor
                            Layout.alignment: Qt.AlignHCenter
                        }
                    }
                }
            }
        }

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
}
