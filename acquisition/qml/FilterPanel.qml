import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "TradeFilters" as TradeFilters

ColumnLayout {
	id: filterSidePanel

	ScrollView {
		id: activeFilters

		ColumnLayout {
			Layout.margins: 10
			Label { text: "Trade Filters:" }
			TradeFilters.TypeFilters { id: typeFilters; visible: filterSettings.showTypeFilters }
			TradeFilters.WeaponFilters { id: weaponFilters; visible: filterSettings.showWeaponFilters }
			TradeFilters.ArmourFilters { id: armourFilters; visible: filterSettings.showArmourFilters }
			TradeFilters.SocketFilters { id: socketFilters; visible: filterSettings.showSocketFilters }
			TradeFilters.RequirementFilters { id: requirementFilters; visible: filterSettings.showRequirementFilters}
			TradeFilters.MapFilters { id: mapFilters; visible: filterSettings.showMapFilters }
			TradeFilters.HeistFilters { id: heistFilters; visible: filterSettings.showHeistFilters }
			TradeFilters.SanctumFilters { id: sanctumFilters; visible: filterSettings.showSanctumFilters }
			TradeFilters.UltimatumFilters { id: ultimatumFilters; visible: filterSettings.showUltimatumFilters }
			TradeFilters.MiscellaneousFilters { id: miscFilters; visible: filterSettings.showMiscFilters }
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
