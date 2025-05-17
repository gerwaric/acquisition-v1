import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import TradeFilters

ColumnLayout {
	id: filterSidePanel

	ScrollView {
		id: activeFilters

		ColumnLayout {
			Layout.margins: 10
			Label { text: "Trade Filters:" }
			TypeFilters { id: typeFilters; visible: filterSettings.showTypeFilters }
			WeaponFilters { id: weaponFilters; visible: filterSettings.showWeaponFilters }
			ArmourFilters { id: armourFilters; visible: filterSettings.showArmourFilters }
			SocketFilters { id: socketFilters; visible: filterSettings.showSocketFilters }
			RequirementFilters { id: requirementFilters; visible: filterSettings.showRequirementFilters}
			MapFilters { id: mapFilters; visible: filterSettings.showMapFilters }
			HeistFilters { id: heistFilters; visible: filterSettings.showHeistFilters }
			SanctumFilters { id: sanctumFilters; visible: filterSettings.showSanctumFilters }
			UltimatumFilters { id: ultimatumFilters; visible: filterSettings.showUltimatumFilters }
			MiscellaneousFilters { id: miscFilters; visible: filterSettings.showMiscFilters }
		}
	}

	Rectangle {
		Layout.fillHeight: true
	}

	Button {
		id: filterSettingsButton
		Layout.fillWidth: true
		text: "Filter Settings"
		onClicked: filterSettings.open()
	}
}
