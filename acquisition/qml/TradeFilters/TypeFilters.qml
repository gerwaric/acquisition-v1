import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    Layout.fillWidth: true
    Button {
        text: "Type Filters"
        Layout.fillWidth: true
        onClicked: filters.visible = !filters.visible
    }
    GridLayout {
        id: filters
        visible: false
        Layout.fillWidth: true
        Layout.margins: 5
        columns: 2

        Label { text: "Item Category"; }
        ComboBox {
            //model: Acquisition.searchFilters.itemCategories;
            //onActivated: Acquisition.search.setItemCategory(text);
        }

        Label { text: "Item Rarity" }
        ComboBox {
            //model: Acquisition.searchFilters.itemRarities
        }
    }
}
