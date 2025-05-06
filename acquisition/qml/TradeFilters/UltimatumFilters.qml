import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    Layout.fillWidth: true
    Button {
        text: "Ultimatum Filters"
        Layout.fillWidth: true
        onClicked: filters.visible = !filters.visible
    }
    GridLayout {
        id: filters
        Layout.fillWidth: true
        Layout.margins: 5
        columns: 2
        visible: false

        Label { text: "Challenge Type" }
        ComboBox {
            //model: Acquisition.searchFilters.ultimatumChallengeTypes
        }

        Label { text: "Reward Type" }
        ComboBox {
            //model: Acquisition.searchFilters.ultimatumRewardTypes
        }

        Label { text: "Required Item" }
        ComboBox {
            //model: Acquisition.searchFilters.ultimatumRequiredItems
        }

        Label { text: "Reward Unique" }
        ComboBox {
            //model: Acquisition.searchFilters.ultimatumRewardUniques
        }
    }
}
