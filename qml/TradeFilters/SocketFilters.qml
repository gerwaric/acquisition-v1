import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    Layout.fillWidth: true
    Button {
        text: "Socket Filters"
        Layout.fillWidth: true
        onClicked: filters.visible = !filters.visible
    }
    GridLayout {
        id: filters
        Layout.fillWidth: true
        Layout.margins: 5
        columns: 7
        visible: false

        Label { text: "Sockets" }
        TextField { placeholderText: "R"; Layout.preferredWidth: 20; inputMethodHints: Qt.ImhDigitsOnly }
        TextField { placeholderText: "G"; Layout.preferredWidth: 20; inputMethodHints: Qt.ImhDigitsOnly }
        TextField { placeholderText: "B"; Layout.preferredWidth: 20; inputMethodHints: Qt.ImhDigitsOnly }
        TextField { placeholderText: "W"; Layout.preferredWidth: 20; inputMethodHints: Qt.ImhDigitsOnly }
        TextField { placeholderText: "Min"; Layout.preferredWidth: 40; inputMethodHints: Qt.ImhDigitsOnly }
        TextField { placeholderText: "Max"; Layout.preferredWidth: 40; inputMethodHints: Qt.ImhDigitsOnly }

        Label { text: "Links" }
        TextField { placeholderText: "R"; Layout.preferredWidth: 20; inputMethodHints: Qt.ImhDigitsOnly }
        TextField { placeholderText: "G"; Layout.preferredWidth: 20; inputMethodHints: Qt.ImhDigitsOnly }
        TextField { placeholderText: "B"; Layout.preferredWidth: 20; inputMethodHints: Qt.ImhDigitsOnly }
        TextField { placeholderText: "W"; Layout.preferredWidth: 20; inputMethodHints: Qt.ImhDigitsOnly }
        TextField { placeholderText: "Min"; Layout.preferredWidth: 40; inputMethodHints: Qt.ImhDigitsOnly }
        TextField { placeholderText: "Max"; Layout.preferredWidth: 40; inputMethodHints: Qt.ImhDigitsOnly }
    }
}
