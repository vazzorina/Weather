import QtQuick

Window {
    id: settings
    visible: false
    width: 300
    height: 200
    title: qsTr("Настройки Weather")

    function showSettingsWindow() {
        settings.visible = true;
    }
}
