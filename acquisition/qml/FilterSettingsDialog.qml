import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
	id: dialog
	modal: true
	title: "Filter Settings"
	anchors.centerIn: parent

	// Needed to avoid a binding loop with Fusion
	width: contentItem.implicitWidth

	property bool showArmourFilters: true
	property bool showTypeFilters: true
	property bool showWeaponFilters: true

	property bool showHeistFilters: false
	property bool showMapFilters: false
	property bool showMiscFilters: false
	property bool showRequirementFilters: false
	property bool showSanctumFilters: false
	property bool showSocketFilters: false
	property bool showUltimatumFilters: false

	standardButtons: Dialog.Ok

	contentItem: Pane {

		Column {

			Label {
				text: "Trade Filters:"
			}

			CheckBox {
				text: "Type Filters";
				checked: dialog.showTypeFilters
				onCheckedChanged: dialog.showTypeFilters = checked
			}

			CheckBox {
				text: "Weapon Filters";
				checked: dialog.showWeaponFilters
				onCheckedChanged: dialog.showWeaponFilters = checked
			}

			CheckBox {
				text: "Armour Filters";
				checked: dialog.showArmourFilters
				onCheckedChanged: dialog.showArmourFilters = checked
			}

			CheckBox {
				text: "Socket Filters";
				checked: dialog.showSocketFilters
				onCheckedChanged: dialog.showSocketFilters = checked
			}

			CheckBox {
				text: "Requirement Filters";
				checked: dialog.showRequirementFilters
				onCheckedChanged: dialog.showRequirementFilters = checked
			}

			CheckBox {
				text: "Map Filters";
				checked: dialog.showMapFilters
				onCheckedChanged: dialog.showMapFilters = checked
			}

			CheckBox {
				text: "Heist Filters";
				checked: dialog.showHeistFilters
				onCheckedChanged: dialog.showHeistFilters = checked
			}

			CheckBox {
				text: "Sanctum Filters";
				checked: dialog.showSanctumFilters
				onCheckedChanged: dialog.showSanctumFilters = checked
			}

			CheckBox {
				text: "Ultimatum Filters";
				checked: dialog.showUltimatumFilters
				onCheckedChanged: dialog.showUltimatumFilters = checked
			}

			CheckBox {
				text: "Miscellaneous Filters";
				checked: dialog.showMiscFilters
				onCheckedChanged: dialog.showMiscFilters = checked
			}
		}
	}
}
