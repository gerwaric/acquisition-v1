import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Pane {

	Layout.fillWidth: true
	Layout.fillHeight: true

	FilterSettingsDialog {
		id: filterSettings
	}

	SplitView {
		id: splitView
		orientation: Qt.Horizontal
		Layout.fillHeight: true

		FilterPanel {}

		ResultsPanel {}
	}

}
