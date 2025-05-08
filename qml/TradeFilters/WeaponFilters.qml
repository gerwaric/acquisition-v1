import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    Layout.fillWidth: true
    Button {
        text: "Weapon Filters"
        Layout.fillWidth: true
        onClicked: filters.visible = !filters.visible
    }
    GridLayout {
        id: filters
        visible: false
        Layout.fillWidth: true
        Layout.margins: 5
        columns: 3

        Label { text: "Damage" }
        TextField { placeholderText: "Min"; Layout.preferredWidth: 40; inputMethodHints: Qt.ImhFormattedNumbersOnly }
        TextField { placeholderText: "Max"; Layout.preferredWidth: 40; inputMethodHints: Qt.ImhFormattedNumbersOnly }

        Label { text: "Critical Chance" }
        TextField { placeholderText: "Min"; Layout.preferredWidth: 40; inputMethodHints: Qt.ImhFormattedNumbersOnly }
        TextField { placeholderText: "Max"; Layout.preferredWidth: 40; inputMethodHints: Qt.ImhFormattedNumbersOnly }

        Label { text: "Physical DPS" }
        TextField { placeholderText: "Min"; Layout.preferredWidth: 40; inputMethodHints: Qt.ImhFormattedNumbersOnly }
        TextField { placeholderText: "Max"; Layout.preferredWidth: 40; inputMethodHints: Qt.ImhFormattedNumbersOnly }

        Label { text: "Attacks per Second" }
        TextField { placeholderText: "Min"; Layout.preferredWidth: 40; inputMethodHints: Qt.ImhFormattedNumbersOnly }
        TextField { placeholderText: "Max"; Layout.preferredWidth: 40; inputMethodHints: Qt.ImhFormattedNumbersOnly }

        Label { text: "Damage per Second" }
        TextField { placeholderText: "Min"; Layout.preferredWidth: 40; inputMethodHints: Qt.ImhFormattedNumbersOnly }
        TextField { placeholderText: "Max"; Layout.preferredWidth: 40; inputMethodHints: Qt.ImhFormattedNumbersOnly }

        Label { text: "Elemental DPS" }
        TextField { placeholderText: "Min"; Layout.preferredWidth: 40; inputMethodHints: Qt.ImhFormattedNumbersOnly }
        TextField { placeholderText: "Max"; Layout.preferredWidth: 40; inputMethodHints: Qt.ImhFormattedNumbersOnly }

        Label { text: "Chaos DPS" }
        TextField { placeholderText: "Min"; Layout.preferredWidth: 40; inputMethodHints: Qt.ImhFormattedNumbersOnly }
        TextField { placeholderText: "Max"; Layout.preferredWidth: 40; inputMethodHints: Qt.ImhFormattedNumbersOnly }
    }
}
