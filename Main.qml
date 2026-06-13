import QtQuick
import QtQuick.Window 2.2
import QtQuick.Layouts
import QtQuick.Controls

Window {
    property int screenX: Screen.desktopAvailableWidth
    property int screenY: Screen.desktopAvailableHeight

    property string mainFontFamily: "Arcade Jeu"
    property string mainFontColor: "#3f2f0d"

    id: wind
    width: 450
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

            // 3. Горизонтальный блок почасового прогноза со скроллом
            Flickable {
                Layout.preferredWidth: 410
                Layout.preferredHeight: 110 // Немного увеличили, чтобы поместился скроллбар, если он появится
                contentWidth: hourlyRow.implicitWidth // Важно: задает область прокрутки по ширине
                clip: true // Обрезает элементы, которые выходят за границы зоны

                RowLayout {
                    id: hourlyRow
                    anchors.fill: parent
                    spacing: 5

                    Repeater {
                        // Здесь вместо 5 в реальном приложении будет ваша модель данных (например, ListModel или массив из C++)
                        model: weatherModel

                        delegate: Rectangle {
                            width: 80
                            height: 110
                            color: "transparent"
                            radius: 15

                            Image {
                                source: "qrc:qt/qml/Weather/images/hour-background.png"
                                anchors.fill: parent
                                width: 450
                                height: 400
                            }

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 10
                                spacing: 2

                                Text {
                                    // modelData хранит индекс или объект, если используется сложная модель
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
