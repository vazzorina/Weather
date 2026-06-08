import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtPositioning
import QtLocation

Rectangle {
    id: mainRec
    width: 200
    height: 200

    // подключение плагина, который предоставляет карту
    Plugin {
        id: mapPlugin
        name: "osm"
        PluginParameter {
            name: "osm.useragent"
            value: "MyWeatherApp/1.0"
        }
    }

    // модель для геокодинга
    GeocodeModel {
        id: geocodeModel
        plugin: mapPlugin

        onStatusChanged: {
            if (status === GeocodeModel.Ready) {
                if (count > 0) {
                    var loc = get(0)
                    map.center = loc.coordinate
                    mapMarker.coordinate = loc.coordinate
                    weatherData.writeLocationToEnvFile(loc.coordinate.latitude, loc.coordinate.longitude, loc.address.text)
                } else {
                    errorPopup.show("Ничего не найдено", "К сожалению, указанный адрес не существует. Проверьте орфографию.")
                }
            } else if (status === GeocodeModel.Error) {
                errorPopup.show("Ошибка сети", "Не удалось связаться с сервером геокодинга. Ошибка: " + errorString)
            }
        }
    }

    // всплывающее окно ошибки при поиске адреса
    Rectangle {
        id: errorPopup
        anchors.fill: parent
        color: Qt.rgba(0, 0, 0, 0.6)
        visible: opacity > 0
        opacity: 0
        z: 10

        property string titleText: ""
        property string messageText: ""

        // функция вызова окна
        function show(title, message) {
            titleText = title
            messageText = message
            opacity = 1.0
        }

        // функция для закрытия окна
        function hide() {
            opacity = 0
        }

        // блокируем клики сквозь это окно, чтобы пользователь не мог нажимать на карту, пока горит ошибка
        MouseArea {
            anchors.fill: parent
            preventStealing: true
            propagateComposedEvents: false
        }

        Rectangle {
            anchors.centerIn: parent
            width: Math.min(parent.width * 0.85, 320)
            height: contentColumn.implicitHeight + 40

            color: Qt.rgba(255, 255, 255, 0.8)
            border.color: Qt.rgba(171, 185, 209, 0.9)
            border.width: 2
            radius: 20

            Column {
                id: contentColumn
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: 20
                spacing: 15

                // заголовок ошибки
                Text {
                    text: errorPopup.titleText
                    font.pixelSize: 16
                    font.bold: true
                    width: parent.width
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                }

                // текст сообщения
                Text {
                    text: errorPopup.messageText
                    font.pixelSize: 12
                    width: parent.width
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                }

                // кнопка ОК
                Rectangle {
                    id: errorButton
                    implicitWidth: 70
                    implicitHeight: 32
                    radius: 8
                    color: buttonMouseArea.pressed ? "#0066b3" : (buttonMouseArea.containsMouse ? "#004b85" : "#003a66")

                    Behavior on color {
                        ColorAnimation {
                            duration: 150
                        }
                    }

                    Text {
                        text: "OK"
                        color: "white"
                        font.bold: true
                        font.pixelSize: 12
                        anchors.centerIn: parent
                    }

                    MouseArea {
                        id: buttonMouseArea
                        anchors.fill: parent
                        hoverEnabled: true

                        onClicked: errorPopup.hide()
                    }
                }
            }
        }
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(latCPP, lonCPP)
        zoomLevel: 10
        focus: true

        // маркер на карте
        MapQuickItem {
            id: mapMarker
            coordinate: QtPositioning.coordinate(latCPP, lonCPP)
            anchorPoint.x: markerImage.width / 2
            anchorPoint.y: markerImage.height

            sourceItem: Image {
                id: markerImage
                source: "qrc:qt/qml/Weather/images/map-marker.png"
                width: 32
                height: 32
            }
        }

        // масштабирование карты + выбор координат с помощью маши
        MouseArea {
            anchors.fill: parent
            propagateComposedEvents: true

            onClicked: mouse => {
                searchField.text = ""
                var coord = map.toCoordinate(Qt.point(mouse.x, mouse.y))
                mapMarker.coordinate = coord
                geocodeModel.query = coord
                geocodeModel.update()
            }

            onWheel: wheel => {
                var zoomDelta = wheel.angleDelta.y > 0 ? 0.5 : -0.5
                var newZoom = map.zoomLevel + zoomDelta
                if (newZoom >= map.minimumZoomLevel && newZoom <= map.maximumZoomLevel) {
                    map.zoomLevel = newZoom
                }
            }
        }

        // жесты тачпада для ноутбуков
        PinchHandler {
            id: pinch
            target: null
            onActiveChanged: if (active) {
                map.startCentroid = map.toCoordinate(pinch.centroid.position, false)
            }
            onScaleChanged: delta => {
                var newZoom = map.zoomLevel + Math.log2(delta) * 0.1
                if (newZoom >= map.minimumZoomLevel && newZoom <= map.maximumZoomLevel) {
                    map.zoomLevel = newZoom
                    map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
                }
            }
        }

        // перетаскивание карты мышкой
        DragHandler {
            id: drag
            target: null
            onTranslationChanged: delta => map.pan(-delta.x, -delta.y)
        }

        // горячие клавиши для масштабирования
        Shortcut {
            enabled: map.zoomLevel < map.maximumZoomLevel
            sequences: [StandardKey.ZoomIn, "+"]
            onActivated: map.zoomLevel = Math.min(map.maximumZoomLevel, Math.round(map.zoomLevel + 1))
        }

        Shortcut {
            enabled: map.zoomLevel > map.minimumZoomLevel
            sequences: [StandardKey.ZoomOut, "-"]
            onActivated: map.zoomLevel = Math.max(map.minimumZoomLevel, Math.round(map.zoomLevel - 1))
        }
    }

    // поисковая строка на карте
    RowLayout {
        id: searchBar
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 15

        width: Math.min(parent.width * 0.8, 400)
        spacing: 8

        z: 10

        TextField {
            id: searchField
            Layout.fillWidth: true
            placeholderText: "Введите адрес для поиска..."
            color: "#222831"
            placeholderTextColor: "#9A9EAB"
            font.pixelSize: 12

            implicitHeight: 32
            leftPadding: 12
            verticalAlignment: Text.AlignVCenter

            onAccepted: {
                geocodeModel.query = searchField.text
                geocodeModel.update()
            }

            background: Rectangle {
                color: "#FFFFFF"
                border.color: searchField.activeFocus ? "#0078D4" : "#DCDCDC"
                border.width: searchField.activeFocus ? 2 : 1
                radius: 8

                layer.enabled: true
                layer.effect: ShaderEffect {}
            }
        }

        Rectangle {
            id: searchButton

            implicitWidth: 70
            implicitHeight: 32
            radius: 8

            color: buttonMouseArea2.pressed ? "#0066b3" : (buttonMouseArea2.containsMouse ? "#004b85" : "#003a66")

            Behavior on color {
                ColorAnimation {
                    duration: 150
                }
            }

            Text {
                text: "Найти"
                color: "white"
                font.bold: true
                font.pixelSize: 12
                anchors.centerIn: parent
            }

            MouseArea {
                id: buttonMouseArea2
                anchors.fill: parent
                hoverEnabled: true

                onClicked: {
                    geocodeModel.query = searchField.text
                    geocodeModel.update()
                }
            }
        }
    }
}
